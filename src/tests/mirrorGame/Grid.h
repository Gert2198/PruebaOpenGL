#pragma once

#include "Mirror_End.h"
#include "DoubleMirror.h"

class Grid {
private:
    unsigned char m_width, m_height;
    glm::vec2 position;

    std::unordered_map<unsigned char, Mirror_End> m_mirrors;
    std::unordered_map<unsigned char, DoubleMirror> m_doubleMirrors;
public: 
    void pick(int index);

    void place(int index, Mirror_End mirror);
    void place(int index, DoubleMirror mirror);

    void rotateClockwise(int i, int j);
    void rotateCounterclockwise(int i, int j);
};