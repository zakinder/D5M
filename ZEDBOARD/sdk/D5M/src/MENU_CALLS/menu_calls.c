#include "menu_calls.h"

#include <stdio.h>
#include <xil_cache.h>
#include <xil_printf.h>

#include "../HDMI_DISPLAY/hdmi_display.h"
#include "../I2C_D5M/i2c_d5m.h"
#include "../TESTPATTERN/pattern.h"
#include "../UART/uartio.h"

hdmi_display_start *pvideo;
hdmi_display_start video;
u32 uartcmd(u32 argA,u32 argB)
{
    printf("\r>>");
    u32 uartquit;
    uartquit = uart_prompt_io();
    if (uartquit == cmds_quit) {return argA;
    } else {return argB;}
}
void menu_calls() {
    int current_status;
    int menu_calls_enable = TRUE;
    unsigned int uart_io;
    int current_state = 0;
    int ret;
    u32 uart_quit;
    u8 address;
    u16 value;
    while (menu_calls_enable == TRUE) {
        switch (current_state) {
        case mainmenu:
            cmds_menu();
            current_state = menu_select;
            break;
        case menu_select:
            uart_io = uart_prompt_io();
            if (uart_io == 0) {
                uart_Red_Text();
                printf("Enter cmd[r],[w],[s]=%x\n", uart_io);
                printf("\r>>");
                uart_Default_Text();
                break;
            } else {
                uart_Default_Text();
                current_state = uart_io;
                break;
            }
            break;
        case clear:
            menu_cls();
            break;
        case cmds_vdfxa:
        	uart_Yellow_Text();
        	printf("address , value\n");
        	uart_Green_Text();
        	printf("\r>>");
        	address = uart_prompt_io();
        	value = uart_prompt_io();
        	img_write_register(address,value);
            //W000_WDATA(enter_data);
            uart_Default_Text();
            camerarUpdate();
            //*****************************
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_vdfxa;break;}
            //*****************************
        case cmds_exposer:
        	uart_Yellow_Text();
        	printf("camera_exposer value\n");
        	uart_Green_Text();
        	printf("\r>>");
        	value = uart_prompt_io();
        	camera_exposer(value);
            current_state =uartcmd(mainmenu,cmds_exposer);
            //*****************************
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_vdfxa;break;}
            //*****************************
            break;
        case cmds_vdfxb:
            //*****************************
        	camera_set_registers();
        	current_state = mainmenu;break;
            uart_Yellow_Text();
            printf("[cmds quite] to exit or [enter] to continue\n");
            uart_Default_Text();
            printf("\r>>");
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_vdfxb;break;}
            //*****************************
        case cmds_vdfxc:
        	ret=hdmi_display_init_bars( &video );
            if (!ret)
            {
            xil_printf( "ERROR : Failed to initialize hdmi_display_init\n\r" );
            current_state = mainmenu;
            break;
            }
            Xil_DCacheFlush();
            printf("Done\n");
            printf("[cmds quite] to exit or [enter] to continue\n");
            printf("\r>>");
            //*****************************
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_vdfxc;break;}
            //*****************************
        case cmds_vdapic:
            //*****************************
            uart_Yellow_Text();
            printf("[cmds quite] to exit or [enter] to continue\n");
            uart_Default_Text();
            printf("\r>>");
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit || cmds_space) {current_state = mainmenu;break;
            } else {current_state = cmds_vdapic;break;}
            //*****************************
        case cmds_vdapib:
            //*****************************
            uart_Yellow_Text();
            printf("[cmds quite] to exit or [enter] to continue\n");
            uart_Default_Text();
            printf("\r>>");
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit || cmds_space) {current_state = mainmenu;break;
            } else {current_state = cmds_vdapic;break;}
            //*****************************
        case cmds_vdtesta:
            //*****************************
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_vdtesta;break;}
            //*****************************
        case cmds_vdtestb:
            //*****************************
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_vdtestb;break;}
            //*****************************
        case cmds_vdtestc:
            pattern();
            //*****************************
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_vdtestc;break;}
            //*****************************
        case cmds_uart:
            uartvalue();
            current_state =uartcmd(mainmenu,cmds_uart);
            break;
            //*****************************
        case cmds_patternv1:
            current_status=pattern();
            Xil_DCacheFlush();
            printf("Done\n");
            printf("[cmds quite] to exit or [enter] to continue\n");
            printf("\r>>");
            //*****************************
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_patternv1;break;}
            //*****************************
        case cmds_patternv2:
            ret = hdmi_display_init( &video );
            if (!ret)
            {
            xil_printf( "ERROR : Failed to initialize hdmi_display_init\n\r" );
            current_state = mainmenu;
            break;
            }
            Xil_DCacheFlush();
            current_state = mainmenu;
            break;
        case cmds_patternv3:
            printf("Done\n");
            printf("[cmds quite] to exit or [enter] to continue\n");
            printf("\r>>");
            //*****************************
            uart_quit = uart_prompt_io();
            if (uart_quit == cmds_quit) {current_state = mainmenu;break;
            } else {current_state = cmds_patternv3;break;}
            //*****************************
        default:
            printf("??\r\n");
            printf("\r\n");
            current_state = mainmenu;
            break;
        }
    }
    printf("sssss\r\n");
    menu_calls_enable = TRUE;
}
