#include "storage.h"
#include <fstream>
#include <sstream>

Storage::Storage(const std::string& filename)
    : filename_(filename) {}

void Storage::put(const std::string& key, const std::string& value) {
    data_[key] = value;
    save();
}

std::string Storage::get(const std::string& key) {
    auto it = data_.find(key);
    if (it != data_.end()) {
        return it->second;
    }
    return "";
}

void Storage::remove(const std::string& key) {
    data_.erase(key);
    save();
}

void Storage::save() {
    std::ofstream file(filename_);

    for (const auto& [key, value] : data_) {
        file << key << "=" << value << '\n';
    }
}

void Storage::load() {
    std::ifstream file(filename_);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') && 
            std::getline(iss, value)) {
                data_[key] = value;
            }
    }
}

void Storage::compact() {
    std::ofstream file(filename_, std::ios::trunc);
    for (const auto& [key, value] : data_)
    {
        file << "PUT " << key << " " << value << '\n';
    }
}