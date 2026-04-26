# HPC, Laboratoire n°4 : SIMD
**Élève : Emily Baquerizo**

## Auto-vectorisation et organisation des données

### Première étape – Prise en main

En exécutant le programme `simd_demo` avec 1000 points et 10000 itérations sans optimisation (`-O0`), j'ai obtenu environ 12-16 ms. C'est ma baseline pour le reste des manipulations. En regardant le code assembleur, on ne voit que des opérations scalaires, pas d'instructions SIMD du tout comme attendue

### Seconde étape – Optimisation par le compilateur

#### Optimisation niveau 2 (`-O2`)

Avec `-O2`, le temps devient pratiquement 0 ms (temps trop petit pour avoir des mesures précises). En regardant le code assembleur, j'ai pu observer des instructions `xmm` (128 bits) et surtout des opérations `unpcklps` et `unpckhps` qui sont des shuffle/rearrangement de données.

On voit que le compilateur a vectorisé la boucle, mais il doit faire du travail supplémentaire pour réorganiser les données. C'est parce que la structure en mémoire est entrelacée : `[position][velocity][position][velocity]...`. Donc quand le compilateur charge 4 floats à la fois, il obtient 2 positions et 2 velocités mélangées, et il doit les séparer

#### Optimisation niveau 3 (`-O3`)

Avec `-O3`, on reste à 0 ms (identique à `-O2`). En regardant le code assembleur, c'est presque la même chose qu'avant. `-O3` active des optimisations plus agressives comme le déroulement de boucles, mais ça ne change pas grand chose dasn notre cas. C'est parce que le vrai problème, c'est l'organisation des données, pas le compilateur...

#### Optimisation pour processeur cible (`-O3 -march=native`)

Quand j'ai ajouté `-march=native`, le compilateur a détecté qu'on utilise un processeur bdver4 (AMD Bulldozer 2013, correspondant à celui de la VM REDS). Selon la donnée, je devais voir des registres `ymm` (256 bits) apparaître, mais je ne les ai pas observé. Le compilateur reste sur les registres `xmm`...

### Optimisation du code

#### Problème du layout Array of Structures

La structure originale est :

```c
typedef struct {
    float position;
    float velocity;
} Point;
```

Ça signifie que si on a 4 points en mémoire, c'est `[pos1][vel1][pos2][vel2][pos3][vel3][pos4][vel4]`. Quand on veut faire `position += velocity * DELTA`, on doit charger ces données entrelacées, puis faire des shuffle pour les séparer. C'est du travail en plus qui ralentit toute l'exéécution

#### Aggraver le cas

Si la structure était :

```c
typedef struct {
    float position;
    float velocity;
    float otherNoisyValues[16];
} Point;
```

Ce serait encore pire parce qu'on chargerait beaucoup plus de données par point. Le cache L1 ne pourrait contenir que très peu de points complets, et on aurait les mêmes problèmes de shuffle mais avec encore plus de données inutiles à charger.

#### Structure of Arrays (SoA)

Avec la structure :

```c
typedef struct {
    float *position;
    float *velocity;
} Point;
```

Là les données sont `[pos1][pos2][pos3]...` + `[vel1][vel2][vel3]...`. Quand on charge, on obtient 4 positions d'un coup, puis 4 velocités d'un coup, sans avoir besoin de shuffle ce qui rend la chose plus efficace.

Avec 1000 points, je n'ai pas vu une grosse différence en timing (environ 4 ms), mais je pense surtout que c'est dû à la charge étant trop petite. Ce qui est plus intéressant à observer est que le code assembleur ne contient plus les opérations de shuffle ralentissante.

La limite de cette approche est que si le nombre de points n'est pas un multiple de 8, il faudrait traiter les derniers points un par un, ce qui ajoute du code et des branches.

#### Approche Hybrid avec alignement cache

Le processeur a un cache L1 de 64 KiB. Si chaque point en SoA prend 8 bytes, on peut en mettre 8192 dans le cache. Mais on ne veut pas utiliser 100% du cache, donc j'ai choisi un `BLOCK_SIZE` de 4096 points. On aligne tout à 64 bytes pour que le CPU soit content.

### Démarche et choix d'optimisation

Une nouvelle structure `SoA_Sample` a été crée avec 4 pointeurs séparés et `init_samples()` a été modifié pour allouer et initialiser les tableaux

Pour `process_samples_simd()`, on load 8 éléments de chaque type, on effectue les calculs vectorisés (énergie et score), et on stocke les résultats. Le clamp doit être fait manuellement pour chaque élément. Les éléments restants sont traités séquentiellement.

La charge a été augmentée à 30 millions de samples pour obtenir des mesures de temps fiables (300k était trop rapide, je n'obtenais que des temps "nuls"). Après comparaison, le résultat obtenu est 2.5x plus rapide par rapport à la version originale.