#pragma once

#include "common.hpp"

class Camera {
public:
	Camera(); // Constructor - sets default values for camera position, viewvector, upvector, and speed

    const glm::vec3& getPosition() const { return position; }			 // Gets the position of the camera centre of projection
    void setPosition(const glm::vec3& pos) { position = pos; }           // Sets the position of the camera centre of projection
    const glm::quat& getRotation() const { return rotation; }			 // Gets the rotation of the camera
    void setRotation(const glm::quat& rot) { rotation = rot; }           // Sets the rotation of the camera
    const glm::vec3& getVelocity() const { return velocity; }			 // Gets the velocity of the camera
    glm::vec3 getUpVector() const { return rotation * vec3::up; }		 // Gets the camera up vector
    glm::vec3 getForwardVector() const { return rotation * vec3::back; } // Gets the camera forward vector
    glm::vec3 getRightVector() const { return rotation * vec3::right; }	 // Gets the camera right vector
    const glm::mat4& getPerspectiveProjectionMatrix() const { return perspectiveProjectionMatrix; }	  // Gets the camera perspective projection matrix
    const glm::mat4& getOrthographicProjectionMatrix() const { return orthographicProjectionMatrix; } // Gets the camera orthographic projection matrix

	// Respond to mouse movement to rotate the camera
	void setViewByMouse();

	// Respond to keyboard presses on arrow keys to translate the camera
	void translateByKeyboard(float dt);

	// Update the camera
	void update(float dt);

    // Gets the camera view
    glm::mat4 getViewMatrix() const;

	// Set the projection matrices
	void setPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane);
	void setOrthographicProjectionMatrix(int width, int height);

private:
	glm::vec3 position{ 0.0f };
	glm::quat rotation{ 1, 0, 0, 0 };
    glm::vec3 velocity{ 0.0f };

	float speed{ 1.0f };
    float yaw{ 0.0f };
    float pitch{ 0.0f };

	glm::mat4 perspectiveProjectionMatrix{ 1.0f };
	glm::mat4 orthographicProjectionMatrix{ 1.0f };
};
