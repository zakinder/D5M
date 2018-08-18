#include "utilities.h"
#include <sleep.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uartio.h"
void cameramenu() {
printf(
"|--------------------|\n"
"|set_hdmi            |\n"
"|write_mipicamerareg |\n"
"|set_camera          |\n"
"|set_mipicamera      |\n"
"|read_hdmireg        |\n"
"|read_vdma1test      |\n"
"|read_vdma2test      |\n"
"|set_uart            |\n"
"|read_camerareg      |\n"
"|--------------------|\n");
menu_print_prompt();
}
void cmd_menu() {
printf(
        "|--------------------------|\n"
        "|Hardware test             |\n"
        "|--------------------------|\n"
        "| R |cmdr plread           |\n"
        "| R |cmdr d5mreg           |\n"
        "| R |cmdr cmdr d5mviewmenu |\n"
        "| W |cmdw d5mreg           |\n"
        "| W |cmdw d5mcsize         |\n"
        "| W |cmdw vgadraw          |\n"
        "| S |cmds uart             |\n"
        "| S |cmds hdmitx           |\n"
        "|--------------------------|\n");
menu_print_prompt();
}
void cmds_menu() {
printf(
        "|--------------------------|\n"
        "|Hardware test             |\n"
        "|--------------------------|\n"
        "| S |cmds vdfxa            |\n"
        "| S |cmds vdfxb            |\n"
        "| S |cmds vdfxc            |\n"
        "| S |cmds vdapic           |\n"
        "| S |cmds vdtesta          |\n"
        "| S |cmds vdtestb          |\n"
        "| S |cmds vdtestc          |\n"
        "| S |cmds exposer          |\n"
        "| S |cmds patternv1        |\n"
        "| S |cmds patternv2        |\n"
        "| S |cmds patternv3        |\n"
        "|--------------------------|\n");
menu_print_prompt();
}
void master_menu() {
printf(
        "o-completed , x- in progress\n"
        "|------------------------|\n"
        "|Hardware test           |\n"
        "|------------------------|\n"
        "|000|Chip Version        |\n"
        "|001|Row Start           |\n"
        "|002|Column Start        |\n"
        "|003|Row Size            |\n"
        "|004|Column Size         |\n"
        "|005|Horizontal Blank    |\n"
        "|006|Vertical Blank      |\n"
        "|007|Output Control      |\n"
        "|008|Shutter Width Upper |\n"
        "|009|Shutter Width Lower |\n"
        "|010|Pixel Clock Control |\n"
        "|011|Restart             |\n"
        "|012|Shutter Delay       |\n"
        "|013|Reset               |\n"
        "|016|PLL Control         |\n"
        "|017|PLL Config 1        |\n"
        "|018|PLL Config 2        |\n"
        "|030|Read Mode 1         |\n"
        "|032|Read Mode 2         |\n"
        "|034|Row Address Mode    |\n"
        "|035|Column address mode |\n"
        "|043|Green1 Gain         |\n"
        "|044|Blue Gain           |\n"
        "|045|Red Gain            |\n"
        "|046|Green2 Gain         |\n"
        "|053|Global Gain         |\n"
        "|075|Row black offset    |\n"
        "|160|TestPatternControl  |\n"
        "|161|Test Pattern Green  |\n"
        "|162|Test Pattern Red    |\n"
        "|163|Test Pattern Blue   |\n"
        "|164|TestPatBarWidth     |\n"
        "|255|Chip Version Alt    |\n"
        "|------------------------|\n");
menu_print_prompt();
}
void menu_cls() {
    printf("\033[2J\033[;H");
}