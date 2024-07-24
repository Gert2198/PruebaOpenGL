#include "CollisionManager.h"

bool CollisionManager::checkPointInside(const glm::vec2& point, const Circle& circle) const {
    float distance = glm::length(point - glm::vec2(circle.getPosition()));
    return distance <= circle.getRadius();
}
bool CollisionManager::checkPointInside(const glm::vec2& point, const AABB& aabb) const {
    return aabb.left() <= point.x && point.x <= aabb.right() && aabb.top() <= point.y && point.y <= aabb.bottom();
}

bool CollisionManager::checkCollision(const Circle& a, const Circle& b) {
    float distance = glm::length(a.getPosition() - b.getPosition());
    float radiusSum = a.getRadius() + b.getRadius();
    return distance < radiusSum;
}
bool CollisionManager::checkCollision(const AABB& a, const Circle& b) {
    glm::vec2 circlePos = b.getPosition();
    float r = b.getRadius();
    const glm::vec2* vertices = a.getVertices();
    glm::vec2 topRight = vertices[2];
    glm::vec2 bottomLeft = vertices[0];

    // Paso 1: Verificar si el centro del círculo está dentro del rectángulo
    if (a.left() <= circlePos.x && circlePos.x <= a.right() && a.bottom() <= circlePos.y && circlePos.y <= a.top()) 
        return true;

    // Paso 2: Verificar si el círculo intersecta algún lado del rectángulo
    // Lados verticales
    if (abs(circlePos.x - a.left()) <= r && a.bottom() <= circlePos.y && circlePos.y <= a.top()) {
        return true;
    }
    if (abs(circlePos.x - a.right()) <= r && a.bottom() <= circlePos.y && circlePos.y <= a.top()) {
        return true;
    }
    // Lados horizontales
    if (abs(circlePos.y - a.bottom()) <= r && a.left() <= circlePos.x && circlePos.x <= a.right()) {
        return true;
    }
    if (abs(circlePos.y - a.top()) <= r && a.left() <= circlePos.x && circlePos.x <= a.right()) {
        return true;
    }

    // Paso 3: Verificar si alguna esquina del rectángulo está dentro del círculo
    if ((circlePos.x - a.left()) * (circlePos.x - a.left()) + (circlePos.y - a.bottom()) * (circlePos.y - a.bottom()) <= r * r) {
        return true;
    }
    if ((circlePos.x - a.left()) * (circlePos.x - a.left()) + (circlePos.y - a.top()) * (circlePos.y - a.top()) <= r * r) {
        return true;
    }
    if ((circlePos.x - a.right()) * (circlePos.x - a.right()) + (circlePos.y - a.bottom()) * (circlePos.y - a.bottom()) <= r * r) {
        return true;
    }
    if ((circlePos.x - a.right()) * (circlePos.x - a.right()) + (circlePos.y - a.top()) * (circlePos.y - a.top()) <= r * r) {
        return true;
    }

    return false;
}
bool CollisionManager::checkCollision(const AABB& a, const AABB& b) {
    return b.left() <= a.right() && a.left() <= b.right() && b.bottom() <= a.top() && a.bottom() <= b.top();
}

void CollisionManager::resolveCollision(Circle& a, Circle& b) {
    glm::vec2 aPos = a.getPosition(),   bPos = b.getPosition();
    float aRadius = a.getRadius(),      bRadius = b.getRadius();
    float aMass = a.getMass(),          bMass = b.getMass();
    glm::vec2 aVel = a.getVelocity(),   bVel = b.getVelocity();

    float d = glm::length(bPos - aPos);
    float radiusSum = aRadius + bRadius;
    float overlap = d - radiusSum;
    glm::vec2 direction = glm::normalize(bPos - aPos);

    aPos = aPos + (overlap * 0.5f) * direction;
    bPos = bPos - (overlap * 0.5f) * direction;

    d = radiusSum;  // HAY QUE CORREGIR LA DISTANCIAAAA

    float massSum = aMass + bMass;
    glm::vec2 pDiff = bPos - aPos;

    float dot = glm::dot((bVel - aVel), (pDiff));

    float num1 = 2 * bMass * dot;
    float num2 = 2 * aMass * dot;
    float den = massSum * d * d;
    
    // Ajustar velocidades
    glm::vec2 vel1 = (aVel + (num1 / den) * (pDiff)); // * m_friction;
    glm::vec2 vel2 = (bVel - (num2 / den) * (pDiff)); // * other.m_friction;

    glm::vec2 error = (vel1 + vel2) - (aVel + bVel);

    a.setVelocity(vel1 - (0.5f * error));
    b.setVelocity(vel2 - (0.5f * error));
}

