#include <sleep.h>
#include <stdio.h>
#include <xil_cache.h>
#include <xil_printf.h>
#include <xparameters.h>
#include "../SYSTEM_CONFIG_HEADER/system_config_header.h"
#include "xaxivdma.h"
#include "../MENU_CALLS/menu_calls.h"
#include "../UART/uartio.h"
#include "../VIDEO_FRAME_BUFFER/video_frame_buffer.h"
#include "../VIDEO_GENERATOR/video_generator.h"
#include "../VIDEO_RESOLUTION/video_resolution.h"
#include "hdmi_display.h"
Xuint8 carrier_hdmi_outb_config[carrier_hdmi_outb_config_LEN][3] =
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
int hdmi_display_init_bars( hdmi_display_start *pvideo )
{
	int ret;
	u32 uart_quit;
	pvideo->hdmio_resolution = VIDEO_RESOLUTION_1080P;
	pvideo->hdmio_width  = SCREEN_WIDTH_HORIZONTAL;
	pvideo->hdmio_height = SCREEN_HEIGHT_VERTICAL;
	ret = zed_iic_axi_init(&(pvideo->hdmi_out_iic),"ZED HDMI I2C Controller", pvideo->uBaseAddr_IIC_HdmiOut );
	if ( !ret )
	{
	xil_printf( "ERROR : Failed to initialize IIC driver\n\r" );
	return -1;
	}
	xil_printf( "HDMI Output Initialization ...\n\r" );
	{
	Xuint8 num_bytes;
	int i;
	for ( i = 0; i < carrier_hdmi_outb_config_LEN; i++ )
	{
	//xil_printf( "[ZedBoard HDMI] IIC Write - Device = 0x%02X, Address = 0x%02X, Data = 0x%02X\n\r", carrier_hdmi_outb_config[i][0]<<1, carrier_hdmi_outb_config[i][1], carrier_hdmi_outb_config[i][2] );
	num_bytes = pvideo->hdmi_out_iic.fpIicWrite( &(pvideo->hdmi_out_iic), carrier_hdmi_outb_config[i][0], carrier_hdmi_outb_config[i][1], &(carrier_hdmi_outb_config[i][2]), 1 );
	}
	}
	xil_printf( "Clear Frame Buffer\n\r" );
	//hdmi_display_clear( pvideo );
	//xil_printf( "Video DMA (Output Side) Initialization ...\n\r" );
	vfb_common_init(
	pvideo->uDeviceId_VDMA_HdmiDisplay,     // uDeviceId
	&(pvideo->vdma_hdmi)                    // pAxiVdma
	);
	vfb_tx_init(
	&(pvideo->vdma_hdmi),                   // pAxiVdma
	&(pvideo->vdmacfg_hdmi_read),           // pReadCfg
	pvideo->hdmio_resolution,               // uVideoResolution
	pvideo->hdmio_resolution,               // uStorageResolution
	pvideo->uBaseAddr_MEM_HdmiDisplay,      // uMemAddr
	pvideo->uNumFrames_HdmiDisplay          // uNumFrames
	);
	xil_printf( "Video Timing Controller (generator) Initialization ...\n\r" );
	vgen_init( &(pvideo->vtc_hdmio_generator), pvideo->uDeviceId_VTC_HdmioGenerator );
	vgen_config( &(pvideo->vtc_hdmio_generator), pvideo->hdmio_resolution, 0 );
	while (1)
	{
	xil_printf( "\n\rPress ENTER to re-start ...\n\r" );
	uart_quit = uart_prompt_io();
	if (uart_quit == cmds_space) {return 0;}
	Xil_DCacheFlush();
	vfb_tx_stop( &(pvideo->vdma_hdmi) );
	hdmi_display_bars(pvideo);
	vfb_tx_start( &(pvideo->vdma_hdmi) );
	xil_printf( "HDMI Output Re-Initialization ...\n\r" );
	{
	Xuint8 num_bytes;
	int i;
	for ( i = 0; i < carrier_hdmi_outb_config_LEN; i++ )
	{
	num_bytes = pvideo->hdmi_out_iic.fpIicWrite( &(pvideo->hdmi_out_iic), carrier_hdmi_outb_config[i][0], carrier_hdmi_outb_config[i][1], &(carrier_hdmi_outb_config[i][2]), 1 );
	}
	}
	sleep(1);
	vfb_dump_registers( &(pvideo->vdma_hdmi) );
	if ( vfb_check_errors( &(pvideo->vdma_hdmi), 1/*clear errors, if any*/ ) )
	{
	vfb_dump_registers( &(pvideo->vdma_hdmi) );
	}
	xil_printf("\n\r");
	xil_printf( "Done\n\r" );
	xil_printf("\n\r");
	}
	return 1;
}
int hdmi_display_clear_bars( hdmi_display_start *pvideo )
{
	xil_printf( "Video Frame Buffer Initialization ...\n\r" );
	Xuint32 frame, row, col;
	Xuint32 pixel;
	volatile Xuint32 *pStorageMem = (Xuint32 *)pvideo->uBaseAddr_MEM_HdmiDisplay;
	for ( frame = 0; frame < pvideo->uNumFrames_HdmiDisplay; frame++ )
	{
      for ( row = 0; row < pvideo->hdmio_height; row++ )
      {
         for ( col = 0; col < pvideo->hdmio_width; col++ )
         {
            pixel = 0x00000000;
            *pStorageMem++ = pixel;
         }
      }
   }
}
int hdmi_display_bars( hdmi_display_start *pvideo)
{
   Xuint32 frame, row, col;
   Xuint32 cbar, pixel;
   volatile Xuint32 *pStorageMem = (Xuint32 *)pvideo->uBaseAddr_MEM_HdmiDisplay;
   for ( frame = 0; frame < pvideo->uNumFrames_HdmiDisplay; frame++ )
   {
      for ( row = 0; row < pvideo->hdmio_height; row++ )
      {
         for ( col = 0; col < pvideo->hdmio_width; col++ )
         {
            cbar = (col * 30) / pvideo->hdmio_width; // color bar = 0..7
            cbar = (cbar) % 8;
            switch ( cbar )
            {
            case 0: pixel = 0x00000000; break; // Black
            case 1: pixel = 0x00FF0000; break; // Red
            case 2: pixel = 0x0000FF00; break; // Green
            case 3: pixel = 0x000000FF; break; // Blue
            case 4: pixel = 0x0000FFFF; break; // Cyan
            case 5: pixel = 0x00FF00FF; break; // Purple
            case 6: pixel = 0x00FFFF00; break; // Yellow
            case 7: pixel = 0x00FF51FF; break; // White
            case 8: pixel = 0x000005F1; break; // Blue
            case 9: pixel = 0x0001FF5F; break; // Cyan
            case 10: pixel = 0x0008F585; break; // Green
            case 11: pixel = 0x00FF8080; break; // Red
            case 12: pixel = 0x0008FF00; break; // Green
            case 13: pixel = 0x000088FF; break; // Blue
            case 14: pixel = 0x00008FFF; break; // Cyan
            case 15: pixel = 0x00FF02FF; break; // Purple
            case 16: pixel = 0x00F2FF00; break; // Yellow
            case 17: pixel = 0x001F52FF; break; // White
            case 18: pixel = 0x000065FF; break; // Blue
            case 19: pixel = 0x00003F5F; break; // Cyan
            case 20: pixel = 0x00050500; break; // Black
            case 21: pixel = 0x00F56000; break; // Red
            case 22: pixel = 0x00007F00; break; // Green
            case 23: pixel = 0x000077FF; break; // Blue
            case 24: pixel = 0x0000F0FF; break; // Cyan
            case 25: pixel = 0x00F0000F; break; // Purple
            case 26: pixel = 0x00F10F00; break; // Yellow
            case 27: pixel = 0x00F15F1F; break; // White
            case 28: pixel = 0x000005F1; break; // Blue
            case 29: pixel = 0x0000111F; break; // Cyan
            case 30: pixel = 0x00001155; break; // Green
            }
            *pStorageMem++ = pixel;
         }
      }
   }
   // Wait for DMA to synchronize.
   Xil_DCacheFlush();
   return 0;
}