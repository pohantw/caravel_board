# Current Measurement Scheduling
The following is a schedule used in `simple_mem`. It wraps a big outer loop over the original app inner loop. The goal is to make the app execute repeatedly such that we can measure steady current. Note that the maximum value of each configuration cannot go beyond 65535 (hardware limit). An example of the scheduling parameters is shown below:
```
cycle_stride:[c0, c1, c2]
extent      :[e0, e1, e2]
c1 = c0 * e0
c2 = c1 * e1
```
Because we want to execute the app as long as possible, we want to make c2 (or the last dimension in c) close 65535. We typically have the first few parameters set for the original app, so you can work out the values for the remaining parameters based on this constraint. For the last extent (in this example, e2), just set it as 65535.

## mem-0 schedule
```json
"sram2tb_0":{
  "cycle_starting_addr":[0],
  "cycle_stride":[2,512,65024],
  "dimensionality":3,
  "extent":[256,127,65535],
  "read_data_starting_addr":[0],
  "read_data_stride":[1,0,0],
  "write_data_starting_addr":[0],
  "write_data_stride":[1,0,0]
},
"tb2out_0":{
  "cycle_starting_addr":[2],
  "cycle_stride":[1,2,512,65024],
  "dimensionality":4,
  "extent":[2,256,127,65535],
  "read_data_starting_addr":[0],
  "read_data_stride":[1,2,0,0]
}
```

## mem-1 schedule
```json
"in2agg_0":{
  "cycle_starting_addr":[2],
  "cycle_stride":[1,2,512,65024],
  "dimensionality":4,
  "extent":[2,256,127,65535],
  "write_data_starting_addr":[0],
  "write_data_stride":[1,2,0,0]
},
"agg2sram_0":{
  "cycle_starting_addr":[4],
  "cycle_stride":[2,512,65024],
  "dimensionality":3,
  "extent":[256,127,65535],
  "read_data_starting_addr":[0],
  "read_data_stride":[1,0,0],
  "write_data_starting_addr":[0],
  "write_data_stride":[1,0,0]
}
```