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

// flush hack
#define flush_pe_addr 0x00000102 // change me according to design.place
#define flush_true 0x001C7E00
#define flush_false 0x001C0000

// source SRAM (pre-program)
// {8b sram address}{8'd1: feature address for sram}{x addr}{y addr}
#define src_sram_addr_base 0x00010205 // change me according to design.place
#define dst_sram_addr_base 0x00010201 // change me according to design.place

int CGRA_load_bs(int rb_chk);
int CGRA_load_bs_unit();
void led_signal_pass(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
void led_signal_fail(uint32_t error, uint32_t last_rtl);
void led_signal_fail_cfg();
void uart_message(int status);
void flush_set(uint32_t value);

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
    // Enable UART transmission
    reg_mprj_io_6 = 0x7ff;
    reg_uart_clkdiv = 1042;
    reg_uart_enable = 1;

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

    // clear the LCD screen
    print("|"); putchar(0x2d);

    // [CGRA] stall the CGRA for configuration
    reg_mprj_stall = 0xF;

    // [CGRA] load bitstream
    int err_cnt;
    print("Load BS: ");
    err_cnt = CGRA_load_bs(1);
    if (err_cnt > 0) {
        print("Fail, err=");
        print_dec(err_cnt);
        print("\n");
    } else {
        print("Pass\n");
    }

    // de-assert flush
    flush_set(flush_false);

    // pre-program sram
    uint32_t i;
    uint32_t ii;
    print("Load SRAM: ");
    for (i=0; i<512; i++) {
        ii = i>>1; // same address write twice
        ii = ii<<24; // shift the address to [31:24]
        ii = src_sram_addr_base | ii;
        reg_mprj_cfg_addr = ii;
        reg_mprj_cfg_wdata = 0x00000099; // use same data
        reg_mprj_cfg_write = 1;
    }
    print("Done\n");

    // unstall the cgra
    reg_mprj_stall = 0x0;

    // create flush
    flush_set(flush_true);
    flush_set(flush_false);
    print("CGRA loops:add");

    // // wait some time
    // for(i=0; i<3000; i++);

    // // stall cgra
    // reg_mprj_stall = 0xF;
    // print("Fin!\n");

    // // check the result SRAM
    // uint32_t rtl, k;
    // uint32_t error=0;
    // for (i=0;i<256;i++){
    //     ii = i<<24;
    //     ii = dst_sram_addr_base | ii;
    //     reg_mprj_cfg_addr = ii;
    //     reg_mprj_cfg_read = 1;
    //     for(k=0; k<50; k++);
    //     rtl = reg_mprj_cfg_rdata;
    //     reg_mprj_cfg_read = 0;
    //     // if (rtl != (0x00000099)){
    //     if (rtl != (0x00000099+1)){
    //     // if (rtl != (0x00000099*3)){
    //     // if (rtl != (0x00000099>>3)){
    //     // if (rtl != (0x00000099^0x00000013)){
    //         error++;
    //     }
    // }

    // // Use LED blinking pattern to show CGRA program status
    // // if (status==0) {
    // //     led_signal_pass(error, rtl, 0, 0);
    // // }
    // // else {
    // //     // led_signal_fail(error);
    // //     led_signal_fail(error, rtl);
    // // }

    // if (error > 0) {
    //     print("#e=");
    //     print_dec(error);
    //     print(", r=");
    //     print_hex(rtl, 8);
    // } else {
    //     print("-------Pass-------");
    // }

}

