#pragma once
#include <string>

class FileLoader {
public:
    static std::string loadTextFile(const char* filepath);
};