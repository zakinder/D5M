library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity video_stream_rgb_v1_0 is
    port (
    aclk                   : in std_logic;
    aresetn                : in std_logic;
    -- VIDEO IN RGB
    rgb_s_axis_tready      : out std_logic;
    rgb_s_axis_tvalid      : in std_logic;
    rgb_s_axis_tuser       : in std_logic;
    rgb_s_axis_tlast       : in std_logic;
    rgb_s_axis_tdata       : in std_logic_vector(31 downto 0);
    -- VIDEO OUT 
    m_axis_mm2s_tkeep      : out std_logic_vector(2 downto 0);
    m_axis_mm2s_tstrb      : out std_logic_vector(2 downto 0);
    m_axis_mm2s_tid        : out std_logic_vector(0 downto 0);
    m_axis_mm2s_tdest      : out std_logic_vector(0 downto 0);  
    m_axis_mm2s_tready     : in std_logic;
    m_axis_mm2s_tvalid     : out std_logic;
    m_axis_mm2s_tuser      : out std_logic;
    m_axis_mm2s_tlast      : out std_logic;
    m_axis_mm2s_tdata      : out std_logic_vector(23 downto 0));
end video_stream_rgb_v1_0;
architecture arch_imp of video_stream_rgb_v1_0 is
    type pixel_locations is (
    video_trans_in_progress,
    wait_to_go);
    signal pixel_locations_address  : pixel_locations; 
    signal wrchx_cont               : integer  := 0;
    signal axis_tvalid              : std_logic:= '0';
    signal axis_tuser               : std_logic:= '0';
    signal axis_tlast               : std_logic:= '0';
    signal axis_tdata               : std_logic_vector(23 downto 0):= (others => '0'); 
    --------
    signal maxis_mm2s_tdata         : std_logic_vector(23 downto 0):= (others => '0');
    signal maxis_mm2s_tuser         : std_logic:= '0';
    signal maxis_mm2s_tvalid        : std_logic:= '0';
    signal maxis_mmss_tvalid        : std_logic:= '0';
    signal maxis_mm2s_tready_db     : std_logic:= '0';
    --------
    
begin

---------------------------------------------------------------------------------
    process(aclk) begin
        if rising_edge(aclk) then
            axis_tvalid <= rgb_s_axis_tvalid;
            maxis_mm2s_tready_db <= m_axis_mm2s_tready;
            axis_tuser  <= rgb_s_axis_tuser;
            axis_tdata  <= rgb_s_axis_tdata(29 downto 22) & rgb_s_axis_tdata(19 downto 12) & rgb_s_axis_tdata(9 downto 2);
        end if;
    end process;
---------------------------------------------------------------------------------
process (aclk) begin
    if (rising_edge (aclk)) then
        if (aresetn = '0') then
            pixel_locations_address <= wait_to_go;
        else
        case (pixel_locations_address) is
        when wait_to_go =>
            ---------------------
            wrchx_cont         <= 0;
            axis_tlast         <='0';
            rgb_s_axis_tready  <='1';
            maxis_mm2s_tvalid  <='0';
            ---------------------
            if (rgb_s_axis_tvalid  = '1')then
                pixel_locations_address <= video_trans_in_progress;            
            else
                pixel_locations_address <= wait_to_go;    
            end if;        
        when video_trans_in_progress =>
            -------------------------------------------------------------------
            maxis_mm2s_tuser   <= axis_tuser;
            maxis_mm2s_tdata   <= axis_tdata;
            -------------------------------------------------------------------
            wrchx_cont <= wrchx_cont + 1;
            -------------------------------------------------------------------
            if (rgb_s_axis_tlast = '1')then
                axis_tlast <='1';
                maxis_mm2s_tvalid  <='0';
                pixel_locations_address <= wait_to_go;
            else
                axis_tlast <='0';
                maxis_mm2s_tvalid  <='1';
                pixel_locations_address <= video_trans_in_progress;
            end if;
        when others =>
            pixel_locations_address <= wait_to_go;
        end case;
        end if;
    end if;
end process;
---------------------------------------------------------------------------------
    process(aclk) begin
        if rising_edge(aclk) then
            m_axis_mm2s_tkeep      <= (others => '1');
            m_axis_mm2s_tid        <= "0";
            m_axis_mm2s_tdest      <= "0";
            m_axis_mm2s_tstrb      <= (others => '1');
            m_axis_mm2s_tdata      <= maxis_mm2s_tdata;
            m_axis_mm2s_tlast      <= axis_tlast;
            m_axis_mm2s_tuser      <= maxis_mm2s_tuser;
            m_axis_mm2s_tvalid     <= maxis_mm2s_tvalid or maxis_mmss_tvalid;
        end if;
    end process;
    process(aclk) begin
        if rising_edge(aclk) then
            maxis_mmss_tvalid  <= maxis_mm2s_tvalid;
        end if;
    end process;    
end arch_imp;    
