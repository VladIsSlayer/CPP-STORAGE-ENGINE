#include "storage.h"
#include <fstream>
#include <sstream>
#include <shared_mutex>

Storage::Storage(const std::string& filename)
    : filename_(filename) {}

void Storage::put(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex>lock(mutex_);
    
    data_[key] = value;

    std::ofstream file(filename_, std::ios::app);
    file << "PUT " << key << " " << value << "\n";
}

std::string Storage::get(const std::string& key) {
    std::lock_guard<std::shared_mutex> lock(mutex_);

    auto it = data_.find(key);
    if (it != data_.end()) {
        return it->second;
    }
    return "";
}

void Storage::remove(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(mutex_);

    data_.erase(key);

    std::ofstream file(filename_, std::ios::app);
    file << "DEL " << key << "\n";
}

void Storage::save() {
    std::ofstream file(filename_);

    for (const auto& [key, value] : data_) {
        file << key << "=" << value << '\n';
    }
}

void Storage::load() {
    std::unique_lock<std::shared_mutex> lock(mutex_);

    std::ifstream file(filename_);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string command, key, value;

        iss >> command >> key;

        if (command == "PUT") {
            iss >> value;
            data_[key] = value;
        } else if (command == "DEL") {
            data_.erase(key);
        }
    }
}

void Storage::compact() {
    std::unique_lock<std::shared_mutex> lock(mutex_);

    std::ofstream file(filename_, std::ios::trunc);
    
    for (const auto& [key, value] : data_)
    {
        file << "PUT " << key << " " << value << '\n';
    }
}