```bash
SHORT HPC lab02

EVENTSET
MBOX0C1 DRAM_READS
FIXC0 INSTR_RETIRED_ANY
PMC1  BR_INST_RETIRED_ALL_BRANCHES

METRICS
Runtime (RDTSC) [s] time
Nombre données lues [B] MBOX0C1*64
Intensité operationnelle totale [Instr/byte] FIXC0/(MBOX0C1)
Nombre d'instructions/s [MInstr/s] 1.0E-06*(FIXC0/time)
Branch rate PMC1/FIXC0

```
