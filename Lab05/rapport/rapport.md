# HPC, Laboratoire n°5 : Profiling
**Student : Emily Baquerizo**

## Note
J'ai eu des soucis lors de l'installation de Valgrind (et donc cachegrind et callgrind) sur ma machine. Ainsi, je n'ai pas pu utiliser cet outil pour ce laboratoire donc mes analyses sont basées uniquement sur l'outil `perf` et `hotspot`

## 5.6 Analyse

Le profiling de l’exécutable `create_sample` montre que la grande majorité du temps d’exécution (environ 90%) est passée dans la fonction d’affichage `fprintf` et les fonctions internes associées à la gestion du buffer d’écriture.

![Flame graph - Hotspot](image-8.png)
![Hotspot Table](image-9.png)

En dehors de la phase d’écriture, il n’y a pas de fonctions vraiment coûteuses, ce qui semble logique vu que la génération des données est assez simple.

Le point problématique dans nos performances est le temps passé en écriture de fichier, et non la génération des valeurs ou le calcul en lui-même.  

## 5.7 Profiling

### Analyse performance

Le profiling du programme du laboratoire 01 montre que la quasi-totalité du temps d’exécution est passée dans la fonction `read_csv`, principalement lors de `strtod`. Ces résutlats ont été conclu avec `perf` et `hotspot`.

![Flame Graph - Hotspot](image-6.png)
![Hotspot Table](image-7.png)

### Amélioration possible

Une amélioration simple serait de remplacer `strtod` par une fonction de conversion optimisée ou d’utiliser une bibliothèque de parsing CSV plus rapide. On pourrait aussi envisager de paralléliser la lecture du fichier si le format s’y prête.
