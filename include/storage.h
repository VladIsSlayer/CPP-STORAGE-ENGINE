#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <thread>
#include <condition_variable>

class Storage {

    public:
        Storage(const std::string& filename);
        
        void put(const std::string& key, const std::string& value);
        std::string get(const std::string& key);

        void remove(const std::string& key);
        void load();
        void save();
        void compact();
        ~Storage();
        void clear();
        std::unordered_map<std::string, std::string> get_all();

    private:
        std::unordered_map<std::string, std::string> data_;
        std::string filename_;
        std::shared_mutex mutex_;
        std::queue<std::string> write_queue_;
        std::mutex queue_mutex_;
        std::condition_variable cv_;
        std::thread worker_;
        bool stop_ = false;
};

