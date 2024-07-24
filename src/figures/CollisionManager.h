#pragma once

class AABB;
class Circle;

#include "AABB.h"
#include "Circle.h"

class CollisionManager {
private:
    glm::vec2* impactPoint(Circle& a, Circle& b) const;
    glm::vec2* impactPoint(AABB& a, Circle& b) const;

    float cuandoColisionan(const glm::vec2& p, const Circle& c) const;
public:
    bool checkPointInside(const glm::vec2& point, const Circle& circle) const;
    bool checkPointInside(const glm::vec2& point, const AABB& aabb) const;

    bool checkCollision(const Circle& a, const Circle& b);
    bool checkCollision(const AABB& a, const Circle& b);
    bool checkCollision(const AABB& a, const AABB& b);

    void resolveCollision(Circle& a, Circle& b);
    void resolveCollision(AABB& a, Circle& b);
};