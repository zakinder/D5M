#include "hdmi_display.h"

#include <sleep.h>
#include <xil_cache.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xparameters.h>

#include "../MENU_CALLS/menu_calls.h"
#include "../SYSTEM_CONFIG_HEADER/system_config_header.h"
#include "../TESTPATTERN/pattern.h"
#include "../VIDEO_FRAME_BUFFER/video_frame_buffer.h"
#include "../VIDEO_GENERATOR/video_generator.h"
#include "../VIDEO_RESOLUTION/video_resolution.h"

Xuint8 carrier_hdmi_out_config[CARRIER_HDMI_OUT_CONFIG_LEN][3] =
{
    {ADV7511_ADDR>>1, 0x15, 0x01},
    {ADV7511_ADDR>>1, 0x16, 0x38},
    {ADV7511_ADDR>>1, 0x18, 0xAC},
    {ADV7511_ADDR>>1, 0x19, 0x53},
    {ADV7511_ADDR>>1, 0x1A, 0x08},
    {ADV7511_ADDR>>1, 0x1B, 0x00},
    {ADV7511_ADDR>>1, 0x1C, 0x00},
    {ADV7511_ADDR>>1, 0x1D, 0x00},
    {ADV7511_ADDR>>1, 0x1E, 0x19},
    {ADV7511_ADDR>>1, 0x1F, 0xD6},
    {ADV7511_ADDR>>1, 0x20, 0x1C},
    {ADV7511_ADDR>>1, 0x21, 0x56},
    {ADV7511_ADDR>>1, 0x22, 0x08},
    {ADV7511_ADDR>>1, 0x23, 0x00},
    {ADV7511_ADDR>>1, 0x24, 0x1E},
    {ADV7511_ADDR>>1, 0x25, 0x88},
    {ADV7511_ADDR>>1, 0x26, 0x02},
    {ADV7511_ADDR>>1, 0x27, 0x91},
    {ADV7511_ADDR>>1, 0x28, 0x1F},
    {ADV7511_ADDR>>1, 0x29, 0xFF},
    {ADV7511_ADDR>>1, 0x2A, 0x08},
    {ADV7511_ADDR>>1, 0x2B, 0x00},
    {ADV7511_ADDR>>1, 0x2C, 0x0E},
    {ADV7511_ADDR>>1, 0x2D, 0x85},
    {ADV7511_ADDR>>1, 0x2E, 0x18},
    {ADV7511_ADDR>>1, 0x2F, 0xBE},
    {ADV7511_ADDR>>1, 0x41, 0x10},
    {ADV7511_ADDR>>1, 0x48, 0x08},
    {ADV7511_ADDR>>1, 0x55, 0x00},
    {ADV7511_ADDR>>1, 0x56, 0x28},
    {ADV7511_ADDR>>1, 0x98, 0x03},
    {ADV7511_ADDR>>1, 0x9A, 0xE0},
    {ADV7511_ADDR>>1, 0x9C, 0x30},
    {ADV7511_ADDR>>1, 0x9D, 0x61},
    {ADV7511_ADDR>>1, 0xA2, 0xA4},
    {ADV7511_ADDR>>1, 0xA3, 0xA4},
    {ADV7511_ADDR>>1, 0xAF, 0x04},
    {ADV7511_ADDR>>1, 0xBA, 0x60},
    {ADV7511_ADDR>>1, 0xE0, 0xD0},
    {ADV7511_ADDR>>1, 0xF9, 0x00}
};
int hdmi_display_init( hdmi_display_start *pvideo )
{
    int ret;
    u32 uart_quit;
    // *****************************
    // vfb_common_init
    // *****************************
    pvideo->hdmio_resolution = VIDEO_RESOLUTION_1080P;
    pvideo->hdmio_width  = SCREEN_WIDTH_HORIZONTAL;
    pvideo->hdmio_height = SCREEN_HEIGHT_VERTICAL;
    // *****************************
    // vfb_common_init
    // *****************************
    ret = zed_iic_axi_init(&(pvideo->hdmi_out_iic),"HDMI I2C Controller", pvideo->uBaseAddr_IIC_HdmiOut );
    if ( !ret )
    {
    xil_printf( "ERROR : Failed to initialize IIC driver\n\r" );
    return -1;
    }
    xil_printf( "HDMI Output Initialization ...\n\r" );
    {
    Xuint8 num_bytes;
    int i;
    for ( i = 0; i < CARRIER_HDMI_OUT_CONFIG_LEN; i++ )
    {
    //xil_printf( "[ZedBoard HDMI] IIC Write - Device = 0x%02X, Address = 0x%02X, Data = 0x%02X\n\r", carrier_hdmi_out_config[i][0]<<1, carrier_hdmi_out_config[i][1], carrier_hdmi_out_config[i][2] );
    num_bytes = pvideo->hdmi_out_iic.fpIicWrite( &(pvideo->hdmi_out_iic), carrier_hdmi_out_config[i][0], carrier_hdmi_out_config[i][1], &(carrier_hdmi_out_config[i][2]), 1 );
    }
    }
    xil_printf( "Clear Frame Buffer\n\r" );
    // *****************************
    // vfb_common_init
    // *****************************
    vfb_common_init(pvideo->uDeviceId_VDMA_HdmiDisplay,&(pvideo->vdma_hdmi));
    // *****************************
    // vfb_tx_init
    // *****************************
    vfb_tx_init(&(pvideo->vdma_hdmi),&(pvideo->vdmacfg_hdmi_read),
    pvideo->hdmio_resolution,pvideo->hdmio_resolution,pvideo->uBaseAddr_MEM_HdmiDisplay,pvideo->uNumFrames_HdmiDisplay);
    xil_printf( "Video Timing Controller (generator) Initialization ...\n\r" );
    // *****************************
    // vgen_init
    // *****************************
    vgen_init( &(pvideo->vtc_hdmio_generator), pvideo->uDeviceId_VTC_HdmioGenerator );
    // *****************************
    // vgen_config
    // *****************************
    vgen_config( &(pvideo->vtc_hdmio_generator), pvideo->hdmio_resolution, 0 );
    u32 menu_enable  = 0;
    // *****************************
    // RE-Start
    // *****************************
    while(menu_enable != cmds_space){
    //menu_enable = uart_prompt_io();
    // *****************************
    // uart_quit
    // *****************************
    xil_printf( "\n\rPress ENTER to re-start ...\n\r" );
    //uart_quit = uart_prompt_io();
    //if (uart_quit == cmds_space) {return 0;}
    // *****************************
    // Xil_DCacheFlush
    // *****************************
    Xil_DCacheFlush();
    // *****************************
    // vfb_tx_stop
    // *****************************
    vfb_tx_stop( &(pvideo->vdma_hdmi) );
    // *****************************
    // hdmi_display
    // *****************************
    hdmi_display(&(pvideo->uBaseAddr_MEM_HdmiDisplay));
    // *****************************
    // vfb_tx_start
    // *****************************
    vfb_tx_start( &(pvideo->vdma_hdmi) );
    xil_printf( "HDMI Output Re-Initialization ...\n\r" );
    {
    // *****************************
    // hdmi_out_iic.fpIicWrite
    // *****************************
    Xuint8 num_bytes;
    int i;
    for ( i = 0; i < CARRIER_HDMI_OUT_CONFIG_LEN; i++ )
    {
    num_bytes = pvideo->hdmi_out_iic.fpIicWrite( &(pvideo->hdmi_out_iic), carrier_hdmi_out_config[i][0], carrier_hdmi_out_config[i][1], &(carrier_hdmi_out_config[i][2]), 1 );
    }
    }
    sleep(1);
    // *****************************
    // vfb_dump_registers
    // *****************************
    vfb_dump_registers( &(pvideo->vdma_hdmi) );
    // *****************************
    // vfb_check_errors
    // *****************************
    if ( vfb_check_errors( &(pvideo->vdma_hdmi), 1/*clear errors, if any*/ ) )
    {
    vfb_dump_registers( &(pvideo->vdma_hdmi) );
    }
    xil_printf("\n\r");
    xil_printf( "Done\n\r" );
    xil_printf("\n\r");
    menu_enable  = cmds_space;
    }
    return 1;
}
int hdmi_display(hdmi_display_start *pvideo)
{
    XAxiVdma InstancePtr;
	run_triple_frame_buffer(&InstancePtr, 0, SCREEN_WIDTH_HORIZONTAL, SCREEN_HEIGHT_VERTICAL,(XPAR_PS7_DDR_0_S_AXI_BASEADDR  + 0x100000), 3, 1);
    //run_triple_frame_buffer(&InstancePtr, 0, SCREEN_WIDTH_HORIZONTAL, SCREEN_HEIGHT_VERTICAL,pvideo->uBaseAddr_MEM_HdmiDisplay, 3, 1);
    Xil_DCacheFlush();
    return 0;
}
