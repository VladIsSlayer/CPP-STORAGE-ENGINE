#include "storage.h"

void Storage::put(const std::string& key, const std::string& value){
    data_[key] = value;
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
}