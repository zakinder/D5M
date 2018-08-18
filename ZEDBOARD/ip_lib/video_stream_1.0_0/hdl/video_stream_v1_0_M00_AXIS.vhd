library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity video_stream_v1_0_M00_AXIS is
    port (
    --
    pixclk            : in std_logic;
    resetlow          : in std_logic;
    ifval             : in std_logic;
    ilval             : in std_logic;
    idata             : in std_logic_vector(11 downto 0);
    --
    m_axis_aclk       : in std_logic;
    m_axis_aresetn    : in std_logic;
    m_axis_tvalid     : out std_logic;
    m_axis_tlast      : out std_logic;
    m_axis_tuser      : out std_logic;
    m_axis_tready     : in std_logic;   
    m_axis_tdata      : out std_logic_vector(15 downto 0));
end video_stream_v1_0_M00_AXIS;
architecture arch_imp of video_stream_v1_0_M00_AXIS is
    constant img_width          : integer := 3071;
    constant data_width         : integer := 31;
    constant addr_width         : integer := 15;
    constant gnd                : std_logic:= '0';
    constant precision_preset   : integer := 0;
    constant precision_level_0  : integer := 0;
    constant precision_level_1  : integer := 1;
    constant precision_level_2  : integer := 2;
    constant precision_level_3  : integer := 3;
    constant precision_level_4  : integer := 4; 
    type ram_frame is array (0 to img_width) of std_logic_vector (data_width downto 0);
    signal cam_ram : ram_frame := (others => (others => '0'));    
    type pixel_locations is (
    wrchx1_state,
    reading_ram,
    wrchx2_state,
    chx2_state,
    wait_to_go);
    signal pixel_locations_address : pixel_locations; 
    type intenstate is (
    frame_idle,
    frame_going_to_start,
    frame_started_line_not_valid,
    frame_started_line_valid,
    frame_start_to_line_or_frame);
    signal video_states    : intenstate; 
    signal precision_config_end : integer := 0;
    signal sp_cont         : integer := 0;
    signal slave_cnt       : integer := 0;
    signal wrchx_cont      : integer := 0;     
    signal pixel_cont      : integer := 0;
    signal go_Read         : std_logic:= '0';
    signal tvalid_data     : std_logic:= '0';
    signal tvaliddata      : std_logic:= '0';
    signal start_of_frame  : std_logic:= '0';
    signal starts_of_frame : std_logic:= '0';
    signal idata3x         : std_logic_vector(11 downto 0):= (others => '0');
    signal idata1x         : std_logic_vector(11 downto 0):= (others => '0');
    signal idata2x         : std_logic_vector(11 downto 0):= (others => '0');
    signal mdata           : std_logic_vector(11 downto 0):= (others => '0');     
    signal inlval          : std_logic;  
    signal infval          : std_logic; 
    signal skip_frame      : std_logic:= '0';
    signal read_conter     : std_logic:= '0';
    signal skip_mframe     : std_logic:= '0';
    signal p1sof           : std_logic;  
    signal write_1enb      : std_logic;  
    signal eofs            : std_logic;  
    signal eofws           : std_logic;     
    signal eofwss          : std_logic;  
    signal w_address       : std_logic_vector(15 downto 0);
    signal w_data          : std_logic_vector(31 downto 0):= (others => '0');    
    signal ram1video       : std_logic_vector(31 downto 0);
    signal p_data          : std_logic_vector(31 downto 0);
    signal gnds            : std_logic_vector(3 downto 0):= (others => '0');    
    signal gndm            : std_logic_vector(5 downto 0):= (others => '0');  
    signal gndl            : std_logic_vector(3 downto 0):= (others => '0');      
    signal y_cont          : integer := 0; 
    signal pixelconts      : std_logic_vector(11 downto 0);
    signal r_address       : std_logic_vector(15 downto 0); 
    signal maxis_xcont     : std_logic_vector(15 downto 0);     
