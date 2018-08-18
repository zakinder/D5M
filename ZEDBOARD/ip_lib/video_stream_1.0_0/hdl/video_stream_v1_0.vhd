library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity video_stream_v1_0 is
    generic (
    C_M00_AXIS_TDATA_WIDTH    : integer    := 24);
    port (
    m00_axis_aclk       : in std_logic;
    m00_axis_aresetn    : in std_logic;
    -- VIDEO RAW IN
    pixclk              : in std_logic;
    resetlow            : in std_logic;
    ifval               : in std_logic;
    ilval               : in std_logic;
    idata               : in std_logic_vector(11 downto 0);
    -- VIDEO OUT TO [Bayer Color Filter Array]
    rgb_m_axis_tready   : in std_logic;
    rgb_m_axis_tvalid   : out std_logic;
    rgb_m_axis_tuser    : out std_logic;
    rgb_m_axis_tlast    : out std_logic;
    rgb_m_axis_tdata    : out std_logic_vector(15 downto 0);
    -- VIDEO IN FROM [Bayer Color Filter Array]
    rgb_s_axis_tready   : out std_logic;
    rgb_s_axis_tvalid   : in std_logic;
    rgb_s_axis_tuser    : in std_logic;
    rgb_s_axis_tlast    : in std_logic;
    rgb_s_axis_tdata    : in std_logic_vector(31 downto 0);
    -- VIDEO OUT VDMA [Bayer Color Filter Array]
    m_axis_mm2s_tready  : in std_logic;
    m_axis_mm2s_tvalid  : out std_logic;
    m_axis_mm2s_tuser   : out std_logic;
    m_axis_mm2s_tlast   : out std_logic;
    m_axis_mm2s_tdata   : out std_logic_vector(23 downto 0);
    m_axis_mm2s_tkeep   : out std_logic_vector(2 downto 0);
    m_axis_mm2s_tstrb   : out std_logic_vector(2 downto 0);
    m_axis_mm2s_tid     : out std_logic_vector(0 downto 0);
    m_axis_mm2s_tdest   : out std_logic_vector(0 downto 0));
end video_stream_v1_0;
architecture arch_imp of video_stream_v1_0 is
    component video_stream_v1_0_M00_AXIS is
    port (
    pixclk             : in std_logic;
    resetlow           : in std_logic;
    ifval              : in std_logic;
    ilval              : in std_logic;
    idata              : in std_logic_vector(11 downto 0);
    --
    m_axis_aclk        : in std_logic;
    m_axis_aresetn     : in std_logic;
    m_axis_tvalid      : out std_logic;
    m_axis_tlast       : out std_logic;
    m_axis_tuser       : out std_logic;
    m_axis_tready      : in std_logic;   
    m_axis_tdata       : out std_logic_vector(15 downto 0));
    end component video_stream_v1_0_M00_AXIS;
    component video_stream_rgb_v1_0 is
    port (
    aclk               : in std_logic;
    aresetn            : in std_logic;
    -- VIDEO IN RGB
    rgb_s_axis_tready  : out std_logic;
    rgb_s_axis_tvalid  : in std_logic;
    rgb_s_axis_tuser   : in std_logic;
    rgb_s_axis_tlast   : in std_logic;
    rgb_s_axis_tdata   : in std_logic_vector(31 downto 0);
    -- VIDEO OUT 
    m_axis_mm2s_tkeep  : out std_logic_vector(2 downto 0);
    m_axis_mm2s_tstrb  : out std_logic_vector(2 downto 0);
    m_axis_mm2s_tid    : out std_logic_vector(0 downto 0);
    m_axis_mm2s_tdest  : out std_logic_vector(0 downto 0);  
    m_axis_mm2s_tready : in std_logic;
    m_axis_mm2s_tvalid : out std_logic;
    m_axis_mm2s_tuser  : out std_logic;
    m_axis_mm2s_tlast  : out std_logic;
    m_axis_mm2s_tdata  : out std_logic_vector(23 downto 0));
    end component video_stream_rgb_v1_0;
begin
-----------------------------------------------------------------------------------
video_stream_v1_0_M00_AXIS_inst: video_stream_v1_0_M00_AXIS
    port map (
    pixclk             => pixclk,
    resetlow           => resetlow,
    ifval              => ifval,
    ilval              => ilval,
    idata              => idata,
    --
    m_axis_aclk        => m00_axis_aclk,
    m_axis_aresetn     => m00_axis_aresetn,
    m_axis_tvalid      => rgb_m_axis_tvalid,
    m_axis_tlast       => rgb_m_axis_tlast,    
    m_axis_tuser       => rgb_m_axis_tuser,
    m_axis_tready      => rgb_m_axis_tready,    
    m_axis_tdata       => rgb_m_axis_tdata);
-----------------------------------------------------------------------------------
video_stream_rgb_v1_0_inst: video_stream_rgb_v1_0
    port map (
    aclk               => m00_axis_aclk,
    aresetn            => m00_axis_aresetn,
    -- VIDEO IN RGB
    rgb_s_axis_tready  => rgb_s_axis_tready,
    rgb_s_axis_tvalid  => rgb_s_axis_tvalid,    
    rgb_s_axis_tuser   => rgb_s_axis_tuser,
    rgb_s_axis_tlast   => rgb_s_axis_tlast,    
    rgb_s_axis_tdata   => rgb_s_axis_tdata,
    -- VIDEO OUT 
    m_axis_mm2s_tkeep  => m_axis_mm2s_tkeep,
    m_axis_mm2s_tstrb  => m_axis_mm2s_tstrb,
    m_axis_mm2s_tid    => m_axis_mm2s_tid,
    m_axis_mm2s_tdest  => m_axis_mm2s_tdest,
    m_axis_mm2s_tready => m_axis_mm2s_tready,
    m_axis_mm2s_tvalid => m_axis_mm2s_tvalid,
    m_axis_mm2s_tuser  => m_axis_mm2s_tuser,
    m_axis_mm2s_tlast  => m_axis_mm2s_tlast,    
    m_axis_mm2s_tdata  => m_axis_mm2s_tdata); 	
-----------------------------------------------------------------------------------
end arch_imp;
