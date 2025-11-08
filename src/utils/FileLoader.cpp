#include "FileLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string FileLoader::loadTextFile(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}