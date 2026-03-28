```bash
reds@2509PC09:~/HPC/likwid$ likwid-topology
--------------------------------------------------------------------------------
CPU name:	Intel(R) Core(TM) i5-14500
CPU type:	Intel AlderLake 14th Gen processor
CPU stepping:	2
********************************************************************************
Hardware Thread Topology
********************************************************************************
Sockets:		1
CPU dies:		1
Cores per socket:	14
Threads per core:	2
--------------------------------------------------------------------------------
HWThread        Thread        Core        Die        Socket        Available
0               0             0           0          0             *                
1               1             0           0          0             *                
2               0             1           0          0             *                
3               1             1           0          0             *                
4               0             2           0          0             *                
5               1             2           0          0             *                
6               0             3           0          0             *                
7               1             3           0          0             *                
8               0             4           0          0             *                
9               1             4           0          0             *                
10              0             5           0          0             *                
11              1             5           0          0             *                
12              0             6           0          0             *                
13              0             7           0          0             *                
14              0             8           0          0             *                
15              0             9           0          0             *                
16              0             10          0          0             *                
17              0             11          0          0             *                
18              0             12          0          0             *                
19              0             13          0          0             *                
--------------------------------------------------------------------------------
Socket 0:		( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 )
--------------------------------------------------------------------------------
********************************************************************************
Cache Topology
********************************************************************************
Level:			1
Size:			48 kB
Cache groups:		( 0 1 ) ( 2 3 ) ( 4 5 ) ( 6 7 ) ( 8 9 ) ( 10 11 ) ( 12 13 ) ( 14 15 ) ( 16 17 ) ( 18 19 )
--------------------------------------------------------------------------------
Level:			2
Size:			1.25 MB
Cache groups:		( 0 1 ) ( 2 3 ) ( 4 5 ) ( 6 7 ) ( 8 9 ) ( 10 11 ) ( 12 13 ) ( 14 15 ) ( 16 17 ) ( 18 19 )
--------------------------------------------------------------------------------
Level:			3
Size:			24 MB
Cache groups:		( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 )
--------------------------------------------------------------------------------
********************************************************************************
NUMA Topology
********************************************************************************
NUMA domains:		1
--------------------------------------------------------------------------------
Domain:			0
Processors:		( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 )
Distances:		10
Free memory:		14178.3 MB
Total memory:		15674.5 MB
--------------------------------------------------------------------------------
```

` likwid-bench -t peakflops -W N:30kB:1` 
```bash
reds@2509PC09:~/Desktop/HPC-main/Lab02/build$ likwid-bench -t peakflops -W N:30kB:1
Allocate: Process running on hwthread 0 (Domain N) - Vector length 3750/30000 Offset 0 Alignment 512
--------------------------------------------------------------------------------
LIKWID MICRO BENCHMARK
Test: peakflops
--------------------------------------------------------------------------------
Using 1 work groups
Using 1 threads
--------------------------------------------------------------------------------
Running without Marker API. Activate Marker API with -m on commandline.
--------------------------------------------------------------------------------
Group: 0 Thread 0 Global Thread 0 running on hwthread 0 - Vector length 3750 Offset 0
--------------------------------------------------------------------------------
Cycles:			4122722646
CPU Clock:		2600000000
Cycle Clock:		2600000000
Time:			1.585663e+00 sec
Iterations:		262144
Iterations per thread:	262144
Inner loop executions:	3750
Size (Byte):		30000
Size per thread:	30000
Number of Flops:	15728640000
MFlops/s:		9919.29
Data volume (Byte):	7864320000
MByte/s:		4959.64
Cycles per update:	4.193850
Cycles per cacheline:	33.550803
Loads per update:	1
Stores per update:	0
Load bytes per element:	8
Store bytes per elem.:	0
Instructions:		19660800032
UOPs:			18677760000
--------------------------------------------------------------------------------
```
`likwid-perfctr -a`
```bash
reds@2509PC09:~/Desktop/HPC-main/Lab02/build$ likwid-perfctr -a
Group name	Description
--------------------------------------------------------------------------------
    FAKE	Test groupe DP simple
ROOFLINE	Roofline (DP) - FLOPs + memory proxy (LLC misses) for Meteor Lake P-core
     HPC	HPC - Lab02
```

