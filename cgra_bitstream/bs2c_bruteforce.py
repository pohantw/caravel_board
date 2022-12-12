import subprocess
from datetime import datetime

time_stamp = datetime.now().strftime("%m/%d/%Y %H:%M:%S")

#tag = "m_m"
#tag = "m_add_m"
#tag = "m_mul_m"
#tag = "m_shr_m"
tag = "m_xor_m"

app = "simple_mem"
docker_container = "pohan-tcgra_v2"
#app_path = "/aha/Halide-to-Hardware/apps/hardware_benchmarks/tests"
app_path = "/aha/tcgra_bs"

cmd = ["docker", "cp", f"{docker_container}:{app_path}/{tag}", "."]
print(" ".join(cmd))
subprocess.run(cmd)

with open(f"{tag}/{app}.bs", "r") as f, open(app+"_a.h", "w") as fa, open(app+"_b.h", "w") as fb:
    line = f.readline()
    fa.write("\n    // write the bitstream\n")
    fb.write("\n    // readback check\n")
    while line:
        pair = line.split()
        addr, data = pair[0], pair[1]
        # reg write
        fa.write(f"    reg_mprj_cfg_addr = 0x{addr};\n")
        fa.write(f"    reg_mprj_cfg_wdata = 0x{data};\n")
        fa.write(f"    reg_mprj_cfg_write = 1;\n")
        # reg read
        fb.write(f"    reg_mprj_cfg_addr = 0x{addr};\n")
        fb.write(f"    reg_mprj_cfg_read = 1;\n")
        fb.write(f"    rtl = reg_mprj_cfg_rdata;\n")
        fb.write(f"    if(rtl != 0x{data}) fail = 1;\n")
        line = f.readline()
    

with open(f"{tag}/{app}_bs_bruteforce.h", "w") as f, open(f"{app}_a.h", "r") as fa, open(f"{app}_b.h", "r") as fb:
    bs_wr = fa.read()
    bs_chk = fb.read()
    f.write(f"    // This is the bitstream file of : {app}\n")
    f.write(f"    // Auto generated at {time_stamp}\n")
    f.write("    uint32_t rtl;\n")
    f.write("    int fail = 0;\n")
    f.write(bs_wr)
    f.write(bs_chk)
    f.write("\n")
    f.write("    reg_mprj_cfg_read = 0;\n")
    f.write("    return fail;\n")

# remove tmp files
subprocess.run(["rm", app+"_a.h"])
subprocess.run(["rm", app+"_b.h"])
