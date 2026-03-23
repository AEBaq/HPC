# HPC, Laboratoire n°1 : Signaux ECG
**Étudiante :** Emily Baquerizo

**Date :** 3 mars 2026

## Choix d'implémentation
N'ayant pas de réelle connaissance dans le traitement de signaux ECG, j'ai préféré suivre les références fournies dans l’énoncé, en m’inspirant du principe général de l’algorithme de Pan–Tompkins (sans chercher à en faire une reproduction exacte) : prétraitement, filtrage passe-bande approximatif, dérivée puis mise au carré, intégration par fenetre glissante et détection des pics.

Afin de pouvoir exécuter avec un nombre différent d'échantillon, le main.c a légèrement été modifié pour recevoir, si désiré, un nombre d'échantillon.

## Résultats
Cette section reprend les manipulations effectuées afin d'obtenir les résultats.

### Génération de `out.json`
```bash
@nox$ ./ecg_dealination ../80bpm0.csv out.json
CSV chargé avec 12 leads et 7500 échantillons.
[ecg_analyze] threshold auto = 0.610762 (max=1.745033)
[ecg_analyze] 22 pics R détectés, 21 RR calculés
22 pics R détectés.
[ecg_destroy] Contexte free
Analyse terminée. Résultats sauvegardés dans out.json
```

### Mesure simple des performances (`/usr/bin/time -v`)
```bash
@nox$ /usr/bin/time -v ./ecg_dealination ../80bpm0.csv /tmp/out.json
CSV chargé avec 12 leads et 7500 échantillons.
[ecg_analyze] threshold auto = 0.610762 (max=1.745033)
[ecg_analyze] 22 pics R détectés, 21 RR calculés
22 pics R détectés.
[ecg_destroy] Contexte free
Analyse terminée. Résultats sauvegardés dans /tmp/out.json
	Command being timed: "./ecg_dealination ../80bpm0.csv /tmp/out.json"
	User time (seconds): 0.05
	System time (seconds): 0.00
	Percent of CPU this job got: 96%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.05
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 3636
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 0
	Minor (reclaiming a frame) page faults: 616
	Voluntary context switches: 1
	Involuntary context switches: 0
	Swaps: 0
	File system inputs: 0
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
```

Valeurs relevées qui peuvent être intéressantes:
- User time (seconds): 0.05
- System time (seconds): 0.00
- Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.05
- Percent of CPU this job got: : 96%
- Maximum resident set size (kbytes): 3636

### Benchmark (`hyperfine`)
```bash
@nox$ hyperfine --warmup 10 './ecg_dealination ../80bpm0.csv /tmp/out.json'
Benchmark 1: ./ecg_dealination ../80bpm0.csv /tmp/out.json
  Time (mean ± σ):      53.6 ms ±   0.9 ms    [User: 49.0 ms, System: 4.0 ms]
  Range (min … max):    52.5 ms …  56.9 ms    52 runs
```

### Impact de la taille des données (N échantillons)
Commande (via le script `scripts/hyperfine_size.sh`) : tests effectués avec N = {1000; 2000; 5000; 10000}.

| N échantillons | Temps (mean ± σ) | Range (min … max) |
|---:|---:|---:|---|
| 1000  | 52.2 ms ± 1.3 ms | 51.3 ms … 59.8 ms |
| 2000  | 52.7 ms ± 3.8 ms | 51.3 ms … 76.3 ms |
| 5000  | 52.8 ms ± 0.5 ms | 52.0 ms … 54.0 ms |
| 10000 | 53.4 ms ± 1.6 ms | 52.4 ms … 60.8 ms |

## Analyse
### Analyse des performances
Les mesures montrent un temps moyen de ~53–55 ms sur `80bpm0.csv`, ce qui est assez rapide pour ce type de traitement.

Les résultats avec différentes valeurs de N (1000 à 10000) montre que le temps varie très peu. Une hypothèse pourrait être que la pluaprt du temps est utilisé par des opérations qui ne dépendent pas vraiment de N, comme la lecture du CSV, les allocations mémoires ou l'écriture du json. Le traitement du signal n'est donc qu'une partie minime du temps total.

### Analyse des résultats (pics R)
Le programme détecte 22 pics R sur le fichier testé, et calcule 21 RR. Le seuil est calculé automatiquement (`threshold auto = 0.610762`).  
Voici le résultat du json :

```json
{
  "peaks": {
    "R": [1097, 1255, 1471, 1630, 1839, 2005, 2214, 2381, 2598, 2965, 3348, 3724, 4099, 4466, 4850, 5217, 5600, 5977, 6351, 6718, 7101, 7469]
  },
  "intervals": {
    "RR": [0.32, 0.43, 0.32, 0.42, 0.33, 0.42, 0.33, 0.43, 0.73, 0.77, 0.75, 0.75, 0.73, 0.77, 0.73, 0.77, 0.75, 0.75, 0.73, 0.77, 0.74]
  }
}
```
