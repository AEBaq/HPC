# HPC, Laboratoire n°2 : Roofline
**Étudiante :** Emily Baquerizo

**Date :** 27 mars 2026

>NOTE : Pour ce laboratoire, la mise en page du rapport a été modifié par rapport à celle demandé dans les consignes de laboratoire afin de faciliter l'étude de chacun des cas.

## Caractéristique Machine
Suite à divers tentatives de mise en place de likwid et heures passées à tester divers approche, j'ai finalement dû de venir travailler sur les ordinateurs de l'école possédant une architecture Intel Core i5-14500 (Alder Lake).

Voici les caractéristiques matérielles trouvées grâce à la commande `likwid-topology`: 
* CPU : Intel Core i5-14500 (14 cores, 20 threads)
* Cache L1 : 48 kB par core
* Cache L2 : 1.25 MB par core
* Cache L3 : 24 MB partagé
* Mode d'exécution : mono-thread
* Précision : double precision (float64)

## Plafond roofline

Afin de trouver le pic de calcul (`maxperf`), la commande `likwid-bench -t peakflops -W N:30kB:1` a été utilisée. Les paramètres choisis limitent la taille des données à 30 [kB] pour assurer que tout reste dans le cache L1. On trouve ainsi que `maxperf` vaut **9919.29 [MFLOP/S]**

Afin de trouver la mesure de bande passante (`maxband`), la commande `likwid-bench -t load -W N:2GB:1` a été utilisée. Cette commande utilise un volume de données de 2 GB pour forcer les accès à la mémoire principale, On trouve ainsi que `maxband` vuat **20620.22 [MB/s]**.

Ainsi grâce à ces deux valeurs, on peut calculer l'AI : 
$$AI = maxperf / maxband = 99919.29 / 20620.22 = 0.481 [FLOP/Byte]$$

Nous indiquant que pour : 
* AI < 0.481 : performance limitée par la bande passante mémoire
* AI > 0.481 : performance limitée par la capcité de calcul

Pour les futures analyses, les formules suivantes vont être utilisées pour trouver les valeures intéressantes : 
* $$FLOPs = (PMC0 * 2) + (PMC1 * 1)$$
* $$DRAM\_Bytes = (PMC2 + PMC3) * 8$$
* $$AI = FLOPs / DRAM\_Bytes$$
* $$Perf = (FLOPs / Runtime) [FLOP/s]$$

## Cas 1 : `HPC_STREAM`

### Analyse théorique
En se basant sur les commentaires du code, on a un AI qui vaut $AI = 3 FLOPs / 16 octets = 0.1875 FLOPs/octets$

### Mesures `LIKWID`
Voici la commande utilisée : `likwid-perfctr -C 2 -g HPC -m ./roofline_demo hpc_stream 50000000`

Extrait du résultat : 
```bash
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
```

### Analyse

A partir des mesures `Likwid`, nous pouvons calculer les mesures suivantes : 
* $$FLOPs = (PMC0 * 2) + (PMC1 * 1) = (0 * 2) + (150000000 * 1) = 150000000 [FLOP]$$
* $$DRAM\_Bytes = (PMC2 + PMC3) * 8 = (650002100 + 200001000) * 8 = 6800 [MB]$$
* $$AI = FLOPs / DRAM\_Bytes = 150000000 / 6800 [MB] = 0.0221 [FLOP/Byte]$$
* $$Perf = (FLOPs / Runtime) [FLOP/s] = 150000000 / 0.084135 = 1'782'848'993 [FLOP/s] = 1'782 [MFLOP/s]$$

Grâce à ces valeurs, on constate que l'intensité arithmétique mesurée est inférieur à l'estimation théorique. Le point se situant au-dessus de la courbe théorique, on confirme que la bande passante réelle est meilleure que prévue.

### Proposition amélioration

## Cas 2 : `HPC_COMPUTE`

### Analyse théorique

En se basant sur le code, on effectue 200 itérations. Ainsi, en remplaçant cette valeur dans la formule donnée, on obtient un AI qui vaut $AI = 3 + (6 * 200 + 1) FLOPs / 16 octets = 1024 / 16 = 64 [FLOPs/octets]$

### Mesures `LIKWID`
Voici la commande utilisée : `likwid-perfctr -C 2 -g HPC -m ./roofline_demo hpc_compute 20000000 200`

