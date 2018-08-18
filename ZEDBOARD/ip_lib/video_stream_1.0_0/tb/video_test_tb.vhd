library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use std.textio.all;
use ieee.std_logic_textio.all;


use ieee.math_real.all;

use work.CommonPckg.all;
use work.AXI_Procedures_pk.all;


entity video_test_tb is
end video_test_tb;
architecture behavioral of video_test_tb is

	constant axi_araddr_adc_0 : integer := 0;
	constant axi_araddr_adc_1 : integer := 4;
	constant axi_araddr_adc_2 : integer := 8;
	constant axi_araddr_adc_3 : integer := 12;
	constant axi_araddr_adc_4 : integer := 16;
	constant axi_araddr_adc_5 : integer := 20;
	constant axi_araddr_adc_6 : integer := 24;
	constant axi_araddr_adc_7 : integer := 28;
	constant axi_araddr_Read_ADC_Reg_Reg_8 : integer := 32;
	constant axi_araddr_Write_ADC_Reg_Reg_9 : integer := 36;
	constant axi_araddr_Delay_Reg_10 : integer := 40;
	constant axi_araddr_ADC_Serial_Slow_Interrupt_Read_11 : integer := 44;
	constant axi_araddr_Interrupt_Status_Reg_12 : integer := 48;
	constant axi_araddr_Interrupt_Enable_Reg_13 : integer := 52;
	constant axi_araddr_ADC_Value_Write_Reg_14 : integer := 56;
	constant axi_araddr_Register_Read_Write_15 : integer := 60;
	----------------------------------------------------------------------------------
	constant C_axi_DATA_WIDTH : integer := 32;
	constant C_axi_ADDR_WIDTH : integer := 6;


    constant LINE_WIDTH      :  integer:=1079;
    constant LINE_HIGHT      :  integer:=4;
    signal FVAL      :  std_logic:='0';
    signal LVAL      :  std_logic:='0';
    signal cam_reset         :  std_logic:='0';
    constant revision_number : std_logic_vector(31 downto 0) := x"03202017";
    constant C_s_axis_video_test_TDATA_WIDTH   : integer     := 24;
    constant C_s_axis_video_test_START_COUNT   : integer     := 32;
    constant C_s_conf_video_test_DATA_WIDTH    : integer     := 32;
    constant C_s_conf_video_test_ADDR_WIDTH    : integer     := 4;
    type camlink_buffer_bus is record
        x_cnt       : integer range 0 to 640;
        y_cnt       : integer range 0 to 640;
        frame_high  : integer range 0 to 640;
        valid_high  : integer range 0 to 640;
        data        : integer range 0 to 640;
        datas       : integer range 0 to 4096;
        fvalid      : std_logic;
        lvalid      : std_logic;
    end record;
    signal video : camlink_buffer_bus;
    component video_stream_v1_0 is
    generic (
    C_M00_AXIS_TDATA_WIDTH    : integer     := 24);
    port (
    m00_axis_aclk       : in std_logic;
    m00_axis_aresetn    : in std_logic;
    pixclk      : in std_logic;
    resetlow    : in std_logic;
    ifval       : in std_logic;
    ilval       : in std_logic;
    idata       : in std_logic_vector(11 downto 0);
    rgb_m_axis_tready   : in std_logic;
    rgb_m_axis_tvalid   : out std_logic;
    rgb_m_axis_tuser    : out std_logic;
    rgb_m_axis_tlast    : out std_logic;
    rgb_m_axis_tdata    : out std_logic_vector(15 downto 0);
    rgb_s_axis_tready   : out std_logic;
    rgb_s_axis_tvalid   : in std_logic;
    rgb_s_axis_tuser    : in std_logic;
    rgb_s_axis_tlast    : in std_logic;
    rgb_s_axis_tdata    : in std_logic_vector(31 downto 0);
    m_axis_mm2s_tready  : in std_logic;
    m_axis_mm2s_tvalid  : out std_logic;
    m_axis_mm2s_tuser   : out std_logic;
    m_axis_mm2s_tlast   : out std_logic;
    m_axis_mm2s_tdata   : out std_logic_vector(23 downto 0);
    m_axis_mm2s_tkeep   : out std_logic_vector(2 downto 0);
    m_axis_mm2s_tstrb   : out std_logic_vector(2 downto 0);
    m_axis_mm2s_tid     : out std_logic_vector(0 downto 0);
    m_axis_mm2s_tdest   : out std_logic_vector(0 downto 0));
    end component video_stream_v1_0;
    
