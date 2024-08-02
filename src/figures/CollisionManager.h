#pragma once

#include "AABB.h"
#include "Circle.h"
#include "Ray2D.h"
#include "Segment.h"

class CollisionManager {
private:
    float cuandoColisionan(const glm::vec2& p, const Circle& c) const;
public:
    bool checkPointInside(const glm::vec2& point, const Circle& circle) const;
    bool checkPointInside(const glm::vec2& point, const AABB& aabb) const;
    bool checkPointInside(const glm::vec2& point, const Segment& seg) const;

    bool checkCollision(const Circle& a, const Circle& b);
    bool checkCollision(const AABB& a, const Circle& b);
    bool checkCollision(const AABB& a, const AABB& b);
    
    std::pair<bool, glm::vec2> impactPoint(Circle& a, Circle& b) const;
    std::pair<bool, glm::vec2> impactPoint(AABB& a, Circle& b) const;

    std::pair<bool, glm::vec2> impactPoint(const Ray2D& ray, const Segment& seg) const;
    std::pair<bool, std::pair<glm::vec2, Segment>> impactPoint(const Ray2D& ray, const AABB& rect) const;

    bool intersects(const Ray2D& ray, const Segment& seg);
    bool intersects(const Ray2D& ray, const AABB& rect);

    void resolveCollision(Circle& a, Circle& b);
    void resolveCollision(AABB& a, Circle& b);

    // Returns a Segment for the main program to draw that segment, and modifies the Ray to reflect with the object
    std::pair<bool, Segment> resolveCollision(Ray2D& ray, Segment& seg);
    std::pair<bool, Segment> resolveCollision(Ray2D& ray, AABB& rect);

    glm::vec2 reflectedVector(const glm::vec2& rayDirection, const Segment& surfaceDirection) const;
};