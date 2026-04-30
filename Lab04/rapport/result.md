```bash
reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 12ms
reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 16ms
reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 12ms
```

```sh
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf record ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 16ms
[ perf record: Woken up 1 times to write data ]
[ perf record: Captured and wrote 0.001 MB perf.data (9 samples) ]
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf report
Samples: 9  of event 'cpu-clock:pppH', Event count (approx.): 2250000
Overhead  Command    Shared Object         Symbol
  22.22%  simd_demo  ld-linux-x86-64.so.2  [.] handle_amd
  22.22%  simd_demo  simd_demo             [.] main
  11.11%  simd_demo  ld-linux-x86-64.so.2  [.] init_cpu_features.constprop.0
  11.11%  simd_demo  ld-linux-x86-64.so.2  [.] update_active.constprop.0
  11.11%  simd_demo  [unknown]             [k] 0xffffffffa2ce9d0f
  11.11%  simd_demo  [unknown]             [k] 0xffffffffa2d76513
  11.11%  simd_demo  [unknown]             [k] 0xffffffffa39de6f5
```

```sh
perf record ./simd_demo 1000 10000
perf annotate
Samples: 8  of event 'cpu-clock:pppH', 4000 Hz, Event count (approx.): 2000000
handle_amd  /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 [Percent: local perio
Percent│       xor     %r8d,%r8d
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:326
       │       sub     $0x7ffffffb,%edx
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:327
       │       cmp     %eax,%edx
       │     ↓ ja      6b
       │       mov     %edi,%esi
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:330
       │       mov     %edx,%eax
       │       cpuid
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:332
100.00 │       cmp     $0xbb,%edi
       │     ↓ jle     70
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:338
       │ 2e:   sub     $0xbd,%esi
       │       cmp     $0x7,%esi
       │     ↓ ja      1c5
       │       lea     msg.0+0x60,%rdi
       │       movslq  (%rdi,%rsi,4),%rax
       │       add     %rdi,%rax
       │       notrack jmp *%rax
```

## Modification de CMakeLists.txt avec `-02`
```sh
reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 0ms
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf record   ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 0ms
[ perf record: Woken up 1 times to write data ]
[ perf record: Captured and wrote 0.001 MB perf.data (6 samples) ]
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf annotate
Samples: 6  of event 'cpu-clock:pppH', 4000 Hz, Event count (approx.): 1500000
handle_amd  /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 [Percent: local perio
Percent│       xor     %r8d,%r8d
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:326
       │       sub     $0x7ffffffb,%edx
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:327
       │       cmp     %eax,%edx
       │     ↓ ja      6b
       │       mov     %edi,%esi
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:330
       │       mov     %edx,%eax
       │       cpuid
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:332
100.00 │       cmp     $0xbb,%edi
       │     ↓ jle     70
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:338
       │ 2e:   sub     $0xbd,%esi
       │       cmp     $0x7,%esi
       │     ↓ ja      1c5
       │       lea     msg.0+0x60,%rdi
       │       movslq  (%rdi,%rsi,4),%rax
       │       add     %rdi,%rax
       │       notrack jmp *%rax
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf report
Samples: 6  of event 'cpu-clock:pppH', Event count (approx.): 1500000
Overhead  Command    Shared Object         Symbol
  33.33%  simd_demo  ld-linux-x86-64.so.2  [.] handle_amd
  33.33%  simd_demo  [unknown]             [k] 0xffffffffa2ce9d0f
  16.67%  simd_demo  ld-linux-x86-64.so.2  [.] init_cpu_features.constprop.0
  16.67%  simd_demo  simd_demo             [.] main
```