component zynq_soc_Color_0 is
      Port ( 
        aclk : in STD_LOGIC;
        aclken : in STD_LOGIC;
        aresetn : in STD_LOGIC;
        s_axi_aclk : in STD_LOGIC;
        s_axi_aclken : in STD_LOGIC;
        s_axi_aresetn : in STD_LOGIC;
        irq : out STD_LOGIC;
        s_axis_video_tdata : in STD_LOGIC_VECTOR ( 15 downto 0 );
        s_axis_video_tready : out STD_LOGIC;
        s_axis_video_tvalid : in STD_LOGIC;
        s_axis_video_tlast : in STD_LOGIC;
        s_axis_video_tuser : in STD_LOGIC;
        m_axis_video_tdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
        m_axis_video_tvalid : out STD_LOGIC;
        m_axis_video_tready : in STD_LOGIC;
        m_axis_video_tlast : out STD_LOGIC;
        m_axis_video_tuser : out STD_LOGIC;
        s_axi_awaddr : in STD_LOGIC_VECTOR ( 8 downto 0 );
        s_axi_awvalid : in STD_LOGIC;
        s_axi_awready : out STD_LOGIC;
        s_axi_wdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
        s_axi_wstrb : in STD_LOGIC_VECTOR ( 3 downto 0 );
        s_axi_wvalid : in STD_LOGIC;
        s_axi_wready : out STD_LOGIC;
        s_axi_bresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
        s_axi_bvalid : out STD_LOGIC;
        s_axi_bready : in STD_LOGIC;
        s_axi_araddr : in STD_LOGIC_VECTOR ( 8 downto 0 );
        s_axi_arvalid : in STD_LOGIC;
        s_axi_arready : out STD_LOGIC;
        s_axi_rdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
        s_axi_rresp : out STD_LOGIC_VECTOR ( 1 downto 0 );
        s_axi_rvalid : out STD_LOGIC;
        s_axi_rready : in STD_LOGIC
      );
    
    end component zynq_soc_Color_0;
    
    
    procedure clk_gen(signal clk : out std_logic; constant freq : real) is
    constant period    : time := 1 sec / freq;
    constant high_time : time := period / 2;
    constant low_time  : time := period - high_time;
    begin
        loop
    clk <= '1';
        wait for high_time;
    clk <= '0';
        wait for low_time;
    end loop;
    end procedure;
    signal pixclk     :  std_logic;
    signal resetlow          :  std_logic;
    signal ifval      :  std_logic;
    signal ilval      :  std_logic;
    signal idata      :  std_logic_vector(11 downto 0);

    signal s_axis_video_test_aresetn :  std_logic;
    signal rgb_m_axis_tready   :  std_logic;
    signal rgb_m_axis_tvalid   :  std_logic;
    signal rgb_m_axis_tuser    :  std_logic;
    signal rgb_m_axis_tlast    :  std_logic;
    signal rgb_m_axis_tdata    :  std_logic_vector(15 downto 0);
    signal rgb_s_axis_tready   :  std_logic;
    signal rgb_s_axis_tvalid   :  std_logic;
    signal rgb_s_axis_tuser    :  std_logic;
    signal rgb_s_axis_tlast    :  std_logic;
    signal rgb_s_axis_tdata    :  std_logic_vector(31 downto 0);
    signal m_axis_mm2s_tready  :  std_logic;
    signal m_axis_mm2s_tvalid  :  std_logic;
    signal m_axis_mm2s_tuser   :  std_logic;
    signal m_axis_mm2s_tlast   :  std_logic;
    signal m_axis_mm2s_tdata   :  std_logic_vector(23 downto 0);
    signal m_axis_mm2s_tkeep   :  std_logic_vector(2 downto 0);
    signal m_axis_mm2s_tstrb   :  std_logic_vector(2 downto 0);
    signal m_axis_mm2s_tid     :  std_logic_vector(0 downto 0);
    signal m_axis_mm2s_tdest   :  std_logic_vector(0 downto 0);      
    
    
    signal aclk : STD_LOGIC;
    signal aclken : STD_LOGIC;
    signal aresetn : STD_LOGIC;
    signal s_axi_aclk : STD_LOGIC;
    signal s_axi_aclken : STD_LOGIC;
    signal s_axi_aresetn : STD_LOGIC;
    signal irq : STD_LOGIC;
    signal s_axis_video_tdata : STD_LOGIC_VECTOR ( 15 downto 0 );
    signal s_axis_video_tready : STD_LOGIC;
    signal s_axis_video_tvalid : STD_LOGIC;
    signal s_axis_video_tlast : STD_LOGIC;
    signal s_axis_video_tuser : STD_LOGIC;
    signal m_axis_video_tdata : STD_LOGIC_VECTOR ( 31 downto 0 );
    signal m_axis_video_tvalid : STD_LOGIC;
    signal m_axis_video_tready : STD_LOGIC;
    signal m_axis_video_tlast : STD_LOGIC;
    signal m_axis_video_tuser : STD_LOGIC;
    signal s_axi_awaddr : STD_LOGIC_VECTOR ( 8 downto 0 );
    signal s_axi_awvalid : STD_LOGIC;
    signal s_axi_awready : STD_LOGIC;
    signal s_axi_wdata : STD_LOGIC_VECTOR ( 31 downto 0 );
    signal s_axi_wstrb : STD_LOGIC_VECTOR ( 3 downto 0 );
    signal s_axi_wvalid : STD_LOGIC;
    signal s_axi_wready : STD_LOGIC;
    signal s_axi_bresp : STD_LOGIC_VECTOR ( 1 downto 0 );
    signal s_axi_bvalid : STD_LOGIC;
    signal s_axi_bready : STD_LOGIC;
    signal s_axi_araddr : STD_LOGIC_VECTOR ( 8 downto 0 );
    signal s_axi_arvalid : STD_LOGIC;
    signal s_axi_arready : STD_LOGIC;
    signal s_axi_rdata : STD_LOGIC_VECTOR ( 31 downto 0 );
    signal s_axi_rresp : STD_LOGIC_VECTOR ( 1 downto 0 );
    signal s_axi_rvalid : STD_LOGIC;
    signal s_axi_rready : STD_LOGIC;
    signal CLK_75MHZ : STD_LOGIC;
    signal CLK_150MHZ : STD_LOGIC;
    
    
     signal axi_aclk: std_logic;
    signal axi_aresetn: std_logic := LO;
    signal axi_awaddr: std_logic_vector(5 downto 0);
    signal axi_awprot: std_logic_vector(2 downto 0);
    signal axi_awvalid: std_logic;
    signal axi_awready: std_logic;
    signal axi_wdata: std_logic_vector(31 downto 0);
    signal axi_wstrb: std_logic_vector(3 downto 0);
    signal axi_wvalid: std_logic;
    signal axi_wready: std_logic;
    signal axi_bresp: std_logic_vector(1 downto 0);
    signal axi_bvalid: std_logic;
    signal axi_bready: std_logic;
    signal axi_araddr: std_logic_vector(5 downto 0);
    signal axi_arprot: std_logic_vector(2 downto 0);
    signal axi_arvalid: std_logic;
    signal axi_arready: std_logic;
    signal axi_rdata: std_logic_vector(31 downto 0);
    signal axi_rresp: std_logic_vector(1 downto 0);
    signal axi_rvalid: std_logic;
    signal axi_rready: std_logic;   
    
    
