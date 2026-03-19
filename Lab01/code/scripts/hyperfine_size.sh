cd ../build

hyperfine --warmup 10 \
  './ecg_dealination ../80bpm0.csv /tmp/out.json 1000' \
  './ecg_dealination ../80bpm0.csv /tmp/out.json 2000' \
  './ecg_dealination ../80bpm0.csv /tmp/out.json 5000' \
  './ecg_dealination ../80bpm0.csv /tmp/out.json 10000'

