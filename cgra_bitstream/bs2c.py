import subprocess
from datetime import datetime

time_stamp = datetime.now().strftime("%m/%d/%Y %H:%M:%S")

app = "simple_mem"
docker_container = "pohan-tcgra_v2"
app_path = "/aha/Halide-to-Hardware/apps/hardware_benchmarks/tests"

cmd = ["docker", "cp", f"{docker_container}:{app_path}/{app}/bin/{app}.bs", "."]
print(" ".join(cmd))
subprocess.run(cmd)

size = 0

with open(app+".bs", "r") as f, open(app+"_addr.h", "w") as fa, open(app+"_data.h","w") as fd:
    line = f.readline()
    fa.write("uint32_t bs_addr[] = {\n")
    fd.write("uint32_t bs_data[] = {\n")
    while line:
        size = size + 1
        pair = line.split()
        addr, data = pair[0], pair[1]
        fa.write(f"    0x{addr}")
        fd.write(f"    0x{data}")
        line = f.readline()
        if line:
            fa.write(f", // [{size-1}]\n")
            fd.write(f", // [{size-1}]\n")
        else:
            fa.write(f"  // [{size-1}]\n")
            fd.write(f"  // [{size-1}]\n")
    fa.write("};\n\n")
    fd.write("};\n\n")


prog_example = """
// Program example (write bitstream)
//
//    for (i=0; i<bs_size; i++){
//        reg_mprj_cfg_addr = bs_addr[i];
//        reg_mprj_cfg_wdata = bs_data[i];
//        reg_mprj_cfg_write = 1;
//    }
//
// Program example (read bitstream)
//
//    for (i=0; i<bs_size; i++){
//        reg_mprj_cfg_addr = bs_addr[i];
//        reg_mprj_cfg_read = 1;
//        rtl = reg_mprj_cfg_rdata;
//        if (rtl != bs_data[i]) fail = 1;
//    }

"""


with open(app+"_bs.h", "w") as f, open(app+"_addr.h", "r") as fa, open(app+"_data.h","r") as fd:
    addr = fa.read()
    data = fd.read()
    f.write(f"// This is the bitstream file of : {app}\n")
    f.write(f"// Auto generated at {time_stamp}\n")
    f.write(prog_example)
    f.write(f"int bs_size = {size};\n\n")
    f.write(addr)
    f.write(data)

# remove tmp files
subprocess.run(["rm", app+"_addr.h"])
subprocess.run(["rm", app+"_data.h"])