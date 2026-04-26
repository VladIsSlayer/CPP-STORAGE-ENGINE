#include <iostream>
#include <sstream>
#include "storage.h"

int main() {
    Storage storage("data.db");

    storage.load();
    
    std::cout << "Simple Storage CLI\n";
    std::cout << "Command: PUT key value | GET key | DEL key | ALL | CLEAR | EXIT\n";

    std::string line;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        std::istringstream iss(line);
        std::string command, key, value;

        iss >> command;

        if (command == "PUT") {
            iss >> key >> value;
            storage.put(key, value);
            std::cout << "OK\n";
        }
        else if (command == "GET") {
            iss >> key;
            std::string result = storage.get(key);

            if (result.empty()) {
                std::cout << "(null)\n";
            } else {
                std::cout << result << "\n";
            }
        }
        else if (command == "DEL") {
            iss >> key;
            storage.remove(key);
            std::cout << "OK\n";
        }
        else if (command == "EXIT") {
            break;
        }
        else if (command == "CLEAR") {
            storage.clear();
            std::cout << "OK\n";
        }
        else if (command == "ALL") {
            auto all =storage.get_all();

            for (const auto& [key, value] : all) {
                std::cout << key << " = " << value << "\n";
            }
        }
        else {
            std::cout << "Unknown command\n";
        }
    }
    return 0;
}