begin
    clk_gen(pixclk, 96.00e6);
    clk_gen(pixclk, 96.00e6);
    clk_gen(CLK_75MHZ, 75.00e6);
    clk_gen(CLK_150MHZ, 150.00e6);
    process begin
        cam_reset       <= '0';
        resetlow        <= '0';
        

        
        s_axis_video_test_aresetn <= '0';
        wait for 100 ns;
        s_axis_video_test_aresetn <= '1';
        resetlow        <= '1';    
        wait for 200 ns;
        cam_reset       <= '1';
        wait for 200 ns;
        -------------------------------------------------------------------------------------------
        AXI_Write_Read(axi_araddr_ADC_Serial_Slow_Interrupt_Read_11, 0, 800, 200, 15, HI, HI, HI, axi_araddr_ADC_Serial_Slow_Interrupt_Read_11,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        AXI_Write_Read(axi_araddr_adc_3, 0, 500, 500, 15, HI, HI, HI, axi_araddr_adc_3,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        -- STOP
        AXI_Write(36, 0, 0, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Write(32, 0, 0, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        -- Put Data
        AXI_Write(56, 0, 2000, 2000, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        -- Write
        AXI_Write(36, 0, 536870911, 536870911, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        -- STOP
        AXI_Write(36, 0, 0, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        -- Put Data
        AXI_Write(56, 0, 800, 800, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        -- Write
        AXI_Write(36, 0, 536870911, 536870911, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Write(32, 0, 536870911, 536870911, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Write(36, 0, 0, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        -- Put Delay_Reg
        AXI_Write(40, 0, 0, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        -- Put Data
        --AXI_Write("101100",0,12,12,15,HI,HI,HI,
        --axi_AWADDR,axi_WSTRB,axi_WDATA,axi_AWVALID,axi_WVALID,axi_AWREADY,axi_BREADY,axi_BVALID);--CLK_DIV_Reg
        wait for 200 ns;
        AXI_Read(axi_araddr_adc_0, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --ADC_CH_0
        AXI_Read(axi_araddr_adc_1, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --ADC_CH_1
        AXI_Read(axi_araddr_adc_2, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --ADC_CH_2
        AXI_Read(axi_araddr_adc_3, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --ADC_CH_3
        AXI_Read(axi_araddr_adc_4, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --ADC_CH_4
        AXI_Read(axi_araddr_adc_5, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --ADC_CH_5
        AXI_Read(axi_araddr_adc_6, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --ADC_CH_6
        AXI_Read(axi_araddr_adc_7, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --ADC_CH_7
        AXI_Read(axi_araddr_Read_ADC_Reg_Reg_8, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --adc_ADC_Read_reg
        AXI_Read(axi_araddr_Write_ADC_Reg_Reg_9, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --write_cr_reg
        AXI_Read(axi_araddr_Delay_Reg_10, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --delay_reg
        AXI_Read(axi_araddr_ADC_Serial_Slow_Interrupt_Read_11, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --clk_divider_reg
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --IP_INTR_status_reg
        AXI_Read(axi_araddr_Interrupt_Enable_Reg_13, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --IP_INTR_enable_reg
        AXI_Read(axi_araddr_ADC_Value_Write_Reg_14, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --cr_reg
        AXI_Read(axi_araddr_Register_Read_Write_15, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID); --Register_Read_Write
        wait for 100 ns;
        AXI_Write(60, 0, 1, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 2, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 3, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 4, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 5, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 6, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 7, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 8, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 9, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 10, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 11, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        wait for 100 ns;
        AXI_Write(60, 0, 12, 0, 15, HI, HI, HI,
        axi_AWADDR, axi_WSTRB, axi_WDATA, axi_AWVALID, axi_WVALID, axi_AWREADY, axi_BREADY, axi_BVALID);
        AXI_Read(axi_araddr_Interrupt_Status_Reg_12, axi_ARADDR, axi_ARREADY, axi_ARVALID, axi_RREADY, axi_RVALID);
        -------------------------------------------------------------------------------------------
        wait;
    end process;
    process (pixclk,cam_reset) begin
        if (cam_reset ='0') then 
    video.frame_high <= 0;
        elsif (rising_edge(pixclk) ) then
    if (video.valid_high = LINE_WIDTH + 40) then
        if (video.frame_high > LINE_HIGHT) then
     video.frame_high <= 0;
        else
     video.frame_high <= video.frame_high + 1;
        end if;
    end if;
        end if;
    end process;
    process (pixclk,cam_reset) begin
        if (cam_reset ='0') then
    video.valid_high <= 0;
        elsif(rising_edge(pixclk) ) then
    if (video.valid_high > LINE_WIDTH + 40) then
        video.valid_high <= 1;
    else
        video.valid_high <= video.valid_high + 1;
    end if;
        end if;
    end process;
    process (pixclk,cam_reset) begin
        if (cam_reset ='0') then
    video.data <= 0;
        elsif(rising_edge(pixclk) ) then
    if (video.data > LINE_WIDTH + 40) then
        video.data <= 1;
    else
        video.data <= video.data + 1;
    end if;
        end if;
    end process;
    process (pixclk,cam_reset) begin
        if (cam_reset ='0') then
    video.datas <= 1;
        elsif(rising_edge(pixclk) ) then
    if (video.data > 1081) then
        video.datas <= 9;
    else
        video.datas <= video.datas + 1;
    end if;
        end if;
    end process;    
    video.fvalid <='1' when (video.frame_high < LINE_HIGHT and cam_reset ='1') else '0';
    video.lvalid <='1' when (video.valid_high < LINE_WIDTH and cam_reset ='1') else '0';
    idata        <= std_logic_vector(to_unsigned(video.datas, 12)) when (video.fvalid ='1' and video.lvalid ='1') else (others => '0');
video_stream_inst: video_stream_v1_0
generic map(
    C_M00_AXIS_TDATA_WIDTH => 24)
port map(
	--in
    m00_axis_aclk        =>     CLK_150MHZ,
    m00_axis_aresetn     =>     s_axis_video_test_aresetn,
    pixclk       =>     pixclk,
    resetlow     =>     resetlow,
    ifval        =>     video.fvalid,
    ilval        =>     video.lvalid,
    idata        =>     idata,
	--out
    rgb_m_axis_tready    =>     rgb_m_axis_tready,
    rgb_m_axis_tvalid    =>     rgb_m_axis_tvalid,
    rgb_m_axis_tuser     =>     rgb_m_axis_tuser,
    rgb_m_axis_tlast     =>     rgb_m_axis_tlast,
    rgb_m_axis_tdata     =>     rgb_m_axis_tdata,
	--in
    rgb_s_axis_tready    =>     m_axis_video_tready,
    rgb_s_axis_tvalid    =>     m_axis_video_tvalid,
    rgb_s_axis_tuser     =>     m_axis_video_tuser,
    rgb_s_axis_tlast     =>     m_axis_video_tlast,
    rgb_s_axis_tdata     =>     m_axis_video_tdata,
	--out video
    m_axis_mm2s_tready   =>     m_axis_mm2s_tready,
    m_axis_mm2s_tvalid   =>     m_axis_mm2s_tvalid,
    m_axis_mm2s_tuser    =>     m_axis_mm2s_tuser,
    m_axis_mm2s_tlast    =>     m_axis_mm2s_tlast,
    m_axis_mm2s_tdata    =>     m_axis_mm2s_tdata,
    m_axis_mm2s_tkeep    =>     m_axis_mm2s_tkeep,
    m_axis_mm2s_tstrb    =>     m_axis_mm2s_tstrb,
    m_axis_mm2s_tid      =>     m_axis_mm2s_tid,
    m_axis_mm2s_tdest    =>     m_axis_mm2s_tdest );
zynq_soc_Color_inst: zynq_soc_Color_0
    port map(
    aclk                  => CLK_150MHZ,
    aclken                => resetlow,
    aresetn               => s_axis_video_test_aresetn,
    s_axi_aclk            => CLK_75MHZ,
    s_axi_aclken          => s_axi_aclken,
    s_axi_aresetn         => resetlow,
    irq                   => irq,
	--in
    s_axis_video_tdata    => rgb_m_axis_tdata,
    s_axis_video_tready   => rgb_m_axis_tready,
    s_axis_video_tvalid   => rgb_m_axis_tvalid,
    s_axis_video_tlast    => rgb_m_axis_tlast,
    s_axis_video_tuser    => rgb_m_axis_tuser,
	--out
    m_axis_video_tdata    => m_axis_video_tdata,
    m_axis_video_tvalid   => m_axis_video_tvalid,
    m_axis_video_tready   => m_axis_video_tready,
    m_axis_video_tlast    => m_axis_video_tlast,
    m_axis_video_tuser    => m_axis_video_tuser,
	--in axilite
    s_axi_awaddr          => s_axi_awaddr,
    s_axi_awvalid         => s_axi_awvalid,
    s_axi_awready         => s_axi_awready,
    s_axi_wdata           => s_axi_wdata,
    s_axi_wstrb           => s_axi_wstrb,
    s_axi_wvalid          => s_axi_wvalid,
    s_axi_wready          => s_axi_wready,
    s_axi_bresp           => s_axi_bresp,
    s_axi_bvalid          => s_axi_bvalid,
    s_axi_bready          => s_axi_bready,
    s_axi_araddr          => s_axi_araddr,
    s_axi_arvalid         => s_axi_arvalid,
    s_axi_arready         => s_axi_arready,
    s_axi_rdata           => s_axi_rdata,
    s_axi_rresp           => s_axi_rresp,
    s_axi_rvalid          => s_axi_rvalid,
    s_axi_rready          => s_axi_rready);
end behavioral;
