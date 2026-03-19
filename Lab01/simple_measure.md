```bash
 @nox  ./ecg_dealination ../80bpm0.csv out.json
CSV chargé avec 12 leads et 7500 échantillons.
[ecg_analyze] threshold auto = 0.610762 (max=1.745033)
[ecg_analyze] 22 pics R détectés, 21 RR calculés
22 pics R détectés.
[ecg_destroy] Contexte free
Analyse terminée. Résultats sauvegardés dans out.json
```

```bash
 @nox  /usr/bin/time -v ./ecg_dealination ../80bpm0.csv /tmp/out.json
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

```bash
 @nox  hyperfine --warmup 10 './ecg_dealination ../80bpm0.csv /tmp/out.json'
Benchmark 1: ./ecg_dealination ../80bpm0.csv /tmp/out.json
  Time (mean ± σ):      53.6 ms ±   0.9 ms    [User: 49.0 ms, System: 4.0 ms]
  Range (min … max):    52.5 ms …  56.9 ms    52 runs
 ```

``` bash
 @nox  ./hyperfine_size.sh
Benchmark 1: ./ecg_dealination ../80bpm0.csv /tmp/out.json 1000
  Time (mean ± σ):      52.2 ms ±   1.3 ms    [User: 48.2 ms, System: 3.5 ms]
  Range (min … max):    51.3 ms …  59.8 ms    53 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet system without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Benchmark 2: ./ecg_dealination ../80bpm0.csv /tmp/out.json 2000
  Time (mean ± σ):      52.7 ms ±   3.8 ms    [User: 48.3 ms, System: 3.6 ms]
  Range (min … max):    51.3 ms …  76.3 ms    54 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet system without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Benchmark 3: ./ecg_dealination ../80bpm0.csv /tmp/out.json 5000
  Time (mean ± σ):      52.8 ms ±   0.5 ms    [User: 48.0 ms, System: 4.2 ms]
  Range (min … max):    52.0 ms …  54.0 ms    53 runs
 
Benchmark 4: ./ecg_dealination ../80bpm0.csv /tmp/out.json 10000
  Time (mean ± σ):      53.4 ms ±   1.6 ms    [User: 49.0 ms, System: 3.9 ms]
  Range (min … max):    52.4 ms …  60.8 ms    51 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet system without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Summary
  ./ecg_dealination ../80bpm0.csv /tmp/out.json 1000 ran
    1.01 ± 0.08 times faster than ./ecg_dealination ../80bpm0.csv /tmp/out.json 2000
    1.01 ± 0.03 times faster than ./ecg_dealination ../80bpm0.csv /tmp/out.json 5000
    1.02 ± 0.04 times faster than ./ecg_dealination ../80bpm0.csv /tmp/out.json 10000
    ```