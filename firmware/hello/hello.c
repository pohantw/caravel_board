#include "../defs_mpw-two-mfix.h"
#include "../print_io.h"

// CGRA wishbone control register address
#define reg_mprj_cfg_addr  (*(volatile uint32_t*)0x30000000)
#define reg_mprj_cfg_wdata (*(volatile uint32_t*)0x30000004)
#define reg_mprj_cfg_rdata (*(volatile uint32_t*)0x30000008)
#define reg_mprj_cfg_write (*(volatile uint32_t*)0x3000000C)
#define reg_mprj_cfg_read  (*(volatile uint32_t*)0x30000010)
#define reg_mprj_stall     (*(volatile uint32_t*)0x30000014)
#define reg_mprj_message   (*(volatile uint32_t*)0x30000018)

int CGRA_load_bs(int rb_chk);
void led_signal_pass();
void led_signal_fail();
void uart_message(int status);

// --------------------------------------------------------
// Firmware routines
// --------------------------------------------------------
void set_gpio(int pin)
{
    (volatile uint32_t) ((reg_mprj_datal) |= pin);
}

void clear_gpio(int pin)
{
    (volatile uint32_t) ((reg_mprj_datal) &= ~(pin));
}

void main()
{
    // Configure LA probes as outputs from the cpu
    reg_la0_oenb = reg_la0_iena = 0xFFFFFFFF;    // [31:0]
    reg_la1_oenb = reg_la1_iena = 0xFFFFFFFF;    // [63:32]
    reg_la2_oenb = reg_la2_iena = 0xFFFFFFFF;    // [95:64]
    reg_la3_oenb = reg_la3_iena = 0xFFFFFFFF;    // [127:96]

    // configure the IO direction through LA probe
    // 0=output, 1=input
    reg_la0_data = 0x0001FFFF; // 0000_0000_0000_0001_1111_1111_1111_1111
    reg_la1_data = 0x0000000C; // 0000_0000_0000_0000_0000_0000_0000_1100
    reg_la2_data = 0x00000000;
    reg_la3_data = 0x00000000; // note: la_data[96] is the clk/reset select signal

	// Important note!  GPIOs 1 to 4 have a reversed bit for
    // management disable that requires swapping the values
    // for "USER" and "MGMT" for these four pins.  Normally, use
    // of these pins for user project purposes is discouraged, so
    // it should not be a major issue.
    // --------------------------------------------------
    // #define GPIO_MODE_USER_STD_INPUT_NOPULL    0x0403
    // #define GPIO_MODE_USER_STD_INPUT_PULLDOWN  0x0803
    // #define GPIO_MODE_USER_STD_INPUT_PULLUP    0x0c03
    // #define GPIO_MODE_USER_STD_OUTPUT          0x1809
    // #define GPIO_MODE_USER_STD_BIDIRECTIONAL   0x1803
    // #define GPIO_MODE_USER_STD_OUT_MONITORED   0x1803
    // #define GPIO_MODE_USER_STD_ANALOG          0x000b
    // --------------------------------------------------
    // #define GPIO_MODE_MGMT_STD_INPUT_NOPULL    0x0402
    // #define GPIO_MODE_MGMT_STD_INPUT_PULLDOWN  0x0802
    // #define GPIO_MODE_MGMT_STD_INPUT_PULLUP    0x0c02
    // #define GPIO_MODE_MGMT_STD_OUTPUT          0x1808
    // #define GPIO_MODE_MGMT_STD_BIDIRECTIONAL   0x1802
    // #define GPIO_MODE_MGMT_STD_OUT_MONITORED   0x1802
    // #define GPIO_MODE_MGMT_STD_ANALOG          0x000a
    // --------------------------------------------------
    reg_mprj_io_0  = 0x1c03; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[0]
    reg_mprj_io_1  = 0x1e01; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[1]
    reg_mprj_io_2  = 0x0f00; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[2]
    reg_mprj_io_3  = 0x0780; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[3]
    reg_mprj_io_4  = 0x03c0; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[4]
    reg_mprj_io_5  = 0x01e0; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[5]
    reg_mprj_io_6  = 0x00f0; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[6]
    reg_mprj_io_7  = 0x0078; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[7]
    reg_mprj_io_8  = 0x003c; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[8]
    reg_mprj_io_9  = 0x001e; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[9]
    reg_mprj_io_10 = 0x100f; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[10]
    reg_mprj_io_11 = 0x1807; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[11]
    reg_mprj_io_12 = 0x1c03; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[12]
    reg_mprj_io_13 = 0x1e01; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[13]
    reg_mprj_io_14 = 0x0f00; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[14]
    reg_mprj_io_15 = 0x1380; // [slippage fix-14USER] Input - glb2io_16_X00_Y00[15]
    reg_mprj_io_16 = 0x0980; // [slippage fix-14USER] Input - glb2io_1_X00_Y00
    reg_mprj_io_17 = 0x00c0; // [slippage fix-14USER] Output - io2glb_16_X01_Y00[0]
    reg_mprj_io_18 = 0x0000; // [slippage fix-14USER] Output - io2glb_16_X01_Y00[1]
    // reg_mprj_io_0  = 0x0403; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[0]
    // reg_mprj_io_1  = 0x1201; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[1]
    // reg_mprj_io_2  = 0x0900; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[2]
    // reg_mprj_io_3  = 0x0480; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[3]
    // reg_mprj_io_4  = 0x03c0; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[4]
    // reg_mprj_io_5  = 0x01e0; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[5]
    // reg_mprj_io_6  = 0x00f0; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[6]
    // reg_mprj_io_7  = 0x0078; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[7]
    // reg_mprj_io_8  = 0x003c; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[8]
    // reg_mprj_io_9  = 0x001e; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[9]
    // reg_mprj_io_10 = 0x100f; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[10]
    // reg_mprj_io_11 = 0x1807; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[11]
    // reg_mprj_io_12 = 0x1c03; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[12]
    // reg_mprj_io_13 = 0x1e01; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[13]
    // reg_mprj_io_14 = 0x0f00; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[14]
    // reg_mprj_io_15 = 0x1380; // [slippage fix-14MGMT] Input - glb2io_16_X00_Y00[15]
    // reg_mprj_io_16 = 0x0980; // [slippage fix-14MGMT] Input - glb2io_1_X00_Y00
    // reg_mprj_io_17 = 0x00c0; // [slippage fix-14MGMT] Output - io2glb_16_X01_Y00[0]
    // reg_mprj_io_18 = 0x0000; // [slippage fix-14MGMT] Output - io2glb_16_X01_Y00[1]
    reg_mprj_io_19 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[2]
    reg_mprj_io_20 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[3]
    reg_mprj_io_21 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[4]
    reg_mprj_io_22 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[5]
    reg_mprj_io_23 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[6]
    reg_mprj_io_24 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[7]
    reg_mprj_io_25 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[8]
    reg_mprj_io_26 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[9]
    reg_mprj_io_27 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[10]
    reg_mprj_io_28 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[11]
    reg_mprj_io_29 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[12]
    reg_mprj_io_30 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[13]
    reg_mprj_io_31 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[14]
    reg_mprj_io_32 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_16_X01_Y00[15]
    reg_mprj_io_33 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - io2glb_1_X01_Y00
    reg_mprj_io_34 = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - io_clk
    reg_mprj_io_35 = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - io_reset
    reg_mprj_io_36 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - message[0]
    reg_mprj_io_37 = GPIO_MODE_MGMT_STD_OUTPUT; // Output - message[1]
    
    // Enable UART transmission
    // reg_mprj_io_6 = 0x7ff;
    // reg_uart_clkdiv = 1042;
    // reg_uart_enable = 1;

    // perform the transfer to configure all I/Os
    reg_mprj_datal = 0x00000000; // set mprj_io[31:0] through management SoC (IO has to be GPIO_MODE_MGMT*)
    reg_mprj_datah = 0x00000000; // set mprj_io[37:32] through management SoC (IO has to be GPIO_MODE_MGMT*)
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);

    // Enable GPIO (all output, ena = 0)
	reg_gpio_ena = 0x0;
	reg_gpio_pu = 0x0;
	reg_gpio_pd = 0x0;
	reg_gpio_data = 0x1;

    // configure again 
    reg_mprj_io_0  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[0]
    reg_mprj_io_1  = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[1] swap USER to MGMT
    reg_mprj_io_2  = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[2] swap USER to MGMT
    reg_mprj_io_3  = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[3] swap USER to MGMT
    reg_mprj_io_4  = GPIO_MODE_MGMT_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[4] swap USER to MGMT
    reg_mprj_io_5  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[5]
    reg_mprj_io_6  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[6]
    reg_mprj_io_7  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[7]
    reg_mprj_io_8  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[8]
    reg_mprj_io_9  = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[9]
    reg_mprj_io_10 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[10]
    reg_mprj_io_11 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[11]
    reg_mprj_io_12 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[12]
    reg_mprj_io_13 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[13]
    reg_mprj_io_14 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[14]
    reg_mprj_io_15 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_16_X00_Y00[15]
    reg_mprj_io_16 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - glb2io_1_X00_Y00
    reg_mprj_io_17 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[0]
    reg_mprj_io_18 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[1]
    reg_mprj_io_19 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[2]
    reg_mprj_io_20 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[3]
    reg_mprj_io_21 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[4]
    reg_mprj_io_22 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[5]
    reg_mprj_io_23 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[6]
    reg_mprj_io_24 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[7]
    reg_mprj_io_25 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[8]
    reg_mprj_io_26 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[9]
    reg_mprj_io_27 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[10]
    reg_mprj_io_28 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[11]
    reg_mprj_io_29 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[12]
    reg_mprj_io_30 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[13]
    reg_mprj_io_31 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[14]
    reg_mprj_io_32 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_16_X01_Y00[15]
    reg_mprj_io_33 = GPIO_MODE_USER_STD_OUTPUT; // Output - io2glb_1_X01_Y00
    reg_mprj_io_34 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - io_clk
    reg_mprj_io_35 = GPIO_MODE_USER_STD_INPUT_NOPULL; // Input - io_reset
    reg_mprj_io_36 = GPIO_MODE_USER_STD_OUTPUT; // Output - message[0]
    reg_mprj_io_37 = GPIO_MODE_USER_STD_OUTPUT; // Output - message[1]

    // [CGRA] stall the CGRA for configuration
    reg_mprj_stall = 0xF;

    // [CGRA] set message
    reg_mprj_message = 3;

    // [CGRA] load bitstream
    // input
    //     0: no readback check (always success)
    //     1: with readback check
    // output
    //     0: success
    //     1: fail
    int status = 0;
    status = CGRA_load_bs(1);

    // Use LED blinking pattern to show CGRA program status
    if (status==0) {
        led_signal_pass();
    }
    else {
        led_signal_fail();
    }

}

