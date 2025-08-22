#include "atm/Utils.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace atm::utils {

std::vector<double> loadCsvColumn(const std::string& path, size_t colIndex, bool hasHeader) {
    std::vector<double> out;
    std::ifstream in(path);
    std::string line;
    if (hasHeader && std::getline(in, line)) { /* skip */ }
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string cell;
        size_t idx = 0;
        double val = 0.0;
        while (std::getline(ss, cell, ',')) {
            if (idx == colIndex) { val = std::stod(cell); break; }
            ++idx;
        }
        out.push_back(val);
    }
    return out;
}

void ensureDir(const std::string& path) {
    fs::create_directories(path);
}

} // namespace atm::utils