## Cas 1 : 
`likwid-perfctr -C 2 -g HPC -m ./roofline_demo hpc_stream 50000000`
```bash
reds@2509PC09:~/Desktop/HPC-main/Lab02/build$ likwid-perfctr -C 2 -g HPC -m ./roofline_demo hpc_stream 50000000
--------------------------------------------------------------------------------
CPU name:	Intel(R) Core(TM) i5-14500
CPU type:	Intel AlderLake 14th Gen processor
CPU clock:	2.60 GHz

[ADD] raw token='FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE:PMC0'
      subtoken[0]='FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE'
      subtoken[1]='PMC0'
[OK]   getIndexAndType counter='PMC0' -> index=3 type=0 key='PMC0'
[CHK]  checkAccess counter='PMC0' index=3 requested_type=0 -> checked_type=0
[OK]   getEvent event='FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE' counter='PMC0' -> limit='PMC' eventId=0xC7 umask=0x4 cfgBits=0x0 options=0
[CHK]  checkCounter counter='PMC0' limit='PMC' -> 1

[ADD] raw token='FP_ARITH_INST_RETIRED_SCALAR_DOUBLE:PMC1'
      subtoken[0]='FP_ARITH_INST_RETIRED_SCALAR_DOUBLE'
      subtoken[1]='PMC1'
[OK]   getIndexAndType counter='PMC1' -> index=4 type=0 key='PMC1'
[CHK]  checkAccess counter='PMC1' index=4 requested_type=0 -> checked_type=0
[OK]   getEvent event='FP_ARITH_INST_RETIRED_SCALAR_DOUBLE' counter='PMC1' -> limit='PMC' eventId=0xC7 umask=0x1 cfgBits=0x0 options=0
[CHK]  checkCounter counter='PMC1' limit='PMC' -> 1

[ADD] raw token='MEM_INST_RETIRED_ALL_LOADS:PMC2'
      subtoken[0]='MEM_INST_RETIRED_ALL_LOADS'
      subtoken[1]='PMC2'
[OK]   getIndexAndType counter='PMC2' -> index=5 type=0 key='PMC2'
[CHK]  checkAccess counter='PMC2' index=5 requested_type=0 -> checked_type=0
[OK]   getEvent event='MEM_INST_RETIRED_ALL_LOADS' counter='PMC2' -> limit='PMC' eventId=0xD0 umask=0x81 cfgBits=0x0 options=0
[CHK]  checkCounter counter='PMC2' limit='PMC' -> 1

[ADD] raw token='MEM_INST_RETIRED_ALL_STORES:PMC3'
      subtoken[0]='MEM_INST_RETIRED_ALL_STORES'
      subtoken[1]='PMC3'
[OK]   getIndexAndType counter='PMC3' -> index=6 type=0 key='PMC3'
[CHK]  checkAccess counter='PMC3' index=6 requested_type=0 -> checked_type=0
[OK]   getEvent event='MEM_INST_RETIRED_ALL_STORES' counter='PMC3' -> limit='PMC' eventId=0xD0 umask=0x82 cfgBits=0x0 options=0
[CHK]  checkCounter counter='PMC3' limit='PMC' -> 1

[SUMMARY]
valid_events=4 fixed_counters=3 isPerfGroup=1
regTypeMask1=0x1
regTypeMask2=0x0
regTypeMask3=0x0
regTypeMask4=0x0
regTypeMask5=0x0
regTypeMask6=0x0
regTypeMask7=0x0
regTypeMask8=0x0
regTypeMask9=0x0
cpuid_info.perf_num_fixed_ctr=3 perfmon_numCounters=20 perfmon_numCoreCounters=15
--------------------------------------------------------------------------------

[ADD] raw token='FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE:PMC0'
      subtoken[0]='FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE'
      subtoken[1]='PMC0'
[OK]   getIndexAndType counter='PMC0' -> index=3 type=0 key='PMC0'
[CHK]  checkAccess counter='PMC0' index=3 requested_type=0 -> checked_type=0
[OK]   getEvent event='FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE' counter='PMC0' -> limit='PMC' eventId=0xC7 umask=0x4 cfgBits=0x0 options=0
[CHK]  checkCounter counter='PMC0' limit='PMC' -> 1

[ADD] raw token='FP_ARITH_INST_RETIRED_SCALAR_DOUBLE:PMC1'
      subtoken[0]='FP_ARITH_INST_RETIRED_SCALAR_DOUBLE'
      subtoken[1]='PMC1'
[OK]   getIndexAndType counter='PMC1' -> index=4 type=0 key='PMC1'
[CHK]  checkAccess counter='PMC1' index=4 requested_type=0 -> checked_type=0
[OK]   getEvent event='FP_ARITH_INST_RETIRED_SCALAR_DOUBLE' counter='PMC1' -> limit='PMC' eventId=0xC7 umask=0x1 cfgBits=0x0 options=0
[CHK]  checkCounter counter='PMC1' limit='PMC' -> 1

[ADD] raw token='MEM_INST_RETIRED_ALL_LOADS:PMC2'
      subtoken[0]='MEM_INST_RETIRED_ALL_LOADS'
      subtoken[1]='PMC2'
[OK]   getIndexAndType counter='PMC2' -> index=5 type=0 key='PMC2'
[CHK]  checkAccess counter='PMC2' index=5 requested_type=0 -> checked_type=0
[OK]   getEvent event='MEM_INST_RETIRED_ALL_LOADS' counter='PMC2' -> limit='PMC' eventId=0xD0 umask=0x81 cfgBits=0x0 options=0
[CHK]  checkCounter counter='PMC2' limit='PMC' -> 1

[ADD] raw token='MEM_INST_RETIRED_ALL_STORES:PMC3'
      subtoken[0]='MEM_INST_RETIRED_ALL_STORES'
      subtoken[1]='PMC3'
[OK]   getIndexAndType counter='PMC3' -> index=6 type=0 key='PMC3'
[CHK]  checkAccess counter='PMC3' index=6 requested_type=0 -> checked_type=0
[OK]   getEvent event='MEM_INST_RETIRED_ALL_STORES' counter='PMC3' -> limit='PMC' eventId=0xD0 umask=0x82 cfgBits=0x0 options=0
[CHK]  checkCounter counter='PMC3' limit='PMC' -> 1

[SUMMARY]
valid_events=4 fixed_counters=3 isPerfGroup=1
regTypeMask1=0x1
regTypeMask2=0x0
regTypeMask3=0x0
regTypeMask4=0x0
regTypeMask5=0x0
regTypeMask6=0x0
regTypeMask7=0x0
regTypeMask8=0x0
regTypeMask9=0x0
cpuid_info.perf_num_fixed_ctr=3 perfmon_numCounters=20 perfmon_numCoreCounters=15
[calc stream]   n=50000000  time=0.082574 s  BW~9.69 GB/s  FLOPs~1.82 GF/s  AI~0.188
[likwid hpc_stream] done (sink=31747445.21345)
WARN: Skipping region rowmajor-0 for evaluation.
WARN: Skipping region hpc_stride-0 for evaluation.
WARN: Skipping region colmajor-0 for evaluation.
WARN: Skipping region hpc_compute-0 for evaluation.
WARN: Regions are skipped because:
      - The region was only registered
      - The region was started but never stopped
      - The region was never started but stopped
--------------------------------------------------------------------------------
Region hpc_stream, Group 1: HPC
+-------------------+------------+
|    Region Info    | HWThread 2 |
+-------------------+------------+
| RDTSC Runtime [s] |   0.084135 |
|     call count    |          1 |
+-------------------+------------+

+------------------------------------------+---------+------------+
|                   Event                  | Counter | HWThread 2 |
+------------------------------------------+---------+------------+
| FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE |   PMC0  |          0 |
|    FP_ARITH_INST_RETIRED_SCALAR_DOUBLE   |   PMC1  |  150000000 |
|        MEM_INST_RETIRED_ALL_LOADS        |   PMC2  |  650002100 |
|        MEM_INST_RETIRED_ALL_STORES       |   PMC3  |  200001000 |
+------------------------------------------+---------+------------+

+------------------------------+------------+
|            Metric            | HWThread 2 |
+------------------------------+------------+
| FLOPs_DP = PMC0 * 2 + PMC1 * |          1 |
+------------------------------+------------+
```
