# HPC, Laboratoire 3 : Optimisations de Compilation

**Étudiante :** Emily Baquerizo

**Date :** 31 mars 2026

## Choix d'implémentation

Ce laboratoire explore trois optimisations du compilateur : Loop Optimization, Strength Reduction, et Dead Code Elimination.

### Exemple 1 : Loop Invariant Code Motion (Déplacement de Code Hors de la Boucle)

Cet exemple permet de démontrer comment une expression constante à l'intérieur d'une boucle peut être déplacée hors de celle-ci pour réduire les calculs redondants.

### Exemple 2 : Strength Reduction

Cet exemple permet de montrer comment remplacer une opération coûteuse par une opération plus rapide. Par exemple, l'utilisation de décalage de bit est plus efficace que des multiplications/division.

### Exemple 3 : Dead Code Elimination (Élimination du Code Mort)

Cet exemple permet de démontrer l'élimination des variables inutilisées ou jamais lues. Le compilateur va analyser et supprimer les variables inutiles afin d'éviter les calculs inutiles.

## Résultats

| Optimisation | Sans optimisation | Avec -O3 |
|---|---|---|
| Loop Optimization | Calcul répété n fois | Calcul effectué 1 fois |
| Strength Reduction | Division (`/`) à chaque itération | Décalage de bit (`>>`) à chaque itération |
| Dead Code Elimination | Variables mortes conservées | Variables mortes éliminées |

## Analyse

Selon le cours, le compilateur doit satisfaire plusieurs contraintes :
- **Conservation du comportement :** Le compilateur ne peut optimiser que s'il peut prouver que le comportement du programme reste identique.
- **Analyse statique uniquement :** Les optimisations reposent sur une analyse statique, sans information d'exécution.
- **Conservatisme :** En cas de doute, le compilateur refuse l'optimisation pour éviter les effets secondaires.

1. **Loop Invariant Code Motion** : Cette optimisation est couramment effectuée par le compilateur à partir de `-O2`. Elle réduit significativement les calculs redondants dans les boucles.

2. **Strength Reduction** : Les compilateurs modernes remplacent automatiquement les divisions par des puissances de 2 par des décalages de bits, ce qui accélère l'exécution.

3. **Dead Code Elimination** : Le compilateur élimine les variables jamais lues et les assignations mortes, réduisant la taille du binaire et la consommation mémoire.
