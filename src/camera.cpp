#include "camera.hpp"
#include "window.hpp"
#include "input.hpp"

// Constructor for camera -- initialise with some default values
Camera::Camera() : position{0.0f, 50.0f, 100.0f}, speed{50.0f} {
}

// Update the camera to respond to mouse motion for rotations and keyboard for translation
void Camera::update(float dt) {
    setViewByMouse();
    translateByKeyboard(dt);
}

// Update the camera to respond to mouse move for rotation
void Camera::setViewByMouse() {
    auto& window = Window::getInstance();
    if (!window.Locked())
        return;

    glm::vec2 delta = Input::MouseDelta() / static_cast<float>(window.getHeight() * 2);
    yaw -= delta.x;
    pitch -= delta.y;

    static constexpr float limit = glm::radians(89.0f);
    if (pitch > limit) {
        pitch = limit;
    }
    if (pitch < -limit) {
        pitch = -limit;
    }

    rotation = glm::quat{ glm::vec3{ pitch, yaw, 0 }};
}

// Update the camera to respond to key presses for translation
void Camera::translateByKeyboard(float dt) {
    glm::vec3 lastPos{ position };

    if (Input::GetKey(GLFW_KEY_W))
        position += getForwardVector() * speed * dt;

    if (Input::GetKey(GLFW_KEY_S))
        position -= getForwardVector() * speed * dt;

    if (Input::GetKey(GLFW_KEY_D))
        position += getRightVector() * speed * dt;

    if (Input::GetKey(GLFW_KEY_A) )
        position -= getRightVector() * speed * dt;

    velocity = (position - lastPos) * dt;
}

// Return the camera view
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + getForwardVector(), getUpVector());
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio,
// and near / far clipping planes
void Camera::setPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane) {
    perspectiveProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClippingPlane, farClippingPlane);
}

// The the camera orthographic projection matrix to match the width and height passed in
void Camera::setOrthographicProjectionMatrix(int width, int height) {
    orthographicProjectionMatrix = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
}