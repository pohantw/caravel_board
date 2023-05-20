import subprocess
from datetime import datetime

time_stamp = datetime.now().strftime("%m/%d/%Y %H:%M:%S")

# tag = "m_m"
# tag = "m_add_m"
# tag = "m_mul_m"
# tag = "m_shr_m"
tag = "m_xor_m"

app = "simple_mem"
docker_container = "pohan-tcgra"
app_path = "/aha/Halide-to-Hardware/apps/hardware_benchmarks/tests"

# create the bitstream folder
subprocess.run(f"rm -rf {tag}", shell=True)
subprocess.run(f"mkdir {tag}", shell=True)

# download the bitstream
cmd = ["docker", "cp", f"{docker_container}:{app_path}/{app}/bin/{app}.bs", f"./{tag}/{tag}.bs"]
print(" ".join(cmd))
subprocess.run(cmd)

# download the generator
cmd = ["docker", "cp", f"{docker_container}:{app_path}/{app}/{app}_generator.cpp", f"./{tag}/{tag}_{app}_generator.cpp"]
print(" ".join(cmd))
subprocess.run(cmd)

# download the collaterals
files = ["design_top.json", "design.packed", "design.place", "design.route"]
for cfile in files:
    cmd = ["docker", "cp", f"{docker_container}:{app_path}/{app}/bin/{cfile}", f"./{tag}/{tag}_{cfile}"]
    print(" ".join(cmd))
    subprocess.run(cmd)

with open(f"./{tag}/{tag}.bs", "r") as f, open(f"{tag}_a.h", "w") as fa, open(f"{tag}_b.h", "w") as fb:
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
        fb.write(f"    if(rtl != 0x{data}) err_cnt += 1;\n")
        line = f.readline()

with open(f"./{tag}/{tag}_bs.cpp", "w") as f, open(f"{tag}_a.h", "r") as fa, open(f"{tag}_b.h", "r") as fb:
    bs_wr = fa.read()
    bs_chk = fb.read()
    f.write("int CGRA_load_bs(int rb_chk)\n")
    f.write("{\n")
    f.write(f"    // This is the bitstream file of : {app}-{tag}\n")
    f.write(f"    // Auto generated at {time_stamp}\n")
    f.write("    uint32_t rtl;\n")
    f.write("    int err_cnt = 0;\n")
    f.write(bs_wr)
    f.write(bs_chk)
    f.write("\n")
    f.write("    reg_mprj_cfg_read = 0;\n")
    f.write("    return err_cnt;\n")
    f.write("}\n")

# remove tmp files
subprocess.run(["rm", f"{tag}_a.h"])
subprocess.run(["rm", f"{tag}_b.h"])
