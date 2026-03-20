# HPC, Laboratoire n°1 : Signaux ECG
**Étudiante :** Nox  
**Date :** 2026-03-19  
**Fichier utilisé pour les mesures :** `80bpm0.csv`

## Choix d'implémentation
N'ayant pas de réelle connaissance dans le traitement de signaux ECG, j'ai préféré suivre les références fournies dans l’énoncé, en m’inspirant du principe général de l’algorithme de Pan–Tompkins (sans chercher à en faire une reproduction exacte).

- **Prétraitement** : application d’un gain et suppression de la composante continue
- **Filtrage passe-bande approximatif** : réalisé avec des moyennes glissantes (passe-bas puis passe-haut par soustraction d’une moyenne glissante). J’ai choisi cette solution car elle est l'une des plus simple à mettre en place.
- **Dérivée puis mise au carré** : pour mettre en évidence les variations rapides liées au complexe QRS.
- **Intégration par fenêtre glissante (MWI)** : pour lisser l’énergie du signal transformé.
- **Détection des pics R** : recherche de maxima locaux au-dessus d’un **seuil** (calculé automatiquement) et application d’une **période réfractaire** pour éviter de détecter plusieurs pics trop proches.

Les **intervalles RR** sont ensuite calculés à partir des indices des pics R et convertis en secondes avec la fréquence d’échantillonnage.

## Résultats
Cette section reprend uniquement les manipulations que j’ai réellement effectuées après compilation, telles qu’elles sont notées dans `simple_measure.md`.

### Exécution (génération de `out.json`)
Commande :
```bash
./ecg_dealination ../80bpm0.csv out.json
```

Sortie observée :
- Le CSV est chargé avec **12 leads** et **7500 échantillons**.
- Seuil automatique utilisé :
  - `threshold auto = 0.610762` (avec `max=1.745033`)
- Détection :
  - **22 pics R détectés**
  - **21 intervalles RR calculés**
- Le programme termine correctement et indique que les résultats ont été sauvegardés dans `out.json`.

### Mesure simple des performances (`/usr/bin/time -v`)
Commande :
```bash
/usr/bin/time -v ./ecg_dealination ../80bpm0.csv /tmp/out.json
```

Valeurs relevées :
- **User time (seconds)** : `0.05`
- **System time (seconds)** : `0.00`
- **Elapsed time** : `0:00.05`
- **Percent CPU** : `96%`
- **Maximum resident set size (kbytes)** : `3636`

### Benchmark (`hyperfine`)
Commande :
```bash
hyperfine --warmup 10 './ecg_dealination ../80bpm0.csv /tmp/out.json'
```

Valeurs relevées :
- **Time (mean ± σ)** : `53.6 ms ± 0.9 ms`
- **Range (min … max)** : `52.5 ms … 56.9 ms`
- **Runs** : `52`

### Impact de la taille des données (N échantillons)
Commande (via le script `hyperfine_size.sh`) : tests avec N = 1000 / 2000 / 5000 / 10000.

| N échantillons | Temps (mean ± σ) | Range (min…max) | Remarque |
|---:|---:|---:|---|
| 1000  | 52.2 ms ± 1.3 ms | 51.3 ms … 59.8 ms | outliers détectés |
| 2000  | 52.7 ms ± 3.8 ms | 51.3 ms … 76.3 ms | outliers détectés |
| 5000  | 52.8 ms ± 0.5 ms | 52.0 ms … 54.0 ms | — |
| 10000 | 53.4 ms ± 1.6 ms | 52.4 ms … 60.8 ms | outliers détectés |

## Analyse
### Analyse des performances
Les mesures montrent un temps moyen d’environ **53–55 ms** sur `80bpm0.csv`, ce qui est rapide pour ce type de traitement.

L’étude avec différentes valeurs de N (1000 à 10000) montre que le temps varie très peu. Une interprétation plausible est que le temps mesuré est en grande partie dominé par des coûts qui changent peu avec N (par exemple lecture du CSV, allocations internes, écriture du JSON), et que la partie “traitement du signal” n’est pas la seule composante du temps total.

Les warnings d’outliers affichés par `hyperfine` indiquent que certaines exécutions ont été perturbées (activité du système). Pour réduire ce phénom��ne, on pourrait relancer sur une machine plus “calme” ou fixer le programme sur un cœur CPU (par exemple avec `taskset`).

### Analyse des résultats (pics R)
Le programme détecte **22 pics R** sur le fichier testé, et calcule **21 RR**. Le seuil est calculé automatiquement (`threshold auto = 0.610762`).  
À ce stade, je me suis concentrée sur l’exécution et les mesures de performance, sans faire d’étape supplémentaire de validation du contenu du JSON ni de visualisation du signal.

Pour améliorer la validation dans une prochaine étape, il serait pertinent de :
- relire `out.json` et calculer la fréquence cardiaque moyenne à partir des RR,
- tracer le signal ECG et superposer les pics R pour vérifier visuellement la qualité de la détection.