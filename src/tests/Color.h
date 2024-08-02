#pragma once

#include "glm/glm.hpp"

class Color {
public: 
    static glm::vec3 red;
    static glm::vec3 green;
    static glm::vec3 blue;

    static glm::vec3 yellow;
    static glm::vec3 cyan;
    static glm::vec3 magenta;

    static glm::vec3 white;
    static glm::vec3 black;
};

glm::vec3 Color::red        = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 Color::green      = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 Color::blue       = glm::vec3(0.f, 0.f, 1.f);

glm::vec3 Color::yellow     = glm::vec3(1.f, 1.f, 0.f);
glm::vec3 Color::cyan       = glm::vec3(0.f, 1.f, 1.f);
glm::vec3 Color::magenta    = glm::vec3(1.f, 0.f, 1.f);

glm::vec3 Color::white      = glm::vec3(1.f, 1.f, 1.f);
glm::vec3 Color::black      = glm::vec3(0.f, 0.f, 0.f);