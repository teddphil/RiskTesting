# Usage

Build
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

Run
```bash
./atm_runner --outdir ../reports
```

Reports are written to `reports/`:
- `results.csv` (regulatory-style tabular record)
  - `timestamp_utc`, `model_id`, `model_version`, `dataset_id`, `test_type`, `metric_name`, `metric_value`,
  `threshold`, `pass`, `notes`
- `junit.xml` (CI parsing)
- `summary.json` (machine-readable)