Extrait du résultat :
```bash
Region hpc_compute, Group 1: HPC
+-------------------+------------+
|    Region Info    | HWThread 2 |
+-------------------+------------+
| RDTSC Runtime [s] |  28.545460 |
|     call count    |          1 |
+-------------------+------------+

+------------------------------------------+---------+-------------+
|                   Event                  | Counter |  HWThread 2 |
+------------------------------------------+---------+-------------+
| FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE |   PMC0  |           0 |
|    FP_ARITH_INST_RETIRED_SCALAR_DOUBLE   |   PMC1  | 24080000000 |
|        MEM_INST_RETIRED_ALL_LOADS        |   PMC2  | 48500010000 |
|        MEM_INST_RETIRED_ALL_STORES       |   PMC3  | 16280000000 |
+------------------------------------------+---------+-------------+
``` 
### Analyse
A partir des mesures `Likwid`, nous pouvons calculer les mesures suivantes :
* $$FLOPs = (PMC0 * 2) + (PMC1 * 1) = (0 * 2) + (24080000000 * 1) = 24080000000 [FLOP]$$
* $$DRAM\_Bytes = (PMC2 + PMC3) * 8 = (48500010000 + 16280000000) * 8 = 518240000 [Bytes] = 518240 [MB]$$
* $$AI = FLOPs / DRAM\_Bytes = 24080000000 / 518240000000 = 0.0465 [FLOP/Byte]$$
* $$Perf = (FLOPs / Runtime) [FLOP/s] = 24080000000 / 28.545460 = 843'350'000 [FLOP/s] = 843 [MFLOP/s]$$


L'intensité arithmétique mesurée est très inférieure à l'estimation théorique. Le calcul réel effectue beaucoup plus d'accès mémoire que prévu, restant limité par la bande passante. Le point se situe également au-dessus de la courbe théorique, suggérant que la bande passante réelle est meilleure que le modèle idéal. 
### Proposition amélioration

## Cas 3 : `HPC_STRIDE`

### Analyse théorique
En se basant sur les commentaires du code, on a un AI qui vaut $AI = 3 FLOPs / 16 octets = 0.1875 FLOPs/octet$. On constate que le nombre de stride n'impacte pas l'AI.

### Mesures `LIKWID`

Trois variantes ont été testées pour ce cas, avec stride = 2, stride = 16 et stride = 64

#### 2 Strides
Commande utilisée : `likwid-perfctr -C 2 -g HPC -m ./roofline_demo hpc_stride 50000000 2`

Extrait du résultat :
```bash
Region hpc_stride, Group 1: HPC
+-------------------+------------+
|    Region Info    | HWThread 2 |
+-------------------+------------+
| RDTSC Runtime [s] |   0.114608 |
|     call count    |          1 |
+-------------------+------------+

+------------------------------------------+---------+------------+
|                   Event                  | Counter | HWThread 2 |
+------------------------------------------+---------+------------+
| FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE |   PMC0  |          0 |
|    FP_ARITH_INST_RETIRED_SCALAR_DOUBLE   |   PMC1  |  150000000 |
|        MEM_INST_RETIRED_ALL_LOADS        |   PMC2  |  800002100 |
|        MEM_INST_RETIRED_ALL_STORES       |   PMC3  |  250001000 |
+------------------------------------------+---------+------------+
```

#### 16 Stride
Commande utilisée : `likwid-perfctr -C 2 -g HPC -m ./roofline_demo hpc_stride 50000000 16`

Extrait du résultat :
```bash
Region hpc_stride, Group 1: HPC
+-------------------+------------+
|    Region Info    | HWThread 2 |
+-------------------+------------+
| RDTSC Runtime [s] |   0.594000 |
|     call count    |          1 |
+-------------------+------------+

+------------------------------------------+---------+------------+
|                   Event                  | Counter | HWThread 2 |
+------------------------------------------+---------+------------+
| FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE |   PMC0  |          0 |
|    FP_ARITH_INST_RETIRED_SCALAR_DOUBLE   |   PMC1  |  150000000 |
|        MEM_INST_RETIRED_ALL_LOADS        |   PMC2  |  800002200 |
|        MEM_INST_RETIRED_ALL_STORES       |   PMC3  |  250001000 |
+------------------------------------------+---------+------------+
```

#### 64 Strides
Commande utilisée : `likwid-perfctr -C 2 -g HPC -m ./roofline_demo hpc_stride 50000000 64`

Extrait du résultat :
```bash
Region hpc_stride, Group 1: HPC
+-------------------+------------+
|    Region Info    | HWThread 2 |
+-------------------+------------+
| RDTSC Runtime [s] |   0.678691 |
|     call count    |          1 |
+-------------------+------------+

+------------------------------------------+---------+------------+
|                   Event                  | Counter | HWThread 2 |
+------------------------------------------+---------+------------+
| FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE |   PMC0  |          0 |
|    FP_ARITH_INST_RETIRED_SCALAR_DOUBLE   |   PMC1  |  150000000 |
|        MEM_INST_RETIRED_ALL_LOADS        |   PMC2  |  800002200 |
|        MEM_INST_RETIRED_ALL_STORES       |   PMC3  |  250001000 |
+------------------------------------------+---------+------------+
```
### Analyse
Pour tous les cas de stride, les calculs suivants montrent une AI constante :
* $$FLOPs = (PMC0 * 2) + (PMC1 * 1) = 150000000 [FLOP]$$
* $$DRAM\_Bytes = (PMC2 + PMC3) * 8 = 8400000000 [Bytes] \approx 8400 [MB]$$
* $$AI = 150000000 / 8400000000 = 0.0179 [FLOP/Byte]$$