### 3e optimisation
```sh
reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 0ms
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf record ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 8ms
[ perf record: Woken up 0 times to write data ]
[ perf record: Captured and wrote 0.001 MB perf.data (4 samples) ]
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf annotate
Samples: 4  of event 'cpu-clock:pppH', 4000 Hz, Event count (approx.): 1000000
main  /home/reds/HPC/HPC/Lab04/build/simd_demo [Percent: local period]
Percent│       movdqa   %xmm13,%xmm1
       │       pandn    %xmm14,%xmm1
       │       por      %xmm13,%xmm1
       │     p->velocity *= -1.f;
       │       movaps   %xmm0,%xmm13
       │       xorps    %xmm4,%xmm13
       │       andps    %xmm1,%xmm13
       │       andnps   %xmm0,%xmm1
       │       movaps   %xmm1,%xmm0
       │       movaps   %xmm2,%xmm1
       │       orps     %xmm13,%xmm0
 50.00 │       unpcklps %xmm0,%xmm1
       │       unpckhps %xmm0,%xmm2
       │       movups   %xmm1,-0x20(%rax)
       │       movups   %xmm2,-0x10(%rax)
       │     for (size_t j = 0; j < numPoints; ++j) {
       │       cmp      %rax,%rdx
       │     ↑ jne      200
       │       cmp      %r9,%r13
       │     ↓ je       44d
       │     p->velocity *= -1.f;
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf report
Samples: 4  of event 'cpu-clock:pppH', Event count (approx.): 1000000
Overhead  Command    Shared Object     Symbol
  50.00%  simd_demo  simd_demo         [.] main
  25.00%  simd_demo  [unknown]         [k] 0xffffffffa2ccd883
  25.00%  simd_demo  [unknown]         [k] 0xffffffffa2ce9d0f
```
### `-O3` with `-march=native`
```sh
reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 0ms
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf record ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 0ms
[ perf record: Woken up 0 times to write data ]
[ perf record: Captured and wrote 0.001 MB perf.data (1 samples) ]
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf annotate
Samples: 4  of event 'cpu-clock:pppH', 4000 Hz, Event count (approx.): 1000000
handle_amd  /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 [Percent: local perio
Percent│
       │     Disassembly of section .text:
       │
       │     0000000000018400 <handle_amd>:
       │     handle_amd():
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:315
       │       push    %rbx
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:320
       │       mov     $0x80000000,%eax
       │       cpuid
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:326
 66.67 │       xor     %edx,%edx
       │       cmp     $0xbe,%edi
       │       setg    %dl
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:328
       │       xor     %r8d,%r8d
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:326
       │       sub     $0x7ffffffb,%edx
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:327
       │       cmp     %eax,%edx
       │     ↓ ja      6b
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf report
Samples: 4  of event 'cpu-clock:pppH', Event count (approx.): 1000000
Overhead  Command    Shared Object         Symbol
  75.00%  simd_demo  ld-linux-x86-64.so.2  [.] handle_amd
  25.00%  simd_demo  ld-linux-x86-64.so.2  [.] init_cpu_features.constprop.0
```

Check of processeur cible
```sh
reds@vmeda2026:~/HPC/HPC/Lab04/build$ gcc -march=native -Q --help=target | grep march
  -march=                     		bdver4
  Known valid arguments for -march= option:
reds@vmeda2026:~/HPC/HPC/Lab04/build$ 
```

