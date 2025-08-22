# ATM Risk Testing — Automated Testing Framework for Risk Models (C++)

A lightweight C++17 framework to **automate testing/validation** of risk models with
**regulatory-style reporting** (CSV + JUnit XML) suitable for CI (e.g., GitHub Actions).

## Features
- Tiny dependency footprint (pure C++17).
- Define **RiskModel**s and **RiskTest**s.
- Built-in metrics: RMSE, bias, hit-rate (coverage), MAPE.
- Reporters:
  - **CSVReporter**: tabular record for audit/regulatory export.
  - **JUnitXmlReporter**: CI-friendly test summary.
- Sample model: **BasicVaR95** (historical VaR @ 95%).

## Build
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## Run
```bash
./atm_runner --outdir ../reports
```

Reports are written to `reports/`:
- `results.csv` (regulatory-style tabular record)
- `junit.xml` (CI parsing)
- `summary.json` (machine-readable)

## Repo Structure
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

## Add your own model
1. Derive from `atm::RiskModel` and implement `id()`, `version()`, and `score(...)` or `predict(...)` as needed.
2. Create test cases using `atm::RiskTest`.
3. Register them in `tests/test_main.cpp`.

## Regulatory-style CSV schema
Columns in `results.csv`:
- `timestamp_utc`, `model_id`, `model_version`, `dataset_id`, `test_type`, `metric_name`, `metric_value`,
  `threshold`, `pass`, `notes`

## License
MIT — see `LICENSE`.