begin
---------------------------------------------------------------------------------
    w_data <= gnds & skip_frame &
             (starts_of_frame or start_of_frame) & 
             (eofws) & 
              tvaliddata & 
              pixelconts & 
              mdata;-- 4+1+1+1+1+12+12=32
---------------------------------------------------------------------------------
    process(pixclk) begin
        if rising_edge(pixclk) then
            inlval          <= ilval;
            infval          <= ifval;
            idata1x         <=idata;
            idata2x         <=idata1x;
            idata3x         <= idata2x;
            eofws           <=eofs;
            eofwss          <=eofws;
            tvaliddata      <=tvalid_data;
            starts_of_frame <= start_of_frame;
            w_address       <= std_logic_vector(to_unsigned(pixel_cont, 16)); 
            pixelconts      <= std_logic_vector(to_unsigned(pixel_cont, 12));
        end if;
    end process;
---------------------------------------------------------------------------------
    skip_mframe <=ram1video(27);
    process(m_axis_aclk) begin
    if rising_edge(m_axis_aclk) then
        p1sof  <=ram1video(26);
        if (ram1video(24) = '1') then
            m_axis_tdata    <=gndm & ram1video(10 downto 1);
            maxis_xcont     <=gndl & ram1video(23 downto 12);
            m_axis_tvalid   <=ram1video(24);
            m_axis_tlast    <=ram1video(25);
        else
            m_axis_tdata    <=(others => '0');
            maxis_xcont     <=(others => '0');
            m_axis_tvalid   <='0';
            m_axis_tlast    <='0';
        end if;
    end if;
    end process;
    process(m_axis_aclk) begin
    if rising_edge(m_axis_aclk) then
    if (y_cont = 0) then
        if (ram1video(24) = '1') then
            m_axis_tuser  <=p1sof;
        else
            m_axis_tuser  <='0';
        end if;
    else
            m_axis_tuser  <='0';
    end if;    
    end if;
    end process;
---------------------------------------------------------------------------------
    process (m_axis_aclk) begin
    if rising_edge(m_axis_aclk) then
    if (skip_mframe = '1') then
        if (ram1video(25) ='1' and ram1video(26)='0') then
            y_cont <= y_cont + 1;
        elsif(ram1video(25) ='0' and ram1video(26)='1') then
            y_cont <= 0;
        else
            y_cont <= y_cont;
        end if;
    end if;
    end if;
    end process;
---------------------------------------------------------------------------------
    process (pixclk) begin
    if rising_edge(pixclk) then
        if (write_1enb ='1') then
            cam_ram(to_integer(unsigned(w_address))) <= w_data;
        end if;
    end if;
    end process;
---------------------------------------------------------------------------------
    process (m_axis_aclk) begin
    if rising_edge(m_axis_aclk) then
        p_data <= cam_ram(to_integer(unsigned(r_address)));
    end if;
    end process;
---------------------------------------------------------------------------------
    process (m_axis_aclk) begin
    if rising_edge(m_axis_aclk) then
        ram1video  <= p_data;
    end if;
    end process;
