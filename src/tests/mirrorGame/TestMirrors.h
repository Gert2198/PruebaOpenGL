#pragma once

#include "../Test.h"

#include "Mirror_End.h"
#include "DoubleMirror.h"
#include "MirrorConstants.h"

#include "VertexArray.h"
#include "Shader.h"
#include <memory>

namespace test
{
    class TestMirrors : public Test {
    private: 
        Ray2D m_ray;
        glm::vec2 m_firstPos, m_firstDir;

        // 0 -> aire, 1 -> rayo, 2 -> mirror_end, 3 -> doubleMirror, 4 -> barrier, 5 -> blocker
        unsigned char grid[GRID_SIZE][GRID_SIZE];

        std::unordered_map<unsigned char, Mirror_End> m_mirrors;
        std::unordered_map<unsigned char, DoubleMirror> m_doubleMirrors;

        Mirror_End m_mirror;
        std::vector<Segment> m_drawableSegments;

        int m_targets;

        glm::mat4 m_projMatrix;

        Shader m_shader;
    public: 
        TestMirrors(GLFWwindow* window);
        ~TestMirrors();

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        void drawGrid();

        // Callbacks
        // void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

        // Necesito funciones pick y place
    };
} // namespace test


// Quiero dar controles a las cosas, pero primero necesito una forma de saber en qué estado estoy. 
// Dependiendo del estado, podré hacer unas cosas u otras. Por ejemplo, si selecciono un objeto para introducirlo al grid, pasaré a modo Insertar. 
// Cuando inserte el objeto, pasaré a modo Seleccionar. 

// En el modo Insertar, tendré un objeto seleccionado, y podré moverlo por la pantalla para posicionarlo en el grid. También podre rotarlo. Habrá
// algun atributo que diga si puedo seleccionar el objeto o no dentro del grid. Si no puedo seleccionarlo, hacer click en el objeto no cambiará 
// nada. 