int CGRA_load_bs(int rb_chk)
{
    // This is the bitstream file of : simple_mem-m_shr_m
    // Auto generated at 05/20/2023 01:13:49
    uint32_t rtl;
    int err_cnt = 0;

    // write the bitstream
    reg_mprj_cfg_addr = 0x01090301;
    reg_mprj_cfg_wdata = 0x00028008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01090201;
    reg_mprj_cfg_wdata = 0x00001010;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00090201;
    reg_mprj_cfg_wdata = 0x08040000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060102;
    reg_mprj_cfg_wdata = 0x00014000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00090101;
    reg_mprj_cfg_wdata = 0x00050000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020101;
    reg_mprj_cfg_wdata = 0x00000006;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020201;
    reg_mprj_cfg_wdata = 0x00000006;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00060302;
    reg_mprj_cfg_wdata = 0x0000C000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00020301;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0001;
    reg_mprj_cfg_wdata = 0x00008000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0101;
    reg_mprj_cfg_wdata = 0x00000081;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0101;
    reg_mprj_cfg_wdata = 0x00018000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050201;
    reg_mprj_cfg_wdata = 0x0000000B;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x010A0201;
    reg_mprj_cfg_wdata = 0x00003000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x000A0201;
    reg_mprj_cfg_wdata = 0x00200000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00040202;
    reg_mprj_cfg_wdata = 0x00000008;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070203;
    reg_mprj_cfg_wdata = 0x0000C000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050202;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00070202;
    reg_mprj_cfg_wdata = 0xC8000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00050101;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x4F000201;
    reg_mprj_cfg_wdata = 0x11100000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x39000201;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x2D000201;
    reg_mprj_cfg_wdata = 0x00010101;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x31000201;
    reg_mprj_cfg_wdata = 0xFFFD007D;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x38000201;
    reg_mprj_cfg_wdata = 0x00020002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x30000201;
    reg_mprj_cfg_wdata = 0x000FE300;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x37000201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x4A000201;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x46000201;
    reg_mprj_cfg_wdata = 0x00011110;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x41000201;
    reg_mprj_cfg_wdata = 0x0000FFFD;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x49000201;
    reg_mprj_cfg_wdata = 0x00010001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x40000201;
    reg_mprj_cfg_wdata = 0x007D00FE;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x48000201;
    reg_mprj_cfg_wdata = 0x00010002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x3F000201;
    reg_mprj_cfg_wdata = 0x00000004;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x47000201;
    reg_mprj_cfg_wdata = 0x01000000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000201;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x1C000101;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x2A000101;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x23000101;
    reg_mprj_cfg_wdata = 0xFFFD007D;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x07000101;
    reg_mprj_cfg_wdata = 0x11100000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x1B000101;
    reg_mprj_cfg_wdata = 0x00020002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x29000101;
    reg_mprj_cfg_wdata = 0x00020200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x22000101;
    reg_mprj_cfg_wdata = 0x00000FE3;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x1A000101;
    reg_mprj_cfg_wdata = 0x00000009;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x0D000101;
    reg_mprj_cfg_wdata = 0x00010001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x09000101;
    reg_mprj_cfg_wdata = 0x01111000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x15000101;
    reg_mprj_cfg_wdata = 0x0000FFFD;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x14000101;
    reg_mprj_cfg_wdata = 0x007D00FE;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x0C000101;
    reg_mprj_cfg_wdata = 0x00010001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x13000101;
    reg_mprj_cfg_wdata = 0x00000004;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x0B000101;
    reg_mprj_cfg_wdata = 0x00000005;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000101;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x05000301;
    reg_mprj_cfg_wdata = 0x00000001;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000301;
    reg_mprj_cfg_wdata = 0x0000003C;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x04000301;
    reg_mprj_cfg_wdata = 0x00010080;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000301;
    reg_mprj_cfg_wdata = 0x0000F880;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x03000301;
    reg_mprj_cfg_wdata = 0x00010000;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x51000301;
    reg_mprj_cfg_wdata = 0x00000400;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_wdata = 0x0080000F;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000202;
    reg_mprj_cfg_wdata = 0x48000200;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x02000202;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x01000203;
    reg_mprj_cfg_wdata = 0x48000C00;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x02000203;
    reg_mprj_cfg_wdata = 0x00000002;
    reg_mprj_cfg_write = 1;
    reg_mprj_cfg_addr = 0x00000302;
    reg_mprj_cfg_wdata = 0x001C7E00;
    reg_mprj_cfg_write = 1;

    // readback check
    reg_mprj_cfg_addr = 0x01090301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00028008) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01090201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00001010) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00090201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x08040000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00060102;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00014000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00090101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00050000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00020101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000006) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00020201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000006) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00060302;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000C000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00020301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x010A0001;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00008000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x010A0101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000081) err_cnt += 1;
    reg_mprj_cfg_addr = 0x000A0101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00018000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00050201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000000B) err_cnt += 1;
    reg_mprj_cfg_addr = 0x010A0201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00003000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x000A0201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00200000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00040202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000008) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070203;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000C000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00050202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00070202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0xC8000000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00050101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x4F000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x11100000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x39000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x2D000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010101) err_cnt += 1;
    reg_mprj_cfg_addr = 0x31000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0xFFFD007D) err_cnt += 1;
    reg_mprj_cfg_addr = 0x38000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00020002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x30000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x000FE300) err_cnt += 1;
    reg_mprj_cfg_addr = 0x37000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x4A000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x46000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00011110) err_cnt += 1;
    reg_mprj_cfg_addr = 0x41000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000FFFD) err_cnt += 1;
    reg_mprj_cfg_addr = 0x49000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x40000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x007D00FE) err_cnt += 1;
    reg_mprj_cfg_addr = 0x48000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x3F000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000004) err_cnt += 1;
    reg_mprj_cfg_addr = 0x47000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x01000000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x51000201;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) err_cnt += 1;
    reg_mprj_cfg_addr = 0x1C000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x2A000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x23000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0xFFFD007D) err_cnt += 1;
    reg_mprj_cfg_addr = 0x07000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x11100000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x1B000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00020002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x29000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00020200) err_cnt += 1;
    reg_mprj_cfg_addr = 0x22000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000FE3) err_cnt += 1;
    reg_mprj_cfg_addr = 0x1A000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000009) err_cnt += 1;
    reg_mprj_cfg_addr = 0x0D000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x09000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x01111000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x15000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000FFFD) err_cnt += 1;
    reg_mprj_cfg_addr = 0x14000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x007D00FE) err_cnt += 1;
    reg_mprj_cfg_addr = 0x0C000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x13000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000004) err_cnt += 1;
    reg_mprj_cfg_addr = 0x0B000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000005) err_cnt += 1;
    reg_mprj_cfg_addr = 0x51000101;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) err_cnt += 1;
    reg_mprj_cfg_addr = 0x05000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000001) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000003C) err_cnt += 1;
    reg_mprj_cfg_addr = 0x04000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010080) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0000F880) err_cnt += 1;
    reg_mprj_cfg_addr = 0x03000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00010000) err_cnt += 1;
    reg_mprj_cfg_addr = 0x51000301;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000400) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00000202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x0080000F) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01000202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x48000200) err_cnt += 1;
    reg_mprj_cfg_addr = 0x02000202;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x01000203;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x48000C00) err_cnt += 1;
    reg_mprj_cfg_addr = 0x02000203;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x00000002) err_cnt += 1;
    reg_mprj_cfg_addr = 0x00000302;
    reg_mprj_cfg_read = 1;
    rtl = reg_mprj_cfg_rdata;
    if(rtl != 0x001C7E00) err_cnt += 1;

    reg_mprj_cfg_read = 0;
    return err_cnt;
}
