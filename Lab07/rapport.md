# HPC, Laboratoire n°7 : Parallélisme des tâches

## Partie 1

### Problèmes identifiés

Le code fourni appelle la fonction `read_kmer()` pour chaque position du fichier. À chaque appel, le fichier est ouvert (`fopen()`), le curseur est positionné (`fseek()`), le k-mers est lu (`fgetc()`), puis le fichier est fermé (`fclose()`). Cette approche génère énormement opérations d'ouverture/fermeture, créant une surcharge I/O massive...

De plus, la recherche dans la table est linéaire O(n), et la capacité initiale est zéro, forçant une réallocation immédiate et fréquente au début de l'exécution du programme.

### Mono-threadé

Le fichier est désormais chargé entièrement en mémoire une seule fois via `fread()`, éliminant la surcharge I/O répétitive. La table est initialisée avec une capacité suffisante pour réduire les réallocations. Et finalement, `strncpy()` remplace `strcpy()` pour ajouter un cran de sécurité en plus afin d'évtier les éventuels dépassements de buffer.

### Multi-threadé

La boucle principale est parallélisée avec OpenMP via `#pragma omp for schedule(dynamic, 1000)`. Chaque thread va extraire des k-mers et les insère dans la table partagée, protégée par un verrou `omp_lock_t`.

#### Répartition du travail
L'ordonnancement dynamique par chunk de 1'000 itérations permet de distribuer la charge de travail aux threads disponibles.

#### Traitement des cas limites
Dans la boucle `for` à la ligne 138, nous avons la condition `long i = 0; i <= file_size - k; i++` qui garantit qu'aucun trhead ne tentera de lire au-delà de la fin du buffer ce qui permet de protéger l'exécution des erreurs de fin de fichier.

#### Lecture et zone de chevauchement
Vu qu'on a copié le fichier entier dans `file_buffer` au préalable, les threads peuvent lire simultatnément les séquences chevauchantes sans risque. Cela est possible car, durant l'extraction, ce buffer est uniquement lu et jamais modifié.

#### Synchronisation
Chacun des threads doit insérer ses k-mers dans une structure commune `KmerTable`. Étant ainsi une zone critique, je l'ai sécurisé à l'aide d'un verrou global `omp_lock_t` avant la recherche, l'ajout et la potentielle réallocation dynamique évitant ainsi les problèmes de concurrence.

### Performance
N'ayant pas réussi à installer `perf` sur WSL (un peu attendu en même temps) et doutant pouvoir avoir une bonne mesure du matériel même en réussissant, j'ai décidé de laisser tomber après plusieurs heures de tentatives et partir sur des hypothèses de performance.

#### `main_mono.c`
L'amélioration des performances entre le code original et la version mono-threadée optimisée serait grande grâce à la suppression des accès continus mentionnés plus haut. Il faudrait tout de même confirmer avec les bons outils que le programme initial était bien I/O bound. Mais si tel était le cas, le temps de chargement devient plus négligable face au temps de calcul, ainsi on passerait cette fois-ci sur un programme CPU bound.

#### `main_multi.c`
Bien que l'intention initiale de répartir le travail entre les threads était bonne, on risque de constater un temps d'exécution plus lent qu'en mono-threadé. Cette hypothèse est basée sur le fait que pour chacun des threads, on va devoir attendre son tour afin de pouvoir stocker le résultat dans la table commune. Il aurait peut-être été intéressant de voir pour mettre une table propore à chaque thread.

## Partie 2

### Description des parallélisation effectuée

### Stratégie de parallélisation

### Justification des choix effectués

#### Evalutation de l'intérêt

#### Efficacité de la parallélisation