void led_signal_pass(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
{
    char *msg[] = {
        " [CGRA] Success!!!   ",
        " [CGRA] Fail...      "
    };
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
    int i, j;
    int N = 5;

	while(1) {
        print_hex(v0, 8);
        print("\n");
        print_hex(v1, 8);
        print("\n");
        print_hex(v2, 8);
        print("\n");
        print_hex(v3, 8);

        for (j = 0; j < 30000; j++);
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

void led_signal_fail(uint32_t error, uint32_t last_rtl)
{
    int i, j;
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
	while(1) {
        print("|"); putchar(0x2d); // clear screen
        print_hex(error, 4);
        print_hex(last_rtl, 4);
        // continuous fast blink
        for (i=0; i<15; i++) {
            reg_gpio_data = 0x0;
            for (j = 0; j < 2000; j++);
            reg_gpio_data = 0x1;
            for (j = 0; j < 2000; j++);
        }
	}
}

void led_signal_fail_cfg()
{
    int i, j;
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
	while(1) {
        print("|"); putchar(0x2d); // clear screen
        print_hex(0x9999, 4);
        // continuous fast blink
        for (i=0; i<15; i++) {
            reg_gpio_data = 0x0;
            for (j = 0; j < 500; j++);
            reg_gpio_data = 0x1;
            for (j = 0; j < 500; j++);
        }
	}
}

void uart_message(int status)
{
    char *msg[] = {
        " [CGRA] Success!!!   ",
        " [CGRA] Fail...      "
    };
    putchar("|"); putchar(0x80);
    putchar("|"); putchar(0x9e);
    putchar("|"); putchar(0xbc);
    print("|"); putchar(0x2d); // clear screen
    print(msg[status]);
}

void flush_set(uint32_t value)
{
    reg_mprj_cfg_addr = flush_pe_addr;
    reg_mprj_cfg_wdata = value;
    reg_mprj_cfg_write = 1;
}

int CGRA_load_bs_unit()
{
    uint32_t rtl;
    reg_mprj_cfg_addr = 0x010A0001;
    reg_mprj_cfg_wdata = 0x00028008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;

    return (rtl != 0x00028008);
}

int CGRA_load_bs(int rb_chk)
{
    // This is the bitstream file of : simple_mem-m_add_m
    // Auto generated at 05/20/2023 01:04:28
    uint32_t rtl;
    int err_cnt = 0;

    // write the bitstream
    reg_mprj_cfg_addr = 0x00090101;
    reg_mprj_cfg_wdata = 0x01028000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020201;
    reg_mprj_cfg_wdata = 0x00000007;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060102;
    reg_mprj_cfg_wdata = 0x0300C000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060103;
    reg_mprj_cfg_wdata = 0x11000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060104;
    reg_mprj_cfg_wdata = 0x11000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01090105;
    reg_mprj_cfg_wdata = 0x00008000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020205;
    reg_mprj_cfg_wdata = 0x0000000B;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020101;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0001;
    reg_mprj_cfg_wdata = 0x01240000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070002;
    reg_mprj_cfg_wdata = 0x00150000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070003;
    reg_mprj_cfg_wdata = 0x00150000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070004;
    reg_mprj_cfg_wdata = 0x00040000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070104;
    reg_mprj_cfg_wdata = 0x40020000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0105;
    reg_mprj_cfg_wdata = 0x00200000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050205;
    reg_mprj_cfg_wdata = 0x00000007;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0201;
    reg_mprj_cfg_wdata = 0x00000018;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0101;
    reg_mprj_cfg_wdata = 0x00048000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0205;
    reg_mprj_cfg_wdata = 0x00003200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0205;
    reg_mprj_cfg_wdata = 0x00010000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00040206;
    reg_mprj_cfg_wdata = 0x00000008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070306;
    reg_mprj_cfg_wdata = 0x0C000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050206;
    reg_mprj_cfg_wdata = 0x00000006;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070206;
    reg_mprj_cfg_wdata = 0x0401C000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070204;
    reg_mprj_cfg_wdata = 0x00008000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070203;
    reg_mprj_cfg_wdata = 0x00008000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070202;
    reg_mprj_cfg_wdata = 0x00008000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01060103;
    reg_mprj_cfg_wdata = 0x00000004;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01060104;
    reg_mprj_cfg_wdata = 0x00000004;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01090201;
    reg_mprj_cfg_wdata = 0x00000200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x4F000205;
    reg_mprj_cfg_wdata = 0x11100000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x39000205;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x2D000205;
    reg_mprj_cfg_wdata = 0x00010101;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x31000205;
    reg_mprj_cfg_wdata = 0xFFFD007D;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x38000205;
    reg_mprj_cfg_wdata = 0x00020002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x30000205;
    reg_mprj_cfg_wdata = 0x000FE300;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x37000205;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x4A000205;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x46000205;
    reg_mprj_cfg_wdata = 0x00011110;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x41000205;
    reg_mprj_cfg_wdata = 0x0000FFFD;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x49000205;
    reg_mprj_cfg_wdata = 0x00010001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x40000205;
    reg_mprj_cfg_wdata = 0x007D00FE;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x48000205;
    reg_mprj_cfg_wdata = 0x00010002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x3F000205;
    reg_mprj_cfg_wdata = 0x00000004;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x47000205;
    reg_mprj_cfg_wdata = 0x01000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000205;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x1C000201;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x2A000201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x23000201;
    reg_mprj_cfg_wdata = 0xFFFD007D;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x07000201;
    reg_mprj_cfg_wdata = 0x11100000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x1B000201;
    reg_mprj_cfg_wdata = 0x00020002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x29000201;
    reg_mprj_cfg_wdata = 0x00020200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x22000201;
    reg_mprj_cfg_wdata = 0x00000FE3;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x1A000201;
    reg_mprj_cfg_wdata = 0x00000009;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x0D000201;
    reg_mprj_cfg_wdata = 0x00010001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x09000201;
    reg_mprj_cfg_wdata = 0x01111000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x15000201;
    reg_mprj_cfg_wdata = 0x0000FFFD;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x14000201;
    reg_mprj_cfg_wdata = 0x007D00FE;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x0C000201;
    reg_mprj_cfg_wdata = 0x00010001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x13000201;
    reg_mprj_cfg_wdata = 0x00000004;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x0B000201;
    reg_mprj_cfg_wdata = 0x00000005;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000201;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x05000101;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000101;
    reg_mprj_cfg_wdata = 0x0000003C;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x04000101;
    reg_mprj_cfg_wdata = 0x00010080;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000101;
    reg_mprj_cfg_wdata = 0x0000F880;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x03000101;
    reg_mprj_cfg_wdata = 0x00010000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000101;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000206;
    reg_mprj_cfg_wdata = 0x00800000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000206;
    reg_mprj_cfg_wdata = 0x48000200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x02000206;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000102;
    reg_mprj_cfg_wdata = 0x001C7E00;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000306;
    reg_mprj_cfg_wdata = 0x48000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x02000306;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;

    // readback check
    reg_mprj_cfg_addr = 0x00090101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x01028000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00020201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000007) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00060102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0300C000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00060103;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x11000000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00060104;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x11000000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01090105;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00008000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00020205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000000B) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00020101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x000A0001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x01240000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070002;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00150000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070003;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00150000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070004;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00040000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070104;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x40020000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x000A0105;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00200000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00050205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000007) err_cnt += 1;
    reg_mprj_cfg_addr = 0x010A0201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000018) err_cnt += 1;
    reg_mprj_cfg_addr = 0x000A0101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00048000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x010A0205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00003200) err_cnt += 1;
    reg_mprj_cfg_addr = 0x000A0205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00040206;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000008) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070306;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0C000000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00050206;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000006) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070206;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0401C000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070204;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00008000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070203;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00008000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00008000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00050201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01060103;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000004) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01060104;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000004) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01090201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000200) err_cnt += 1;
    reg_mprj_cfg_addr = 0x4F000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x11100000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x39000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x2D000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010101) err_cnt += 1;
    reg_mprj_cfg_addr = 0x31000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0xFFFD007D) err_cnt += 1;
    reg_mprj_cfg_addr = 0x38000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00020002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x30000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x000FE300) err_cnt += 1;
    reg_mprj_cfg_addr = 0x37000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x4A000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x46000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00011110) err_cnt += 1;
    reg_mprj_cfg_addr = 0x41000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000FFFD) err_cnt += 1;
    reg_mprj_cfg_addr = 0x49000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x40000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x007D00FE) err_cnt += 1;
    reg_mprj_cfg_addr = 0x48000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x3F000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000004) err_cnt += 1;
    reg_mprj_cfg_addr = 0x47000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x01000000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x51000205;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) err_cnt += 1;
    reg_mprj_cfg_addr = 0x1C000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x2A000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x23000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0xFFFD007D) err_cnt += 1;
    reg_mprj_cfg_addr = 0x07000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x11100000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x1B000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00020002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x29000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00020200) err_cnt += 1;
    reg_mprj_cfg_addr = 0x22000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000FE3) err_cnt += 1;
    reg_mprj_cfg_addr = 0x1A000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000009) err_cnt += 1;
    reg_mprj_cfg_addr = 0x0D000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x09000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x01111000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x15000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000FFFD) err_cnt += 1;
    reg_mprj_cfg_addr = 0x14000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x007D00FE) err_cnt += 1;
    reg_mprj_cfg_addr = 0x0C000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x13000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000004) err_cnt += 1;
    reg_mprj_cfg_addr = 0x0B000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000005) err_cnt += 1;
    reg_mprj_cfg_addr = 0x51000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) err_cnt += 1;
    reg_mprj_cfg_addr = 0x05000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000003C) err_cnt += 1;
    reg_mprj_cfg_addr = 0x04000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010080) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000F880) err_cnt += 1;
    reg_mprj_cfg_addr = 0x03000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x51000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00000206;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00800000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01000206;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x48000200) err_cnt += 1;
    reg_mprj_cfg_addr = 0x02000206;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00000102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x001C7E00) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01000306;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x48000400) err_cnt += 1;
    reg_mprj_cfg_addr = 0x02000306;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;

    reg_mprj_cfg_read = 0;
    return err_cnt;
}
