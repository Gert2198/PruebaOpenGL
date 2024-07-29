#pragma once

#include "../Test.h"

#include "CollisionManager.h"

#include "VertexArray.h"
#include "Shader.h"
#include <memory>

namespace test
{
    class TestMirrors : public Test {
    private: 
        AABB m_rect;
        Segment m_seg;
        Ray2D m_ray;
        float m_rayAngle;
        glm::vec2 m_firstPos, m_firstDir;
        std::vector<Segment> m_segments;

        CollisionManager m_collisionManager;

        glm::mat4 m_projMatrix;

        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<Shader> m_shader;
    public: 
        TestMirrors(GLFWwindow* window);
        ~TestMirrors();

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        // Callbacks
        // void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    };
} // namespace test


// Quiero dar controles a las cosas, pero primero necesito una forma de saber en qué estado estoy. 
// Dependiendo del estado, podré hacer unas cosas u otras. Por ejemplo, si selecciono un objeto para introducirlo al grid, pasaré a modo Insertar. 
// Cuando inserte el objeto, pasaré a modo Seleccionar. 

// En el modo Insertar, tendré un objeto seleccionado, y podré moverlo por la pantalla para posicionarlo en el grid. También podre rotarlo. Habrá
// algun atributo que diga si puedo seleccionar el objeto o no dentro del grid. Si no puedo seleccionarlo, hacer click en el objeto no cambiará 
// nada. 