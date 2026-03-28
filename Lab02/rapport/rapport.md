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
En se basant sur les commentaires du code, on a un AI qui vaut $AI = 3 FLOPs / 16 octets = 0.1875 FLOPs/octets$ pour 

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
* $$Perf = (FLOPs / Runtime) [FLOP/s] = 150000000 / 0.084135 = 1'782'848'993 [FLOP/s]$$

Grâce à ces valeurs, on constate que l'intensité arithmétique mesurée est inférieur à l'estimation théorique. Le point se situant au-dessus de la courbe théorique, on confirme que la bande passante réelle est meilleure que prévue.

### Proposition amélioration

## Cas 2 : `HPC_COMPUTE`

### Analyse théorique

### Roofline

### Mesures `LIKWID`

### Analyse

### Proposition amélioration

## Cas 3 : `HPC_STRIDE`

### Analyse théorique

### Roofline

### Mesures `LIKWID`

### Analyse

### Proposition amélioration

## Cas 4 : `ROW` vs `COL`

### Analyse théorique

### Roofline

### Mesures `LIKWID`

### Analyse

### Proposition amélioration


