#include "TestFluid.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "Color.h"

#include "Segment.h"
#include "CollisionManager.h"

namespace test
{
    TestFluid::TestFluid(GLFWwindow* window) : Test(window), 
        m_circleCount(2000),
        m_gravity(10.f), m_radius(0.05f), m_damping(0.5f), m_spacing(0.f),
        m_smoothingRadius(0.2f),
        m_targetDensity(12.f), m_pressuerMultiplier(5.f), m_nearPressuerMultiplier(5.f), m_viscosityStrength(2.f),
        m_bounds(16.f, 9.f),
        m_projMatrix(glm::ortho(-8.f, 8.f, -4.5f, 4.5f)), 
        m_viewMatrix(glm::mat4(1.f)), 
        m_shader("../res/shaders/circle.glsl"), 
        leftClick(false), rightClick(false)
    {
        createCircles();
        m_densities = std::vector<float>(m_circleCount);
        m_nearDensities = std::vector<float>(m_circleCount);
        m_predictedPositions = std::vector<glm::vec2>(m_circleCount);
        resizeGrid();
    }

    void TestFluid::resizeGrid() {
        uint columns = 1;
        while(m_bounds.x / (columns + 1) >= m_smoothingRadius)  // Me sirve
            columns++;
        uint rows = 1;
        while(m_bounds.y / (rows + 1) >= m_smoothingRadius) // Me sirve
            rows++;
        
        GRID_COLUMNS = columns;
        GRID_ROWS = rows;

        m_grid.clear();
        m_grid.resize(GRID_ROWS * GRID_COLUMNS);

        m_predictedGrid.clear();
        m_predictedGrid.resize(GRID_ROWS * GRID_COLUMNS);
    }
    int2 TestFluid::getCellByPosition(const glm::vec2& position, const glm::vec2& cellSize) {
        glm::vec2 pos = position + m_bounds * 0.5f;
        int column;
        int row;
        if (pos.x < 0.f) 
            column = 0;
        else if (pos.x >= m_bounds.x)
            column = GRID_COLUMNS - 1;
        else 
            column = pos.x / cellSize.x;

        if (pos.y < 0.f) 
            row = 0;
        else if (pos.y >= m_bounds.y)
            row = GRID_ROWS - 1;
        else 
            row = pos.y / cellSize.y;

        return {column, row};
    }
    void TestFluid::updateGrid() {
        for (int i = 0; i < GRID_COLUMNS * GRID_ROWS; i++) 
            m_grid[i].clear();

        // Conozco de donde a donde va el grid, y conozco en cuantas filas/columnas se descompone
        // Puedo pasar todas las posiciones de [-m_bounds/2, m_bounds/2] a [0, m_bounds] sumando m_bounds/2 a las posiciones de los circulos
        // Tambien puedo saber la longitud de los rectángulos que forman el grid
        // Horizontal -> m_bounds.x / GRID_COLUMNS
        // Vertical   -> m_bounds.y / GRID_ROWS
        // Entonces si divido la posicion entre los tamaños de las celdas, puedo saber la posicion que ocuparía en el grid
        // pos.x / horizontal = i; pos.y / vertical = j;
        // Luego solo queda insertar el circulo en el m_grid[i * GRID_COLUMNS + j]

        glm::vec2 cellSize(m_bounds.x / GRID_COLUMNS, m_bounds.y / GRID_ROWS);
        for (int i = 0; i < m_circleCount; i++) {
            int2 cellPos = getCellByPosition(m_circles[i].getPosition(), cellSize);
            m_grid[cellPos.y * GRID_COLUMNS + cellPos.x].push_back(i);
        }
    }
    void TestFluid::updatePredictedGrid() {
        for (int i = 0; i < GRID_COLUMNS * GRID_ROWS; i++) 
            m_predictedGrid[i].clear();

        glm::vec2 cellSize(m_bounds.x / GRID_COLUMNS, m_bounds.y / GRID_ROWS);
        for (int i = 0; i < m_circleCount; i++) {
            int2 cellPos = getCellByPosition(m_predictedPositions[i], cellSize);
            m_predictedGrid[cellPos.y * GRID_COLUMNS + cellPos.x].push_back(i);
        }
    }