---------------------------------------------------------------------------------
process (pixclk) begin
    if (rising_edge (pixclk)) then
        if (resetlow = '0') then
            video_states <= frame_idle;
        else
        case (video_states) is
        when frame_idle =>
            start_of_frame <='0';
            write_1enb     <='0';
            eofs           <='0';
            pixel_cont     <= 0;
            tvalid_data    <='0';
            go_Read        <='0';
            read_conter    <='0';
            mdata          <= (others => '0');
            if (infval = '1') then
                video_states <= frame_going_to_start;
            else
                video_states <= frame_idle;
            end if;
        when frame_going_to_start =>
            tvalid_data <='0';
            if (infval = '1')then
                if (inlval = '1')then
                    video_states <= frame_started_line_valid;
                    start_of_frame <='1';            
                else
                    video_states <= frame_going_to_start;
                    start_of_frame <='0';
                end if;
            else
                video_states <= frame_idle;
                skip_frame   <='1';
            end if;
        when frame_started_line_valid =>
            if (read_conter = '1')then
                mdata <= pixelconts;            
            else
                mdata <= idata3x;
            end if;       
            start_of_frame <= '0';
            tvalid_data    <='1';
            pixel_cont     <= pixel_cont + 1;
            write_1enb <='1';
            if (infval = '1')then
                if (inlval = '0')then
                    video_states <= frame_started_line_not_valid;
                    go_Read <='1';
                    eofs <='1';
                else
                    video_states <= frame_started_line_valid;
                    go_Read <='0';
                end if;
            else
                video_states <= frame_idle;
                skip_frame <='1';
                go_Read <='0';
            end if;
        when frame_started_line_not_valid =>
            go_Read <='0';
            ---------------------------------------------------
            -- end of line operations
            ---------------------------------------------------
            precision_config_end <= precision_config_end + 1;
            ---------------------------------------------------
            if (precision_config_end = precision_level_0 )then
            if (read_conter = '1')then
                mdata   <= pixelconts;            
            else
                mdata   <= idata3x;
            end if;  
            else
                mdata   <= (others => '0');
            end if;
            ---------------------------------------------------
            if (precision_config_end = precision_level_0 )then
                slave_cnt <= sp_cont;
            end if;
            ---------------------------------------------------
            if (precision_config_end = precision_level_0 )then
                sp_cont   <= pixel_cont;
            end if;
            ---------------------------------------------------
            if (precision_config_end <= precision_level_1)then
                write_1enb  <='1';
            else
                write_1enb  <='0';
            end if;  
            ---------------------------------------------------
            if (precision_config_end = precision_level_0 )then
                eofs        <='0';
            end if;
            ---------------------------------------------------
            if (precision_config_end = precision_level_0)then
                tvalid_data <='0';    
            end if;
            ---------------------------------------------------
            if (precision_config_end = precision_level_0)then
                pixel_cont <= pixel_cont + 1;
            else
                pixel_cont <= 0;
            end if;    
            ---------------------------------------------------
            if (precision_config_end = precision_level_3)then
                video_states <= frame_start_to_line_or_frame;
            else
                video_states <= frame_started_line_not_valid;
            end if;
            ---------------------------------------------------
        when frame_start_to_line_or_frame =>
            precision_config_end <= precision_preset;
            pixel_cont <= 0;
            go_Read    <='0';
            if (infval = '1')then
                if (inlval = '1')then
                    video_states <= frame_started_line_valid;
                else
                    video_states <= frame_start_to_line_or_frame;
                end if;
            else
                video_states <= frame_idle;
                skip_frame <='1';
            end if;
        when others =>
            video_states <= frame_idle;
        end case;
        end if;
    end if;
end process;
---------------------------------------------------------------------------------
process (m_axis_aclk) begin
    if (rising_edge (m_axis_aclk)) then
        if (m_axis_aresetn = '0') then
            pixel_locations_address <= wait_to_go;
        else
        case (pixel_locations_address) is
        when wait_to_go =>
            wrchx_cont <= 0;
            if (go_Read  = '1' and (slave_cnt /= 0))then
                pixel_locations_address <= reading_ram;            
            else
                pixel_locations_address <= wait_to_go;    
            end if;        
        when reading_ram =>
            wrchx_cont <= wrchx_cont + 1;
            r_address <= std_logic_vector(to_unsigned(wrchx_cont, 16)); 
            if (wrchx_cont = slave_cnt + 1) then
                wrchx_cont <= 0;
                pixel_locations_address <= wait_to_go;
            else
                pixel_locations_address <= reading_ram;
            end if;
        when others =>
            pixel_locations_address <= wait_to_go;
        end case;
        end if;
    end if;
end process;
---------------------------------------------------------------------------------
end arch_imp;    
