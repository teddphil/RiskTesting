#pragma once
#include <string>
#include <vector>

namespace atm::utils {
    std::vector<double> loadCsvColumn(const std::string& path, size_t colIndex = 0, bool hasHeader = true);
    void ensureDir(const std::string& path);
}
