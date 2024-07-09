#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

const float YAW         = -glm::half_pi<float>();
const float PITCH       =  0.0f;
const float SPEED       =  1.0f;
const float SENSITIVITY =  1.0f;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT, 
    UP, 
    DOWN
};

class Camera {
private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_horizontalAngle;
    float m_verticalAngle;

    float m_movementSpeed;
    float m_mouseSensitivity;

    void updateCameraVectors();
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    glm::mat4 getViewMatrix();
    inline glm::vec3 const getCameraPos() { return m_position; }
    inline float const getCameraHorizAngle() { return m_horizontalAngle; }
    inline float const getCameraVerticAngle() { return m_verticalAngle; }

    inline float getSensitivity() { return m_mouseSensitivity; }
    inline void setSensitivity(float sens) { m_mouseSensitivity = sens; }

    void processKeyboard(std::vector<Camera_Movement> directions, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

};