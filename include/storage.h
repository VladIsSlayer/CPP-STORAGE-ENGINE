#pragma once
#include <string>
#include <unordered_map>

class Storage {

    public:
        Storage(const std::string& filename);
        
        void put(const std::string& key, const std::string& value);
        std::string get(const std::string& key);

        void remove(const std::string& key);
        void load();
        void save();
        void compact();

    private:
        std::unordered_map<std::string, std::string> data_;
        std::string filename_;
};