void led_signal_pass()
{
    int i, j;
    int N = 5;

	while(1) {
        // a slow bink
        reg_gpio_data = 0x0;
        for (j = 0; j < 10000; j++);
        reg_gpio_data = 0x1;
        for (j = 0; j < 10000; j++);

        // followed by N fast blink
        for (i=0; i<N; i++) {
            reg_gpio_data = 0x0;
            for (j = 0; j < 1000; j++);
            reg_gpio_data = 0x1;
            for (j = 0; j < 1000; j++);
        }
	}
}

void led_signal_fail()
{
    int j;

	while(1) {
        // continuous fast blink
        reg_gpio_data = 0x0;
        for (j = 0; j < 1000; j++);
        reg_gpio_data = 0x1;
        for (j = 0; j < 1000; j++);
	}
}

void uart_message(int status)
{
    int i, j;
    char *msg[] = {
    " [CGRA] Success!!!   ",
    " [CGRA] Fail...      "
    };
    int n = 2;
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
	while(1) {
	    print("|"); putchar(0x2d); // clear screen
        for (i=0; i < 2; i++) {
            reg_gpio_data = 0x0;
            for (j = 0; j < 3000; j++);
            reg_gpio_data = 0x1;
            for (j = 0; j < 5000; j++);
        }
	    for (i = 0; i < n; i++) {
	        print(msg[status]);
	        for (j = 0; j < 5000; j++);
	        reg_gpio_data = 0x0;
            for (j = 0; j < 5000; j++);
	        reg_gpio_data = 0x1;
	    }
        for (j = 0; j < 10000; j++);
    }
}

