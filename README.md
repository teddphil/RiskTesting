# ATM Risk Testing

### Overview

ATM Risk Testing — Automated Testing Framework for Risk Models (C++), a lightweight C++17 framework to automate testing/validation of risk models with regulatory-style reporting (CSV + JUnit XML).

### Features
- Tiny dependency footprint (pure C++17).
- Define `RiskModel`s and `RiskTest`s.
- Built-in metrics: RMSE, bias, hit-rate (coverage), MAPE.
- Reporters:
  - CSVReporter: tabular record for audit/regulatory export.
  - JUnitXmlReporter: CI-friendly test summary.
- Sample model: BasicVaR95 (historical VaR @ 95%).

### Usage

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

### Structure
```text
atm-risk-testing/
├─ include/atm/              # framework headers
├─ src/                      # framework implementation
├─ models/                   # sample risk models
├─ tests/                    # example tests (entry point)
├─ data/                     # sample data
├─ reports/                  # output artifacts (git-kept)
├─ .github/workflows/ci.yml  # GitHub Actions
├─ CMakeLists.txt
├─ README.md
└─ LICENSE
```
