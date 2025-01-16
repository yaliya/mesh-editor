#pragma once

#include <glm/glm.hpp>

struct Camera {
    float fov;
    float far;
    float near;
    int viewWidth;
    int viewHeight;

    glm::vec3 up{};
    glm::vec3 front{};
    glm::vec3 right{};
    glm::vec3 position{};
    glm::vec3 rayDirection{};
    glm::vec3 rayOrigin{};

    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 0.01f;
    float sensivity = 0.25f;

    float lastX = 0;
    float lastY = 0;
    bool mouseStart = true;
    bool mouseClick = false;

    void moveLeft();
    void moveRight();
    void resetMouse();
    void moveForward();
    void moveBackwards();
    void trackMouse(float mx, float my);

    void focusAt(glm::vec3 position);
    void screenToWorld(float mx, float my);
    [[nodiscard]] glm::mat4 getProjection() const;
    [[nodiscard]] glm::mat4 getView() const;

    [[nodiscard]] glm::mat4 getTransform() const;

    Camera(int viewWidth, int viewHeight, float fov, float near, float far);
    Camera(glm::vec3& position, glm::vec3& front, glm::vec3& right, glm::vec3& up, int viewWidth, int viewHeight, float fov, float near, float far);
};