int CGRA_load_bs(int rb_chk)
{
    // [CGRA] load config
    reg_mprj_cfg_addr = 17367297;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 590081;
    reg_mprj_cfg_wdata = 163840;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 393474;
    reg_mprj_cfg_wdata = 285212672;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 393475;
    reg_mprj_cfg_wdata = 285212672;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 393476;
    reg_mprj_cfg_wdata = 285212672;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 131333;
    reg_mprj_cfg_wdata = 4;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17367041;
    reg_mprj_cfg_wdata = 32768;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 131329;
    reg_mprj_cfg_wdata = 11;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 655361;
    reg_mprj_cfg_wdata = 19136512;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 458754;
    reg_mprj_cfg_wdata = 3473408;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 458755;
    reg_mprj_cfg_wdata = 1376256;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 458756;
    reg_mprj_cfg_wdata = 52756480;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 655365;
    reg_mprj_cfg_wdata = 35979264;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17432837;
    reg_mprj_cfg_wdata = 67;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 655621;
    reg_mprj_cfg_wdata = 588283904;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17433093;
    reg_mprj_cfg_wdata = 522;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 655877;
    reg_mprj_cfg_wdata = 167772160;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17236486;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459270;
    reg_mprj_cfg_wdata = 1347321856;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17236230;
    reg_mprj_cfg_wdata = 5;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 458758;
    reg_mprj_cfg_wdata = 491520;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459014;
    reg_mprj_cfg_wdata = 4469760;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 327941;
    reg_mprj_cfg_wdata = 9;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262407;
    reg_mprj_cfg_wdata = 8;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459015;
    reg_mprj_cfg_wdata = 201334784;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459271;
    reg_mprj_cfg_wdata = 13897728;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262404;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459010;
    reg_mprj_cfg_wdata = 1073856512;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 655617;
    reg_mprj_cfg_wdata = 65536;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262402;
    reg_mprj_cfg_wdata = 7;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459266;
    reg_mprj_cfg_wdata = 201326592;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 327938;
    reg_mprj_cfg_wdata = 6;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459011;
    reg_mprj_cfg_wdata = 12288;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262659;
    reg_mprj_cfg_wdata = 3;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459268;
    reg_mprj_cfg_wdata = 114688;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 328195;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459267;
    reg_mprj_cfg_wdata = 53248;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 328194;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262660;
    reg_mprj_cfg_wdata = 5;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459524;
    reg_mprj_cfg_wdata = 2351497216;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 328196;
    reg_mprj_cfg_wdata = 6;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17236739;
    reg_mprj_cfg_wdata = 3;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459523;
    reg_mprj_cfg_wdata = 2149122048;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262916;
    reg_mprj_cfg_wdata = 8;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459526;
    reg_mprj_cfg_wdata = 1363968;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 656133;
    reg_mprj_cfg_wdata = 52756992;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 328452;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459522;
    reg_mprj_cfg_wdata = 196608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 328454;
    reg_mprj_cfg_wdata = 3;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262406;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17432581;
    reg_mprj_cfg_wdata = 32833;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 327942;
    reg_mprj_cfg_wdata = 11;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 459012;
    reg_mprj_cfg_wdata = 205520928;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262148;
    reg_mprj_cfg_wdata = 6;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 458759;
    reg_mprj_cfg_wdata = 1490944;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 327684;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262662;
    reg_mprj_cfg_wdata = 11;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 327687;
    reg_mprj_cfg_wdata = 6;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262914;
    reg_mprj_cfg_wdata = 9;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262915;
    reg_mprj_cfg_wdata = 9;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262403;
    reg_mprj_cfg_wdata = 5;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17236227;
    reg_mprj_cfg_wdata = 4;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262146;
    reg_mprj_cfg_wdata = 5;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17235974;
    reg_mprj_cfg_wdata = 4;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17170690;
    reg_mprj_cfg_wdata = 4;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17170691;
    reg_mprj_cfg_wdata = 4;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17170692;
    reg_mprj_cfg_wdata = 4;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17367301;
    reg_mprj_cfg_wdata = 32832;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17236483;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17236487;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 17236742;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 83886337;
    reg_mprj_cfg_wdata = 3;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777473;
    reg_mprj_cfg_wdata = 60;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 67109121;
    reg_mprj_cfg_wdata = 65666;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 257;
    reg_mprj_cfg_wdata = 61568;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 50331905;
    reg_mprj_cfg_wdata = 65536;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1358954753;
    reg_mprj_cfg_wdata = 1024;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1342177541;
    reg_mprj_cfg_wdata = 1114112;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1006633221;
    reg_mprj_cfg_wdata = 131074;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 788529413;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 872415493;
    reg_mprj_cfg_wdata = 4063262;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 771752197;
    reg_mprj_cfg_wdata = 16777216;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 855638277;
    reg_mprj_cfg_wdata = 131072;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 989856005;
    reg_mprj_cfg_wdata = 251;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1325400325;
    reg_mprj_cfg_wdata = 17825792;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 939524357;
    reg_mprj_cfg_wdata = 131074;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 754974981;
    reg_mprj_cfg_wdata = 257;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 822083845;
    reg_mprj_cfg_wdata = 62;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 805306629;
    reg_mprj_cfg_wdata = 123392;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 922747141;
    reg_mprj_cfg_wdata = 125;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1291845893;
    reg_mprj_cfg_wdata = 65537;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1191182597;
    reg_mprj_cfg_wdata = 16777489;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1140850949;
    reg_mprj_cfg_wdata = 62;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1124073733;
    reg_mprj_cfg_wdata = 1966080;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1275068677;
    reg_mprj_cfg_wdata = 65664;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1107296517;
    reg_mprj_cfg_wdata = 196608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1174405381;
    reg_mprj_cfg_wdata = 4368;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1258291461;
    reg_mprj_cfg_wdata = 65536;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1224737029;
    reg_mprj_cfg_wdata = 65537;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1073742085;
    reg_mprj_cfg_wdata = 4063262;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1207959813;
    reg_mprj_cfg_wdata = 65600;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1056964869;
    reg_mprj_cfg_wdata = 3;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 452985093;
    reg_mprj_cfg_wdata = 131074;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 687866117;
    reg_mprj_cfg_wdata = 131584;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 587202821;
    reg_mprj_cfg_wdata = 62;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 117440773;
    reg_mprj_cfg_wdata = 17825792;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 570425605;
    reg_mprj_cfg_wdata = 482;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 436207877;
    reg_mprj_cfg_wdata = 5;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 218104069;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 150995205;
    reg_mprj_cfg_wdata = 1118208;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 335544581;
    reg_mprj_cfg_wdata = 4063262;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 201326853;
    reg_mprj_cfg_wdata = 65537;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 318767365;
    reg_mprj_cfg_wdata = 3;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 184549637;
    reg_mprj_cfg_wdata = 1;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 1358954757;
    reg_mprj_cfg_wdata = 1024;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 258;
    reg_mprj_cfg_wdata = 8388608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777474;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554690;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 515;
    reg_mprj_cfg_wdata = 8388608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777731;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554947;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 516;
    reg_mprj_cfg_wdata = 8388608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777732;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554948;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 772;
    reg_mprj_cfg_wdata = 8388608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777988;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33555204;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 774;
    reg_mprj_cfg_wdata = 8388608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777990;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33555206;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 262;
    reg_mprj_cfg_wdata = 8388608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777478;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554694;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 4;
    reg_mprj_cfg_wdata = 8388608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777220;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554436;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 7;
    reg_mprj_cfg_wdata = 8388608;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777223;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554439;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777730;
    reg_mprj_cfg_wdata = 1207960064;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554946;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 2;
    reg_mprj_cfg_wdata = 12582923;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777218;
    reg_mprj_cfg_wdata = 1207970816;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554434;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 259;
    reg_mprj_cfg_wdata = 8388619;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777475;
    reg_mprj_cfg_wdata = 1207973888;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554691;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 519;
    reg_mprj_cfg_wdata = 8388619;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777735;
    reg_mprj_cfg_wdata = 1207976960;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554951;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 771;
    reg_mprj_cfg_wdata = 12582923;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777987;
    reg_mprj_cfg_wdata = 1207971840;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33555203;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 770;
    reg_mprj_cfg_wdata = 8388619;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777986;
    reg_mprj_cfg_wdata = 1208220672;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33555202;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 518;
    reg_mprj_cfg_wdata = 8388619;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777734;
    reg_mprj_cfg_wdata = 1207977984;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554950;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 260;
    reg_mprj_cfg_wdata = 12582923;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777476;
    reg_mprj_cfg_wdata = 1207972864;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554692;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 6;
    reg_mprj_cfg_wdata = 8388619;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777222;
    reg_mprj_cfg_wdata = 1207979008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554438;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 263;
    reg_mprj_cfg_wdata = 8388619;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 16777479;
    reg_mprj_cfg_wdata = 1207975936;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 33554695;
    reg_mprj_cfg_wdata = 2;
    reg_mprj_cfg_write = 1;

    // [CGRA] Readback test
    if (!rb_chk) {
        return 0;
    }
    else {
        int fail = 0;
        unsigned int rtl;
        reg_mprj_cfg_addr = 17367297;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 590081;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 163840) fail=1;
        reg_mprj_cfg_addr = 393474;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 285212672) fail=1;
        reg_mprj_cfg_addr = 393475;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 285212672) fail=1;
        reg_mprj_cfg_addr = 393476;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 285212672) fail=1;
        reg_mprj_cfg_addr = 131333;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4) fail=1;
        reg_mprj_cfg_addr = 17367041;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 32768) fail=1;
        reg_mprj_cfg_addr = 131329;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 11) fail=1;
        reg_mprj_cfg_addr = 655361;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 19136512) fail=1;
        reg_mprj_cfg_addr = 458754;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 3473408) fail=1;
        reg_mprj_cfg_addr = 458755;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1376256) fail=1;
        reg_mprj_cfg_addr = 458756;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 52756480) fail=1;
        reg_mprj_cfg_addr = 655365;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 35979264) fail=1;
        reg_mprj_cfg_addr = 17432837;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 67) fail=1;
        reg_mprj_cfg_addr = 655621;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 588283904) fail=1;
        reg_mprj_cfg_addr = 17433093;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 522) fail=1;
        reg_mprj_cfg_addr = 655877;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 167772160) fail=1;
        reg_mprj_cfg_addr = 17236486;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 459270;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1347321856) fail=1;
        reg_mprj_cfg_addr = 17236230;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 5) fail=1;
        reg_mprj_cfg_addr = 458758;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 491520) fail=1;
        reg_mprj_cfg_addr = 459014;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4469760) fail=1;
        reg_mprj_cfg_addr = 327941;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 9) fail=1;
        reg_mprj_cfg_addr = 262407;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8) fail=1;
        reg_mprj_cfg_addr = 459015;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 201334784) fail=1;
        reg_mprj_cfg_addr = 459271;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 13897728) fail=1;
        reg_mprj_cfg_addr = 262404;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 459010;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1073856512) fail=1;
        reg_mprj_cfg_addr = 655617;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65536) fail=1;
        reg_mprj_cfg_addr = 262402;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 7) fail=1;
        reg_mprj_cfg_addr = 459266;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 201326592) fail=1;
        reg_mprj_cfg_addr = 327938;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 6) fail=1;
        reg_mprj_cfg_addr = 459011;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 12288) fail=1;
        reg_mprj_cfg_addr = 262659;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 3) fail=1;
        reg_mprj_cfg_addr = 459268;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 114688) fail=1;
        reg_mprj_cfg_addr = 328195;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 459267;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 53248) fail=1;
        reg_mprj_cfg_addr = 328194;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 262660;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 5) fail=1;
        reg_mprj_cfg_addr = 459524;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2351497216) fail=1;
        reg_mprj_cfg_addr = 328196;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 6) fail=1;
        reg_mprj_cfg_addr = 17236739;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 3) fail=1;
        reg_mprj_cfg_addr = 459523;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2149122048) fail=1;
        reg_mprj_cfg_addr = 262916;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8) fail=1;
        reg_mprj_cfg_addr = 459526;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1363968) fail=1;
        reg_mprj_cfg_addr = 656133;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 52756992) fail=1;
        reg_mprj_cfg_addr = 328452;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 459522;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 196608) fail=1;
        reg_mprj_cfg_addr = 328454;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 3) fail=1;
        reg_mprj_cfg_addr = 262406;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 17432581;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 32833) fail=1;
        reg_mprj_cfg_addr = 327942;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 11) fail=1;
        reg_mprj_cfg_addr = 459012;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 205520928) fail=1;
        reg_mprj_cfg_addr = 262148;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 6) fail=1;
        reg_mprj_cfg_addr = 458759;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1490944) fail=1;
        reg_mprj_cfg_addr = 327684;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 262662;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 11) fail=1;
        reg_mprj_cfg_addr = 327687;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 6) fail=1;
        reg_mprj_cfg_addr = 262914;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 9) fail=1;
        reg_mprj_cfg_addr = 262915;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 9) fail=1;
        reg_mprj_cfg_addr = 262403;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 5) fail=1;
        reg_mprj_cfg_addr = 17236227;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4) fail=1;
        reg_mprj_cfg_addr = 262146;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 5) fail=1;
        reg_mprj_cfg_addr = 17235974;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4) fail=1;
        reg_mprj_cfg_addr = 17170690;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4) fail=1;
        reg_mprj_cfg_addr = 17170691;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4) fail=1;
        reg_mprj_cfg_addr = 17170692;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4) fail=1;
        reg_mprj_cfg_addr = 17367301;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 32832) fail=1;
        reg_mprj_cfg_addr = 17236483;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 17236487;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 17236742;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 83886337;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 3) fail=1;
        reg_mprj_cfg_addr = 16777473;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 60) fail=1;
        reg_mprj_cfg_addr = 67109121;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65666) fail=1;
        reg_mprj_cfg_addr = 257;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 61568) fail=1;
        reg_mprj_cfg_addr = 50331905;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65536) fail=1;
        reg_mprj_cfg_addr = 1358954753;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1024) fail=1;
        reg_mprj_cfg_addr = 1342177541;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1114112) fail=1;
        reg_mprj_cfg_addr = 1006633221;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 131074) fail=1;
        reg_mprj_cfg_addr = 788529413;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 872415493;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4063262) fail=1;
        reg_mprj_cfg_addr = 771752197;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 16777216) fail=1;
        reg_mprj_cfg_addr = 855638277;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 131072) fail=1;
        reg_mprj_cfg_addr = 989856005;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 251) fail=1;
        reg_mprj_cfg_addr = 1325400325;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 17825792) fail=1;
        reg_mprj_cfg_addr = 939524357;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 131074) fail=1;
        reg_mprj_cfg_addr = 754974981;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 257) fail=1;
        reg_mprj_cfg_addr = 822083845;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 62) fail=1;
        reg_mprj_cfg_addr = 805306629;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 123392) fail=1;
        reg_mprj_cfg_addr = 922747141;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 125) fail=1;
        reg_mprj_cfg_addr = 1291845893;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65537) fail=1;
        reg_mprj_cfg_addr = 1191182597;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 16777489) fail=1;
        reg_mprj_cfg_addr = 1140850949;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 62) fail=1;
        reg_mprj_cfg_addr = 1124073733;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1966080) fail=1;
        reg_mprj_cfg_addr = 1275068677;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65664) fail=1;
        reg_mprj_cfg_addr = 1107296517;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 196608) fail=1;
        reg_mprj_cfg_addr = 1174405381;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4368) fail=1;
        reg_mprj_cfg_addr = 1258291461;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65536) fail=1;
        reg_mprj_cfg_addr = 1224737029;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65537) fail=1;
        reg_mprj_cfg_addr = 1073742085;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4063262) fail=1;
        reg_mprj_cfg_addr = 1207959813;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65600) fail=1;
        reg_mprj_cfg_addr = 1056964869;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 3) fail=1;
        reg_mprj_cfg_addr = 452985093;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 131074) fail=1;
        reg_mprj_cfg_addr = 687866117;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 131584) fail=1;
        reg_mprj_cfg_addr = 587202821;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 62) fail=1;
        reg_mprj_cfg_addr = 117440773;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 17825792) fail=1;
        reg_mprj_cfg_addr = 570425605;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 482) fail=1;
        reg_mprj_cfg_addr = 436207877;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 5) fail=1;
        reg_mprj_cfg_addr = 218104069;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 150995205;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1118208) fail=1;
        reg_mprj_cfg_addr = 335544581;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 4063262) fail=1;
        reg_mprj_cfg_addr = 201326853;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 65537) fail=1;
        reg_mprj_cfg_addr = 318767365;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 3) fail=1;
        reg_mprj_cfg_addr = 184549637;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1) fail=1;
        reg_mprj_cfg_addr = 1358954757;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1024) fail=1;
        reg_mprj_cfg_addr = 258;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388608) fail=1;
        reg_mprj_cfg_addr = 16777474;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33554690;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 515;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388608) fail=1;
        reg_mprj_cfg_addr = 16777731;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33554947;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 516;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388608) fail=1;
        reg_mprj_cfg_addr = 16777732;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33554948;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 772;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388608) fail=1;
        reg_mprj_cfg_addr = 16777988;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33555204;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 774;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388608) fail=1;
        reg_mprj_cfg_addr = 16777990;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33555206;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 262;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388608) fail=1;
        reg_mprj_cfg_addr = 16777478;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33554694;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 4;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388608) fail=1;
        reg_mprj_cfg_addr = 16777220;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33554436;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 7;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388608) fail=1;
        reg_mprj_cfg_addr = 16777223;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33554439;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 16777730;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207960064) fail=1;
        reg_mprj_cfg_addr = 33554946;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 2;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 12582923) fail=1;
        reg_mprj_cfg_addr = 16777218;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207970816) fail=1;
        reg_mprj_cfg_addr = 33554434;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 259;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388619) fail=1;
        reg_mprj_cfg_addr = 16777475;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207973888) fail=1;
        reg_mprj_cfg_addr = 33554691;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 519;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388619) fail=1;
        reg_mprj_cfg_addr = 16777735;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207976960) fail=1;
        reg_mprj_cfg_addr = 33554951;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 771;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 12582923) fail=1;
        reg_mprj_cfg_addr = 16777987;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207971840) fail=1;
        reg_mprj_cfg_addr = 33555203;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 770;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388619) fail=1;
        reg_mprj_cfg_addr = 16777986;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1208220672) fail=1;
        reg_mprj_cfg_addr = 33555202;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 518;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388619) fail=1;
        reg_mprj_cfg_addr = 16777734;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207977984) fail=1;
        reg_mprj_cfg_addr = 33554950;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 260;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 12582923) fail=1;
        reg_mprj_cfg_addr = 16777476;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207972864) fail=1;
        reg_mprj_cfg_addr = 33554692;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 6;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388619) fail=1;
        reg_mprj_cfg_addr = 16777222;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207979008) fail=1;
        reg_mprj_cfg_addr = 33554438;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;
        reg_mprj_cfg_addr = 263;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 8388619) fail=1;
        reg_mprj_cfg_addr = 16777479;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 1207975936) fail=1;
        reg_mprj_cfg_addr = 33554695;
        reg_mprj_cfg_read = 1;
        rtl = reg_mprj_cfg_rdata;
        if (rtl != 2) fail=1;

        return fail;
    }
}