En revanche, les performances mesurées varient considérablement selon le stride :
* Stride 2 : $Perf = 150000000 / 0.114608 = 1'308'000'000 [FLOP/s] = 1308 [MFLOP/s]$
* Stride 16 : $Perf = 150000000 / 0.594 = 252'000'000 [FLOP/s] = 252 [MFLOP/s]$
* Stride 64 : $Perf = 150000000 / 0.678691 = 221'000'000 [FLOP/s] = 221 [MFLOP/s]$

On constate ainsi que même avec une intensité arithmétique identique, les performances chutent quand le stride augmente, on peut donc en conclure que l’intensité arithmétique ne suffit pas à prédire les performances réelles
### Proposition amélioration

## Cas 4 : `ROW` vs `COL`

### Analyse théorique
En se basant sur les commentaires du code des deux cas, on a un AI qui vaut $AI = (N * N) FLOPs / (N * N)*8 octets = 1/8 = 0.125 FLOPs/octet$

En revanche, en row-major, les accès au sein d'une boucle interne sont séquentiels en mémoire, exploitant bien le cache et le prefetching. En col-major, les accès sautent entre les lignes de la matrice stockée en mémoire contiguë, déteriorant l'utilisation du prefetching. On aura donc une différence dans les performances malgré un AI similaire dû au accès mémoire supplémentaire demandé pour col-major.

### Mesures `LIKWID`

#### Row-major :
Commande utilisée : `likwid-perfctr -C 2 -g HPC -m ./roofline_demo rowmajor 4096`

Extrait du résultat :
```bash
Region rowmajor, Group 1: HPC
+-------------------+------------+
|    Region Info    | HWThread 2 |
+-------------------+------------+
| RDTSC Runtime [s] |   0.027359 |
|     call count    |          1 |
+-------------------+------------+

+------------------------------------------+---------+------------+
|                   Event                  | Counter | HWThread 2 |
+------------------------------------------+---------+------------+
| FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE |   PMC0  |          0 |
|    FP_ARITH_INST_RETIRED_SCALAR_DOUBLE   |   PMC1  |   16777220 |
|        MEM_INST_RETIRED_ALL_LOADS        |   PMC2  |  134248400 |
|        MEM_INST_RETIRED_ALL_STORES       |   PMC3  |   33567710 |
+------------------------------------------+---------+------------+
```

#### Col-major
Commande utilisée : `likwid-perfctr -C 2 -g HPC -m ./roofline_demo colmajor 4096`

Extrait du résultat :
```bash
Region colmajor, Group 1: HPC
+-------------------+------------+
|    Region Info    | HWThread 2 |
+-------------------+------------+
| RDTSC Runtime [s] |   0.179456 |
|     call count    |          1 |
+-------------------+------------+

+------------------------------------------+---------+------------+
|                   Event                  | Counter | HWThread 2 |
+------------------------------------------+---------+------------+
| FP_ARITH_INST_RETIRED_128B_PACKED_DOUBLE |   PMC0  |          0 |
|    FP_ARITH_INST_RETIRED_SCALAR_DOUBLE   |   PMC1  |   16777220 |
|        MEM_INST_RETIRED_ALL_LOADS        |   PMC2  |  151017500 |
|        MEM_INST_RETIRED_ALL_STORES       |   PMC3  |   33563620 |
+------------------------------------------+---------+------------+
```

### Analyse

Pour le cas row-major :
* $$FLOPs = (PMC0 * 2) + (PMC1 * 1) = (0 * 2) + (16777220 * 1) = 16777220 [FLOP]$$
* $$DRAM\_Bytes = (PMC2 + PMC3) * 8 = (134248400 + 33567710) * 8 = 1341280 [Bytes] = 1341 [MB]$$
* $$AI = FLOPs / DRAM\_Bytes = 16777220 / 1341280000 = 12.51 [FLOP/Byte]$$
* $$Perf = (FLOPs / Runtime) [FLOP/s] = 16777220 / 0.027359 = 613'297'000 [FLOP/s] = 613 [MFLOP/s]$$

Pour le cas col-major :
* $$FLOPs = (PMC0 * 2) + (PMC1 * 1) = 16777220 [FLOP]$$
* $$DRAM\_Bytes = (PMC2 + PMC3) * 8 = (151017500 + 33563620) * 8 = 1481384 [Bytes] = 1481 [MB]$$
* $$AI = FLOPs / DRAM\_Bytes = 16777220 / 1481384000 = 11.33 [FLOP/Byte]$$
* $$Perf = (FLOPs / Runtime) [FLOP/s] = 16777220 / 0.179456 = 93'500'000 [FLOP/s] = 93.5 [MFLOP/s]$$

Les AIs des deux cas sont presque similaire, allant donc dans la direction de l'analyse théorique. Cependant, la performance varie drastiquement comme attendu... en terme de perdormance, row-major atteint 613 [MFLOP/s] tandis que col-major ne fait que 93.5 [MFLOP/s]. Le point col-major se situe bien en-dessous de la courbe théorique, indiquant que le système n'atteint pas la bande passante maximale en raison de la mauvaise localité mémoire et des nombreux cache misses. Cet exemple montre bien que la façon dont sont stocké les données en mémoire et la façon dont nous utilisons les boucles sont tout aussi important pour la performance que l'intensité arithmétique.

### Proposition amélioration