    glm::vec3 TestFluid::velocityToColor(glm::vec2 velocity, float maxSpeed) const {
        float speed = glm::length(velocity);
        float normSpeed = speed / maxSpeed;

        return {normSpeed, 0.f, 1.f - normSpeed};
    }

    void TestFluid::onUpdate(float deltaTime) {
        leftClick = false;
        rightClick = false;
        processInput(m_window, deltaTime);

        updateGrid();

        glm::vec2 mousePos = getMousePosition();
        float strength = leftClick ? 30.f : rightClick ? -30.f : 0.f;

        for (int i = 0; i < m_circles.size(); i++) {
            glm::vec2 circlePos = m_circles[i].getPosition();

            glm::vec2 gravity(0.f, -m_gravity);
            m_circles[i].setVelocity(m_circles[i].getVelocity() + gravity * deltaTime);
            m_predictedPositions[i] = circlePos + m_circles[i].getVelocity() / 120.f;
        }

        updatePredictedGrid();

        for (int i = 0; i < m_circles.size(); i++) {
            m_densities[i]      = calculateDensity(m_predictedPositions[i]);
            m_nearDensities[i]  = calculateNearDensity(m_predictedPositions[i]);
        }

        for (int i = 0; i < m_circles.size(); i++) {
            glm::vec2 circlePos = m_circles[i].getPosition();

            glm::vec2 pressureForce = calculatePressureForce(i);
            glm::vec2 pressureAcceleration = pressureForce / m_densities[i];

            glm::vec2 nearPressureForce = calculateNearPressureForce(i);
            glm::vec2 nearPressureAcceleration = nearPressureForce / m_densities[i];

            glm::vec2 interaction = interactionForce(mousePos, 2.f, strength, i);
            glm::vec2 viscosityForce = calculateViscosityForce(i);
            glm::vec2 viscosityAcceleration = viscosityForce / m_densities[i];


            m_circles[i].setVelocity(m_circles[i].getVelocity() + interaction * deltaTime);
            // m_circles[i].setVelocity(m_circles[i].getVelocity() + viscosityAcceleration * deltaTime);
            m_circles[i].setVelocity(m_circles[i].getVelocity() + pressureAcceleration * deltaTime);
            // m_circles[i].setVelocity(m_circles[i].getVelocity() + nearPressureAcceleration * deltaTime);

            m_circles[i].setPosition(circlePos + m_circles[i].getVelocity() * deltaTime);
            checkEdges(m_circles[i], m_bounds);
        }

        // CollisionManager cManager;
        // for (int i = 0; i < m_circles.size(); i++) {
        //     for (int j = i + 1; j < m_circles.size(); j++) {
        //         if (cManager.checkCollision(m_circles[i], m_circles[j])) 
        //             cManager.resolveCollision(m_circles[i], m_circles[j]);
        //     }
        // }
    }
    void TestFluid::onRender() {
        Renderer renderer;
        m_shader.bind();

        for (auto& m_circle : m_circles) {
            m_shader.setUniformMat4f("u_MVP", m_projMatrix * m_viewMatrix * m_circle.getModelMatrix());
            m_shader.setUniform3f("u_color", velocityToColor(m_circle.getVelocity(), 3.f));
            renderer.drawCircle(m_circle, m_shader);
        }

        m_shader.setUniformMat4f("u_MVP", m_projMatrix * m_viewMatrix);
        m_shader.setUniform3f("u_color", glm::vec3(0.f, 1.f, 0.f));
        drawEdges();

        glm::vec2 mousePos = getMousePosition();
        m_shader.setUniformMat4f("u_MVP", m_projMatrix * m_viewMatrix * glm::translate(glm::mat4(1.f), glm::vec3(mousePos, 0.f)));
        m_shader.setUniform3f("u_color", Color::white);
        renderer.drawCircleOutline(m_smoothingRadius, m_shader);
    }
    void TestFluid::onImGuiRender() {
        if (ImGui::SliderFloat("Bounds size X", &m_bounds.x, 0.f, 16.f)) {
            resizeGrid();
        }
        if (ImGui::SliderFloat("Bounds size Y", &m_bounds.y, 0.f, 9.f)) {
            resizeGrid();
        }
        ImGui::NewLine();
        ImGui::SliderFloat("Gravity", &m_gravity, 0.f, 10.f);

        ImGui::NewLine();
        if (ImGui::SliderInt("Particles", &m_circleCount, 0, 4000)) {
            createCircles();
            m_densities.resize(m_circleCount);
            m_nearDensities.resize(m_circleCount);
            m_predictedPositions.resize(m_circleCount);
        }
        ImGui::NewLine();
        if (ImGui::SliderFloat("Radius", &m_radius, 0.f, 2.f)) {
            createCircles();
        }
        ImGui::NewLine();
        if (ImGui::SliderFloat("Collision damping", &m_damping, 0.f, 1.f)) {
            for (auto& circle : m_circles) circle.setFriction(m_damping);
        }
        ImGui::NewLine();
        if (ImGui::SliderFloat("Spacing", &m_spacing, 0.f, 3.f)) {
            createCircles();
        }
        if (ImGui::SliderFloat("Smoothing radius", &m_smoothingRadius, 0.01f, 6.f)) {
            resizeGrid();
        }

        ImGui::NewLine();
        float density = calculateDensity(getMousePosition());
        ImGui::InputFloat("Density", &density);
        ImGui::SliderFloat("Target density", &m_targetDensity, 0.f, 100.f);
        ImGui::SliderFloat("Pressure multipiler", &m_pressuerMultiplier, 0.f, 100.f);
        ImGui::SliderFloat("Near pressure multipiler", &m_nearPressuerMultiplier, -2.f, 2.f);
        ImGui::SliderFloat("Viscosity strength", &m_viscosityStrength, 0.f, 3.f);

        ImGui::NewLine();
        ImGui::Text("%.2f FPS", ImGui::GetIO().Framerate);
    }

