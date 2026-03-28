```bash
reds@2509PC09:~/Desktop/HPC-main/Lab02/script$ likwid-bench -t load -W N:2GB:1
Allocate: Process running on hwthread 0 (Domain N) - Vector length 250000000/2000000000 Offset 0 Alignment 512
--------------------------------------------------------------------------------
LIKWID MICRO BENCHMARK
Test: load
--------------------------------------------------------------------------------
Using 1 work groups
Using 1 threads
--------------------------------------------------------------------------------
Running without Marker API. Activate Marker API with -m on commandline.
--------------------------------------------------------------------------------
Group: 0 Thread 0 Global Thread 0 running on hwthread 0 - Vector length 250000000 Offset 0
--------------------------------------------------------------------------------
Cycles:			4098158614
CPU Clock:		2600000000
Cycle Clock:		2600000000
Time:			1.576215e+00 sec
Iterations:		16
Iterations per thread:	16
Inner loop executions:	31250000
Size (Byte):		2000000000
Size per thread:	2000000000
Number of Flops:	0
MFlops/s:		0.00
Data volume (Byte):	32000000000
MByte/s:		20301.80
Cycles per update:	1.024540
Cycles per cacheline:	8.196317
Loads per update:	1
Stores per update:	0
Load bytes per element:	8
Store bytes per elem.:	0
Instructions:		5500000016
UOPs:			5000000000
--------------------------------------------------------------------------------

```
