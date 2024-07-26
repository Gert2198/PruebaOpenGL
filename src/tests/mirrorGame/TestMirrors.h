#pragma once

#include "Test.h"

namespace test
{
    class TestMirrors : public Test {
    private: 

    public: 
    };
} // namespace test


// Quiero dar controles a las cosas, pero primero necesito una forma de saber en qué estado estoy. 
// Dependiendo del estado, podré hacer unas cosas u otras. Por ejemplo, si selecciono un objeto para introducirlo al grid, pasaré a modo Insertar. 
// Cuando inserte el objeto, pasaré a modo Seleccionar. 

// En el modo Insertar, tendré un objeto seleccionado, y podré moverlo por la pantalla para posicionarlo en el grid. También podre rotarlo. Habrá
// algun atributo que diga si puedo seleccionar el objeto o no dentro del grid. Si no puedo seleccionarlo, hacer click en el objeto no cambiará 
// nada. 