    void TestFluid::processInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            leftClick = true;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            rightClick = true;
        
        if (leftClick && rightClick) {
            leftClick = false;
            rightClick = false;
        }
    }
    glm::vec2 TestFluid::getMousePosition() {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);

        x /= 80;
        y /= 80;

        x -= 8;
        y -= 4.5;
        y = -y;

        return glm::vec2(x, y);
    }

    void TestFluid::createCircles() {
        m_circles.clear();

        int particlesPerRow = (int)std::sqrt(m_circleCount);
        int particlesPerCol = (m_circleCount - 1) / particlesPerRow + 1;
        float spacing = m_radius * 2 + m_spacing;

        for (int i = 0; i < m_circleCount; i++) {
            float x = (i % particlesPerRow - particlesPerRow * 0.5f + 0.5f) * spacing;
            float y = (i / particlesPerRow - particlesPerCol * 0.5f + 0.5f) * spacing;

            m_circles.push_back(Circle(m_radius, 1.f, m_damping, Color::cyan, glm::vec2(x, y)));
        }
    }

    void TestFluid::checkEdges(Circle& circle, glm::vec2 boundsSize) {
        glm::vec2 circlePos = circle.getPosition();
        glm::vec2 circleVel = circle.getVelocity();
        float circleRadius = circle.getRadius();
        float circleFriction = circle.getFriction();

        glm::vec2 halfBoundsSize = boundsSize * 0.5f;

        if (circlePos.x + circleRadius > halfBoundsSize.x) {
            circlePos.x = halfBoundsSize.x - circleRadius;
            circleVel.x *= -1 * circleFriction;
        }
        else if (circlePos.x - circleRadius < - halfBoundsSize.x) {
            circlePos.x = circleRadius - halfBoundsSize.x;
            circleVel.x *= -1 * circleFriction;
        }
        if (circlePos.y + circleRadius > halfBoundsSize.y) {
            circlePos.y = halfBoundsSize.y - circleRadius;
            circleVel.y *= -1 * circleFriction;
        }
        else if (circlePos.y - circleRadius < - halfBoundsSize.y) {
            circlePos.y = circleRadius - halfBoundsSize.y;
            circleVel.y *= -1 * circleFriction;
        }
        circle.setPosition(circlePos);
        circle.setVelocity(circleVel);
    }

    void TestFluid::drawEdges() {
        glm::vec2 halfBoundsSize = m_bounds * 0.5f;
        Renderer renderer;
        std::vector<Segment> segments;
        segments.push_back({glm::vec2(-halfBoundsSize.x, -halfBoundsSize.y), glm::vec2( halfBoundsSize.x, -halfBoundsSize.y)});
        segments.push_back({glm::vec2( halfBoundsSize.x, -halfBoundsSize.y), glm::vec2( halfBoundsSize.x,  halfBoundsSize.y)});
        segments.push_back({glm::vec2( halfBoundsSize.x,  halfBoundsSize.y), glm::vec2(-halfBoundsSize.x,  halfBoundsSize.y)});
        segments.push_back({glm::vec2(-halfBoundsSize.x,  halfBoundsSize.y), glm::vec2(-halfBoundsSize.x, -halfBoundsSize.y)});

        for (auto& segment : segments) {
            renderer.drawSegment(segment, m_shader, 1.f);
        }
    }

    float TestFluid::smoothingKernel(float radius, float dist) {
        if (dist >= radius) return 0;

        float volume = glm::pi<float>() * std::pow(radius, 4) / 6;
        float value = radius - dist;
        return value * value / volume;
    }
    float TestFluid::nearSmoothingKernel(float radius, float dist) {
        if (dist >= radius) return 0;

        float volume = glm::pi<float>() * std::pow(radius, 4) / 6;
        float value = radius - dist;
        return value * value * value / volume;
    }
    float TestFluid::smoothingKernelDerivative(float radius, float dist) {
        if (dist >= radius) return 0;
        float scale = - 12 / (glm::pi<float>() * std::pow(radius, 4));
        return scale * (radius - dist);
    }
    float TestFluid::viscositySmoothingKernel(float radius, float dist) {
        if (dist >= radius) return 0;

        float volume = glm::pi<float>() * std::pow(radius, 8) / 4;
        float value = std::max(0.f, radius * radius - dist * dist);
        return value * value * value / volume;
    }

    float TestFluid::calculateDensity(glm::vec2 point) {
        float density = 0.f;

        // Tengo que ver en qué celda está el punto, y solo ver las celdas adyacentes
        glm::vec2 cellSize(m_bounds.x / GRID_COLUMNS, m_bounds.y / GRID_ROWS);
        int2 cell = getCellByPosition(point, cellSize);

        for (int i = 0; i < 9; i++) {
            int2 cellToCheck = cell + m_offsets2D[i];
            // Si no está entre los limites, no miro
            if (cellToCheck.x < 0 || cellToCheck.y < 0 || cellToCheck.x >= GRID_COLUMNS || cellToCheck.y >= GRID_ROWS) continue;
            // Ahora tengo que coger todas las particulas que estan en la celda
            int gridPos = cellToCheck.y * GRID_COLUMNS + cellToCheck.x; // Esta es la celda real dentro del grid
            // El vector con los indices de las particulas que están dentro de la celda es m_predictedGrid[gridPos]
            for (int index : m_predictedGrid[gridPos]) {
                float dst = glm::length(m_predictedPositions[index] - point);
                if (dst > m_smoothingRadius) continue;
                float influence = smoothingKernel(m_smoothingRadius, dst);
                density += influence;
            }
        }

        return density;
    }
    float TestFluid::calculateNearDensity(glm::vec2 point) {
        float density = 0.f;

        // Tengo que ver en qué celda está el punto, y solo ver las celdas adyacentes
        glm::vec2 cellSize(m_bounds.x / GRID_COLUMNS, m_bounds.y / GRID_ROWS);
        int2 cell = getCellByPosition(point, cellSize);

        for (int i = 0; i < 9; i++) {
            int2 cellToCheck = cell + m_offsets2D[i];
            // Si no está entre los limites, no miro
            if (cellToCheck.x < 0 || cellToCheck.y < 0 || cellToCheck.x >= GRID_COLUMNS || cellToCheck.y >= GRID_ROWS) continue;
            // Ahora tengo que coger todas las particulas que estan en la celda
            int gridPos = cellToCheck.y * GRID_COLUMNS + cellToCheck.x; // Esta es la celda real dentro del grid
            // El vector con los indices de las particulas que están dentro de la celda es m_predictedGrid[gridPos]
            for (int index : m_predictedGrid[gridPos]) {
                float dst = glm::length(m_predictedPositions[index] - point);
                if (dst > m_smoothingRadius) continue;
                float influence = nearSmoothingKernel(m_smoothingRadius, dst);
                density += influence;
            }
        }

        return density;
    }

    float TestFluid::calculateProperty(glm::vec2 point) {
        float property = 0.f;

        // Tengo que ver en qué celda está el punto, y solo ver las celdas adyacentes
        glm::vec2 cellSize(m_bounds.x / GRID_COLUMNS, m_bounds.y / GRID_ROWS);
        int2 cell = getCellByPosition(point, cellSize);

        for (int i = 0; i < 9; i++) {
            int2 cellToCheck = cell + m_offsets2D[i];
            // Si no está entre los limites, no miro
            if (cellToCheck.x < 0 || cellToCheck.y < 0 || cellToCheck.x >= GRID_COLUMNS || cellToCheck.y >= GRID_ROWS) continue;
            // Ahora tengo que coger todas las particulas que estan en la celda
            int gridPos = cellToCheck.y * GRID_COLUMNS + cellToCheck.x; // Esta es la celda real dentro del grid
            // El vector con los indices de las particulas que están dentro de la celda es m_predictedGrid[gridPos]
            for (int index : m_predictedGrid[gridPos]) {
                float dst = glm::length(m_predictedPositions[index] - point);
                if (dst > m_smoothingRadius) continue;
                // float dst = glm::length(m_circles[i].getPosition() - point); TODO
                float influence = smoothingKernel(m_smoothingRadius, dst);
                float density = m_densities[i];
                property += m_particleProperties[i] / density * influence;
            }
        }

        return property;
    }

    glm::vec2 TestFluid::calculatePressureForce(int particleIndex) {
        glm::vec2 pressureForce(0.f);

        float density = m_densities[particleIndex];
        float pressure = convertDensityToPressure(density);

        // Tengo que ver en qué celda está el punto, y solo ver las celdas adyacentes
        glm::vec2 cellSize(m_bounds.x / GRID_COLUMNS, m_bounds.y / GRID_ROWS);
        int2 cell = getCellByPosition(m_predictedPositions[particleIndex], cellSize);

        for (int i = 0; i < 9; i++) {
            int2 cellToCheck = cell + m_offsets2D[i];
            // Si no está entre los limites, no miro
            if (cellToCheck.x < 0 || cellToCheck.y < 0 || cellToCheck.x >= GRID_COLUMNS || cellToCheck.y >= GRID_ROWS) continue;
            // Ahora tengo que coger todas las particulas que estan en la celda
            int gridPos = cellToCheck.y * GRID_COLUMNS + cellToCheck.x; // Esta es la celda real dentro del grid
            // El vector con los indices de las particulas que están dentro de la celda es m_predictedGrid[gridPos]
            for (int index : m_predictedGrid[gridPos]) {
                if (index == particleIndex) continue;

                // glm::vec2 offset(m_circles[i].getPosition() - m_circles[particleIndex].getPosition());
                glm::vec2 offset(m_predictedPositions[index] - m_predictedPositions[particleIndex]);
                float dst = glm::length(offset);

                if (dst >= m_smoothingRadius) continue;

                glm::vec2 dir = dst > 0 ? (offset) / dst : glm::vec2(0, 1);

                float neighbourDensity = m_densities[index];
                float neighbourPressure = convertDensityToPressure(neighbourDensity);

                float sharedPressure = (pressure + neighbourPressure) * 0.5;

                float slope = smoothingKernelDerivative(m_smoothingRadius, dst);

                pressureForce += dir * slope * sharedPressure / neighbourDensity;
            }
        }

        return pressureForce;
    }
    glm::vec2 TestFluid::calculateNearPressureForce(int particleIndex) {
        glm::vec2 pressureForce(0.f);

        float density = m_nearDensities[particleIndex];
        float pressure = convertDensityToNearPressure(density);

        // Tengo que ver en qué celda está el punto, y solo ver las celdas adyacentes
        glm::vec2 cellSize(m_bounds.x / GRID_COLUMNS, m_bounds.y / GRID_ROWS);
        int2 cell = getCellByPosition(m_predictedPositions[particleIndex], cellSize);

        for (int i = 0; i < 9; i++) {
            int2 cellToCheck = cell + m_offsets2D[i];
            // Si no está entre los limites, no miro
            if (cellToCheck.x < 0 || cellToCheck.y < 0 || cellToCheck.x >= GRID_COLUMNS || cellToCheck.y >= GRID_ROWS) continue;
            // Ahora tengo que coger todas las particulas que estan en la celda
            int gridPos = cellToCheck.y * GRID_COLUMNS + cellToCheck.x; // Esta es la celda real dentro del grid
            // El vector con los indices de las particulas que están dentro de la celda es m_predictedGrid[gridPos]
            for (int index : m_predictedGrid[gridPos]) {
                if (index == particleIndex) continue;

                // glm::vec2 offset(m_circles[i].getPosition() - m_circles[particleIndex].getPosition());
                glm::vec2 offset(m_predictedPositions[index] - m_predictedPositions[particleIndex]);
                float dst = glm::length(offset);

                if (dst >= m_smoothingRadius) continue;

                glm::vec2 dir = dst > 0 ? (offset) / dst : glm::vec2(0, 1);

                float neighbourDensity = m_nearDensities[index];
                float neighbourPressure = convertDensityToNearPressure(neighbourDensity);

                float sharedPressure = (pressure + neighbourPressure) * 0.5;

                float slope = smoothingKernelDerivative(m_smoothingRadius, dst);

                pressureForce += dir * slope * sharedPressure / neighbourDensity;
            }
        }

        return pressureForce;
    }

    glm::vec2 TestFluid::calculateViscosityForce(int particleIndex) {
        glm::vec2 viscosityForce(0.f);

        glm::vec2 position = m_circles[particleIndex].getPosition();

        // Tengo que ver en qué celda está el punto, y solo ver las celdas adyacentes
        glm::vec2 cellSize(m_bounds.x / GRID_COLUMNS, m_bounds.y / GRID_ROWS);
        int2 cell = getCellByPosition(position, cellSize);

        for (int i = 0; i < 9; i++) {
            int2 cellToCheck = cell + m_offsets2D[i];
            if (cellToCheck.x < 0 || cellToCheck.y < 0 || cellToCheck.x >= GRID_COLUMNS || cellToCheck.y >= GRID_ROWS) continue;
            int gridPos = cellToCheck.y * GRID_COLUMNS + cellToCheck.x; 

            for (int index : m_predictedGrid[gridPos]) {
                if (index == particleIndex) continue;
                float dst = glm::length(m_circles[index].getPosition() - position);
                if (dst >= m_smoothingRadius) continue;
                float influence = viscositySmoothingKernel(m_smoothingRadius, dst);
                viscosityForce += (m_circles[index].getVelocity() - m_circles[particleIndex].getVelocity()) * influence;
            }
        }

        return viscosityForce * m_viscosityStrength;
    }

    float TestFluid::convertDensityToPressure(float density) {
        float densityError = density - m_targetDensity;
        float pressure = densityError * m_pressuerMultiplier;
        return pressure;
    }
    float TestFluid::convertDensityToNearPressure(float nearDensity) {
        float nearPressure = nearDensity * m_nearPressuerMultiplier;
        return nearPressure;
    }

    glm::vec2 TestFluid::interactionForce(glm::vec2 inputPos, float radius, float strength, int particleIndex) {
        glm::vec2 interactionForce(0.f);
        glm::vec2 offset = inputPos - m_circles[particleIndex].getPosition();
        float sqrDst = glm::dot(offset, offset);
        
        if (sqrDst < radius * radius) {
            float dst = std::sqrt(sqrDst);
            glm::vec2 dirToInputPoint = dst <= 0.001f ? glm::vec2(0.f) : offset / dst;
            float centerT = 1 - dst / radius;
            interactionForce += (dirToInputPoint * strength - m_circles[particleIndex].getVelocity()) * centerT;
        }
        return interactionForce;
    }
} // namespace test
