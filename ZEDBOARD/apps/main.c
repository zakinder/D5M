#include <stdio.h>
#include <xbasic_types.h>
#include <xil_printf.h>
#include <xparameters.h>
#include <xparameters_ps.h>
#include "CFA/cfa.h"
#include "HDMI_DISPLAY/hdmi_display.h"
#include "I2C_D5M/i2c_d5m.h"
#include "MENU_CALLS/menu_calls.h"
#include "PLATFORM/platform.h"
hdmi_display_start video;
int main(void)
{
	Xuint32 status, reg_val;
	int ret;
	CFA_Reset(XPAR_CFA_0_BASEADDR);
	CFA_ClearReset(XPAR_CFA_0_BASEADDR);
	CFA_Enable(XPAR_CFA_0_BASEADDR);
	xil_printf( "\n\r" );
	xil_printf( "-------------------11/06/2017-------------------\n\r" );
	printf("\t Color Filter Array Core Configuration:\n\r");
	printf("\t Enable Bit            = %1lu\n\r",CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_CONTROL) & CFA_CTL_EN_MASK);
	printf("\t Register Update Bit   = %1lu\n\r",(CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_CONTROL) & CFA_CTL_RUE_MASK) >> 1);
	printf("\t Reset Bit             = %1lu\n\r",CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_CONTROL) & CFA_RST_RESET);
	status = CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_STATUS);
	printf("\t CFA Status            = %08x\n\r", (unsigned int) status);
	printf("\t Core Version          = %1lu.%1lu\n\r", (status & 0x78) >> 3,(status & 0x07));
	reg_val = CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_CONTROL );
	printf("\t CFA_CONTROL           = %8x\n\r", (unsigned int) reg_val);
	reg_val = CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_IRQ_EN );
	printf("\t CFA Interrupt Control = %8x\n\r", (unsigned int) reg_val);
	reg_val = (CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_ACTIVE_SIZE )) >> 16;
	printf("\t CFA Active Rows       = %8d\n\r", (unsigned int) reg_val);
	reg_val = (CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_ACTIVE_SIZE ) & 0x0000FFFF);
	printf("\t CFA Active Cols       = %8d\n\r", (unsigned int) reg_val);
	reg_val = CFA_ReadReg(XPAR_CFA_0_BASEADDR, CFA_BAYER_PHASE );
	printf("\t CFA_BAYER_PHASE       = %8u\n\r", (unsigned int) reg_val);
	xil_printf( "-----------------------------------------------\n\r" );
    init_platform();
    //camera_set_registers();
    cameraread();
	xil_printf( "-----------------------------------------------\n\r" );
    video.uBaseAddr_IIC_HdmiOut = XPAR_HDMI_OUTPUT_HDMI_IIC_BASEADDR;
    video.uDeviceId_VTC_HdmioGenerator = XPAR_VIDEO_PIPELINE_TIMMING_CONTROLELR_DEVICE_ID;
    video.uDeviceId_VDMA_HdmiDisplay = XPAR_AXIVDMA_0_DEVICE_ID;
    video.uBaseAddr_MEM_HdmiDisplay = XPAR_DDR_MEM_BASEADDR + 0x100000;
    video.uNumFrames_HdmiDisplay = XPAR_AXIVDMA_0_NUM_FSTORES;
    ret = hdmi_display_init( &video );
    if (!ret)
    {
    xil_printf( "ERROR : Failed to initialize hdmi_display_init\n\r\r" );
    }
	xil_printf( "-----------------------------------------------\n\r" );
	xil_printf( "----------------SETUP IS COMPLETED-------------\n\r" );
	xil_printf( "-----------------------------------------------\n\r" );
    menu_calls();
    return 0;
}