## SoA
```sh
reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 4ms
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf record ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 0ms
[ perf record: Woken up 1 times to write data ]
[ perf record: Captured and wrote 0.001 MB perf.data (6 samples) ]
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf report
Samples: 6  of event 'cpu-clock:pppH', Event count (approx.): 1500000
Overhead  Command    Shared Object         Symbol
  50.00%  simd_demo  ld-linux-x86-64.so.2  [.] handle_amd
  33.33%  simd_demo  [unknown]             [k] 0xffffffffa2ce9d0f
  16.67%  simd_demo  ld-linux-x86-64.so.2  [.] init_cpu_features.constprop.0
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf annotate
Samples: 6  of event 'cpu-clock:pppH', 4000 Hz, Event count (approx.): 1500000
handle_amd  /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 [Percent: local perio
Percent│       xor     %r8d,%r8d
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:326
       │       sub     $0x7ffffffb,%edx
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:327
       │       cmp     %eax,%edx
       │     ↓ ja      6b
       │       mov     %edi,%esi
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:330
       │       mov     %edx,%eax
       │       cpuid
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:332
 66.67 │       cmp     $0xbb,%edi
       │     ↓ jle     70
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:338
       │ 2e:   sub     $0xbd,%esi
       │       cmp     $0x7,%esi
       │     ↓ ja      1c5
       │       lea     msg.0+0x60,%rdi
       │       movslq  (%rdi,%rsi,4),%rax
       │       add     %rdi,%rax
       │       notrack jmp *%rax
```
## AoS vs SoA
```sh
reds@vmeda2026:~/HPC/HPC/Lab04/build$ lscpu | grep -i cache
L1d cache:                            64 KiB (2 instances)
L1i cache:                            64 KiB (2 instances)
L2 cache:                             2 MiB (2 instances)
L3 cache:                             128 MiB (2 instances)
# Calcul du BLOCK_SIZE optimal :
# 
# Chaque point contient 2 floats (position + velocity)
# Chaque float = 4 bytes
# Donc 2 floats = 8 bytes par point
#
# On veut que plusieurs points tiennent dans L1d cache :
# L1d = 65536 bytes
# Points que tu peux mettre = 65536 / 8 = 8192 points
#
# Mais il faut laisser de la place pour le code/autres données
# Donc on prend environ 80% : 8192 * 0.8 = 6553.6 ≈ 6400 ou 8192
#
# Pour simplifier et avoir un nombre round : 4096 ou 8192

reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_demo 1000 10000
Starting 10000 update loops of 1000 points... ran for 0ms
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf record ./simd_demo 1000 10000
WARNING: Kernel address maps (/proc/{kallsyms,modules}) are restricted,
check /proc/sys/kernel/kptr_restrict and /proc/sys/kernel/perf_event_paranoid.

Samples in kernel functions may not be resolved if a suitable vmlinux
file is not found in the buildid cache or in the vmlinux path.

Samples in kernel modules won't be resolved at all.

If some relocation was applied (e.g. kexec) symbols may be misresolved
even with a suitable vmlinux or kallsyms file.

Couldn't record kernel reference relocation symbol
Symbol resolution may be skewed if relocation was used (e.g. kexec).
Check /proc/kallsyms permission or run as root.
Starting 10000 update loops of 1000 points... ran for 0ms
[ perf record: Woken up 1 times to write data ]
[ perf record: Captured and wrote 0.001 MB perf.data (6 samples) ]
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf report
Samples: 6  of event 'cpu-clock:pppH', Event count (approx.): 1500000
Overhead  Command    Shared Object         Symbol
  33.33%  simd_demo  ld-linux-x86-64.so.2  [.] handle_amd
  33.33%  simd_demo  ld-linux-x86-64.so.2  [.] init_cpu_features.constprop.0
  16.67%  simd_demo  simd_demo             [.] main
  16.67%  simd_demo  [unknown]             [k] 0xffffffffa2f1f0fa
reds@vmeda2026:~/HPC/HPC/Lab04/build$ perf annotate
Samples: 6  of event 'cpu-clock:pppH', 4000 Hz, Event count (approx.): 1500000
handle_amd  /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 [Percent: local perio
Percent│       xor     %r8d,%r8d
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:326
       │       sub     $0x7ffffffb,%edx
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:327
       │       cmp     %eax,%edx
       │     ↓ ja      6b
       │       mov     %edi,%esi
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:330
       │       mov     %edx,%eax
       │       cpuid
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:332
 50.00 │       cmp     $0xbb,%edi
       │     ↓ jle     70
       │     ./elf/../sysdeps/x86/dl-cacheinfo.h:338
       │ 2e:   sub     $0xbd,%esi
       │       cmp     $0x7,%esi
       │     ↓ ja      1c5
       │       lea     msg.0+0x60,%rdi
       │       movslq  (%rdi,%rsi,4),%rax
       │       add     %rdi,%rax
       │       notrack jmp *%rax
```

## Exercice.c
```sh
reds@vmeda2026:~/HPC/HPC/Lab04/build$ ./simd_exercice 
Processing time (normal) : 112 ms
Processing time (SIMD)   : 44 ms
reds@vmeda2026:~/HPC/HPC/Lab04/build$ 
```