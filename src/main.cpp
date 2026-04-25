#include <iostream>
#include "storage.h"

int main() {
    Storage storage;

    storage.put("name", "Vlad");
    storage.put("lang", "C++");

    std::cout << "name: " << storage.get("name") << std::endl;
    std::cout << "lang: " << storage.get("lang") << std::endl;

    storage.remove("name");

    std::cout << "after delete name: " << storage.get("name") << std::endl;

    return 0;
}