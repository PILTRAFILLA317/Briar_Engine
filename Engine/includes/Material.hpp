#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

#include "Texture.hpp"

struct TextureComb
{
    Texture mainTex;
    Texture normalMap;
    Texture specularMap;
};

class Material
{
public:
    std::string name;
    TextureComb textures;

    Material(std::string name, TextureComb textures)
    {
        if (name == "")
            name = "default";
        else
            Material::name = name;
        Material::textures = textures;
    }
};