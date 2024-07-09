#include "Camera.h"

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    m_front = glm::normalize(glm::vec3(cos(m_horizontalAngle) * cos(m_verticalAngle), 
                                        sin(m_verticalAngle), 
                                        sin(m_horizontalAngle) * cos(m_verticalAngle)));
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY)
{
    m_position = position;
    m_worldUp = up;
    m_horizontalAngle = yaw;
    m_verticalAngle = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(std::vector<Camera_Movement> directions, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;

    bool forward = false, backward = false, left = false, right = false, up = false, down = false;
    for (auto dir : directions) 
        switch (dir) {
            case FORWARD:   forward  = true; break;
            case BACKWARD:  backward = true; break;
            case LEFT:      left     = true; break;
            case RIGHT:     right    = true; break;
            case UP:        up       = true; break;
            case DOWN:      down     = true; break;
        }

    int input = forward + (backward << 1) + (left << 2) + (right << 3);
    /*
        0 = NOTHING, 1 = FORWARD, 2 = BACKWARD, 3 = FORWARD Y BACKWARD, 4 = LEFT, 
        5 = FORWARD Y LEFT, 6 = BACKWARD Y LEFT, 7 = FORWARD Y BACKWARD Y LEFT, 
        8 = RIGHT, 9 = FORWARD Y RIGHT, 10 = BACKWARD Y RIGHT, 
        11 = FORWARD Y BACKWARD Y RIGHT, 12 = LEFT Y RIGHT, 
        13 = FORWARD Y LEFT Y RIGHT, 14 = BACKWARD Y LEFT Y RIGHT, 
        15 = FORWARD Y BACKWARD Y LEFT Y RIGHT

        Same output: 
        0, 3, 12, 15    -> No output
        1, 13           -> Forward
        2, 14           -> Backward
        4, 7            -> Left
        8, 11           -> Right
        5               -> Forward Left
        6               -> Backward Left
        9               -> Forward right
        10              -> Backward right 
    */
    glm::vec3 direction;
    switch (input) {
    case 0: case 3: case 12: case 15: break;
    case 1: case 13:    // forward
        m_position += glm::vec3(m_front.x, 0.0f, m_front.z) * velocity;
        break;
    case 2: case 14:    // backward
        m_position -= glm::vec3(m_front.x, 0.0f, m_front.z) * velocity;
        break;
    case 4: case 7:     // left
        m_position -= glm::vec3(m_right.x, 0.0f, m_right.z) * velocity;
        break;
    case 8: case 11:    // right
        m_position += glm::vec3(m_right.x, 0.0f, m_right.z) * velocity;
        break;
    case 5:             // Forward Left
        direction = glm::normalize(m_front - m_right);
        m_position += glm::vec3(direction.x, 0.0f, direction.z) * velocity;
        break;
    case 6:             // Backward Left
        direction = glm::normalize(- m_front - m_right);
        m_position += glm::vec3(direction.x, 0.0f, direction.z) * velocity;
        break;
    case 9:             // Forward right
        direction = glm::normalize(m_front + m_right);
        m_position += glm::vec3(direction.x, 0.0f, direction.z) * velocity;
        break;
    case 10:            // Backward right
        direction = glm::normalize(- m_front + m_right);
        m_position += glm::vec3(direction.x, 0.0f, direction.z) * velocity;
        break;
    }

    if (up && down) {}
    else if (up) m_position += m_worldUp * velocity;
    else if (down) m_position -= m_worldUp * velocity;

}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    float sensitivity = 0.00081875f * m_mouseSensitivity;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_horizontalAngle   += xoffset;
    m_verticalAngle     += yoffset;

    if (constrainPitch) {
        if (m_verticalAngle > glm::half_pi<float>() - 0.001)
            m_verticalAngle = glm::half_pi<float>() - 0.001;
        if (m_verticalAngle < - glm::half_pi<float>() + 0.001)
            m_verticalAngle = - glm::half_pi<float>() + 0.001;
    }

    updateCameraVectors();
}