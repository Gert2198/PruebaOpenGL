#pragma once

#include "Test.h"

#include "Circle.h"

#include "Shader.h"

#include "glm/glm.hpp"

typedef unsigned int uint;

struct int2 {
    int x, y;
    int2(int x, int y) : x(x), y(y) {}
    int2 operator+(const int2& other) const {
        return int2(x + other.x, y + other.y);
    }
    int2 operator-(const int2& other) const {
        return int2(x - other.x, y - other.y);
    }
    int2 operator*(int scalar) const {
        return int2(x * scalar, y * scalar);
    }
    int2 operator/(int divisor) const {
        return int2(x / divisor, y / divisor);
    }
    int2 operator%(int modulo) const {
        return int2(x % modulo, y % modulo);
    }
};
struct uint2 {
    uint x, y;
    uint2(uint x, uint y) : x(x), y(y) {}
    uint2 operator+(const uint2& other) const {
        return uint2(x + other.x, y + other.y);
    }
    uint2 operator-(const uint2& other) const {
        return uint2(x - other.x, y - other.y);
    }
    uint2 operator*(uint scalar) const {
        return uint2(x * scalar, y * scalar);
    }
    uint2 operator/(uint divisor) const {
        return uint2(x / divisor, y / divisor);
    }
    uint2 operator%(uint modulo) const {
        return uint2(x % modulo, y % modulo);
    }
};

namespace test 
{
    class TestFluid : public Test {
    private: 
        uint GRID_ROWS; 
        uint GRID_COLUMNS;
        void resizeGrid();
        void updateGrid();
        int2 getCellByPosition(const glm::vec2& position, const glm::vec2& cellSize);
        void updatePredictedGrid();

        std::vector<Circle> m_circles;
        std::vector<float> m_particleProperties;
        std::vector<float> m_densities, m_nearDensities; // Hay que calcular esto antes de calcular ninguna otra propiedad
        std::vector<glm::vec2> m_predictedPositions;
        int m_circleCount;

        std::vector<std::vector<uint>> m_grid; 
        std::vector<std::vector<uint>> m_predictedGrid;


        int2 m_offsets2D[9] = { // El cuadrado de alrededor de la celda en la que estamos
            {-1,  1}, { 0,  1}, { 1,  1},
            {-1,  0}, { 0,  0}, { 1,  0},
            {-1, -1}, { 0, -1}, { 1, -1}
        };

        float m_gravity, m_radius, m_damping, m_spacing;
        float m_smoothingRadius;
        float m_targetDensity, m_pressuerMultiplier, m_nearPressuerMultiplier, m_viscosityStrength;

        glm::vec2 m_bounds;

        glm::mat4 m_projMatrix, m_viewMatrix;

        Shader m_shader;

        bool leftClick, rightClick;

        glm::vec3 velocityToColor(glm::vec2 velocity, float maxSpeed) const;
    public: 
        TestFluid(GLFWwindow* window);

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        void processInput(GLFWwindow* window, float deltaTime);
        glm::vec2 getMousePosition();

        void createCircles();

        void checkEdges(Circle& circle, glm::vec2 boundsSize);
        void drawEdges();

        float smoothingKernel(float radius, float dist);
        float nearSmoothingKernel(float radius, float dist);
        float smoothingKernelDerivative(float radius, float dist);
        float viscositySmoothingKernel(float radius, float dist);

        float calculateDensity(glm::vec2 point);
        float calculateNearDensity(glm::vec2 point);
        float calculateProperty(glm::vec2 point);
        glm::vec2 calculatePressureForce(int particleIndex);
        glm::vec2 calculateNearPressureForce(int particleIndex);
        glm::vec2 calculateViscosityForce(int particleIndex);

        float convertDensityToPressure(float density);
        float convertDensityToNearPressure(float nearDensity);
        glm::vec2 interactionForce(glm::vec2 inputPos, float radius, float strength, int particleIndex);
    };
}