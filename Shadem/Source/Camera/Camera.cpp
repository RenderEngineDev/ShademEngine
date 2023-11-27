#pragma once
#include "Camera/Camera.h"


// constructor with vectors
Camera::Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), worldUp(up), yaw(yaw), pitch(pitch) {
    updateCameraVectors();
}

// constructor with scalar values
Camera::Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : position(glm::vec3(posX, posY, posZ)), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch) {
    updateCameraVectors();
}


// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity; // first case //glm::vec3(front.x, sin(glm::radians(-front.y)), front.z) * velocity;
    if (direction == BACKWARD)
        position -= front * velocity; //glm::vec3(front.x, sin(glm::radians(-front.y)), front.z) * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
    if (direction == UP)
        position += up * velocity;
    if (direction == DOWN)
        position -= up * velocity;
    // second case // make sure the user stays at the ground level
    //position.y = 15.0f; // <-- this one-liner keeps the user at the ground level (xz plane)

}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update front, right and up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::Camera::ProcessMouseScroll(float yoffset)
{
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::Camera::updateCameraVectors()
{
    // calculate the new front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    // also re-calculate the right and up vector
    right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}