void CollisionManager::resolveCollision(AABB& a, Circle& b) {
    glm::vec2* impact = impactPoint(a, b);
    if (impact) {
        const glm::vec2* vertices = a.getVertices();
        if (*impact == vertices[0] || *impact == vertices[1] || *impact == vertices[2] || *impact == vertices[3]) {
            glm::vec2 normal = glm::normalize(*impact - b.getPosition());
            glm::vec2 velocity = b.getVelocity();
            b.setVelocity(velocity - 2 * glm::dot(velocity, normal) * normal);
        } else {
            if (impact->x == a.left() || impact->x == a.right()) 
                b.setVelocity(glm::vec2(- b.getVelocity().x, b.getVelocity().y));
            else if (impact->y == a.top() || impact->y == a.bottom()) 
                b.setVelocity(glm::vec2(b.getVelocity().x, -b.getVelocity().y));
        }
    }
}

glm::vec2* CollisionManager::impactPoint(Circle& a, Circle& b) const {
    return nullptr; // TODO: Implement this
}
glm::vec2* CollisionManager::impactPoint(AABB& a, Circle& b) const {
    const glm::vec2* vertices = a.getVertices();
    glm::vec2 circlePos = b.getPosition();
    glm::vec2 circleVel = b.getVelocity();
    float circleRadius = b.getRadius();

    glm::vec2* intersection = nullptr;
    float bestTime = std::numeric_limits<float>::infinity();
    for (int i = 0; i < 4; i++) {
        glm::vec2 p1 = vertices[i];
        glm::vec2 p2 = vertices[(i + 1) % 4];

        float t1, t2;
        if (p1.x == p2.x) {
            if (circleVel.x == 0) // No intersection (there could be but doesn't give information)
                continue;
            t1 = (p1.x - circlePos.x + circleRadius) / circleVel.x;
            t2 = (p1.x - circlePos.x - circleRadius) / circleVel.x;
            float min = std::min(t1, t2);
            float y = circlePos.y + circleVel.y * min;

            if (min < bestTime && y <= a.top() && y >= a.bottom()) {
                bestTime = min;
                if (intersection) delete intersection;
                intersection = new glm::vec2(p1.x, y);
            }

        } else if (p1.y == p2.y) {
            if (circleVel.y == 0) // No intersection (there could be but doesn't give information)
                continue;
            t1 = (p1.y - circlePos.y + circleRadius) / circleVel.y;
            t2 = (p1.y - circlePos.y - circleRadius) / circleVel.y;
            float min = std::min(t1, t2);
            float x = circlePos.x + circleVel.x * min;

            if (min < bestTime && x <= a.right() && x >= a.left()) {
                bestTime = min;
                if (intersection) delete intersection;
                intersection = new glm::vec2(x, p1.y);
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        float tiempo = cuandoColisionan(vertices[i], b);
        if (tiempo != std::numeric_limits<float>::infinity() && tiempo < bestTime) {
            bestTime = tiempo;
            if (intersection) delete intersection;
            intersection = new glm::vec2(vertices[i]);
        }
    }

    if (intersection) 
        b.setPosition(circlePos + circleVel * bestTime);

    return intersection;
}

float CollisionManager::cuandoColisionan(const glm::vec2& p, const Circle& c) const {
    glm::vec2 C = c.getPosition();
    glm::vec2 V = c.getVelocity();

    float r = c.getRadius();
    float rSquared = r * r;
    
    float VxSquared = V.x * V.x;
    float VySquared = V.y * V.y;
    float velSquaredSum = VxSquared + VySquared;
    float discriminant = rSquared * velSquaredSum - std::pow(V.x * (C.y - p.y) - V.y * (C.x - p.x), 2);
    if (discriminant < 0) return std::numeric_limits<float>::infinity();
    float squareRoot = std::sqrt(discriminant);
    float num = V.x * (p.x - C.x) + V.y * (p.y - C.y);

    float t1 = (num + squareRoot) / velSquaredSum;
    float t2 = (num - squareRoot) / velSquaredSum;

    return std::min(t1, t2);
}