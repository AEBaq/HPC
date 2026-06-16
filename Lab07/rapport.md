# HPC, Laboratoire n°7 : Parallélisme des tâches

## Partie 1

### Problèmes identifiés

Le code fourni appelle la fonction `read_kmer()` pour chaque position du fichier. À chaque appel, le fichier est ouvert (`fopen()`), le curseur est positionné (`fseek()`), le k-mers est lu (`fgetc()`), puis le fichier est fermé (`fclose()`). Cette approche génère énormement opérations d'ouverture/fermeture, créant une surcharge I/O massive...

De plus, la recherche dans la table est linéaire O(n), et la capacité initiale est zéro, forçant une réallocation immédiate.

### Optimisations apportées

Le fichier est désormais chargé entièrement en mémoire une seule fois via `fread()`, éliminant la surcharge I/O répétitive. La table est initialisée avec une capacité suffisante pour réduire les réallocations. Et finalement, `strncpy()` remplace `strcpy()` pour ajouter un cran de sécurité en plus.

### Parallélisation

La boucle principale est parallélisée avec OpenMP via `#pragma omp for schedule(dynamic, 1000)`. Chaque thread va extraire des k-mers et les insère dans la table partagée, protégée par un verrou `omp_lock_t`.

### Performance

La version optimisée mono-threadée offre un gain de 50-100x grâce à l'élimination des accès disque. La version multi-threadée accélère davantage selon le nombre de coeurs, bien que la contention du verrou lors de l'insertion reste un goulot d'étranglement.


## Partie 2