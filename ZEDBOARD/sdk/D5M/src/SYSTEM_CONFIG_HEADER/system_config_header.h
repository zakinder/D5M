#define S2MM_VDMA_SINGLE  XPAR_DATAPATH_PIPELINE_B_AXI_VDMA_1_BASEADDR
#define MM2S_VDMA_SINGLE  XPAR_DATAPATH_PIPELINE_B_AXI_VDMA_0_BASEADDR
#define MMSS_VDMA_PAIRED  XPAR_DATAPATH_PIPELINE_A_AXI_VDMA_0_BASEADDR
#define IIC_BASE_ADDRESS XPAR_D5M_IIC_BASEADDR
#define VGA_CONFIG_BASE_ADDRESS     XPAR_VGA_0_S_AXI_CONFIG_BASEADDR
#define SCREEN_WIDTH_HORIZONTAL 1920
#define SCREEN_HEIGHT_VERTICAL 1080
#define ADV7511_ADDR   0x72
#define carrier_hdmi_outb_config_LEN  (40)
#define CARRIER_HDMI_OUT_CONFIG_LEN  (40)
typedef unsigned char un8bits;//Unsign 8 bits
typedef char sn8bits;//Sign 8 bits
typedef unsigned short un16bits;//Unsign 16 bits
typedef short sn16bits;//Sign 16 bits
typedef unsigned long	un32bits;//Unsign 32 bits
typedef long sn32bits;//Sign 32 bits
typedef unsigned   long long    un64bits;//Unsign 64 bits
typedef signed     long long    sn64bits;//Sign 64 bits
typedef float		Xfloat32;
typedef double		Xfloat64;
typedef unsigned long	Xboolean;
