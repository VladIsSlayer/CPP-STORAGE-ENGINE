#include "storage.h"
#include <fstream>
#include <sstream>
#include <shared_mutex>

Storage::Storage(const std::string& filename)
    : filename_(filename) {

        worker_ = std::thread([this]() {
            std::ofstream file(filename_, std::ios::app);

            while (true) {
                std::unique_lock<std::mutex> lock(queue_mutex_);

                cv_.wait(lock, [this]() {
                    return ! write_queue_.empty() || stop_;
                });

                while (!write_queue_.empty())
                {
                    file << write_queue_.front() << "\n";
                    write_queue_.pop();
                }

                if (stop_) break;
            }
        });
    }

Storage::~Storage() {
    {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    stop_ = true;
    }

    cv_.notify_all();
    worker_.join();
}


void Storage::put(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex>lock(mutex_);
    
    data_[key] = value;

    {
        std::lock_guard<std::mutex> qlock(queue_mutex_);
        write_queue_.push("PUT " + key + " " + value);
    }

    cv_.notify_one();
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

    {
        std::lock_guard<std::mutex> qlock(queue_mutex_);
        write_queue_.push("DEL " + key);
    }

    cv_.notify_one();
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
        } else if (command == "CLEAR") {
            data_.clear();
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

void Storage::clear() {
    std::unique_lock<std::shared_mutex> lock(mutex_);

    data_.clear();

    {
        std::lock_guard<std::mutex> qlock(queue_mutex_);
        write_queue_.push("CLEAR");
    }
    cv_.notify_one();
}

std::unordered_map<std::string, std::string> Storage::get_all() {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return data_;
}