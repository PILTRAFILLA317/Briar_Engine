#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

#include "Mesh.hpp"

// Simple OBJ Loader
class OBJLoader {
public:
    static Mesh LoadOBJ(const std::string& filePath);
};
