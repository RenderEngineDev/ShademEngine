#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window/window.h"

namespace Camera {

    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    // Default camera values
    const float YAW = 90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 15.0f;// 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;


    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera
    {
    private:
        glm::mat4 projection;
        glm::mat4 view;
        glm::vec2 range = glm::vec2(0.1f, 250.0f);

    public:
        // camera Attributes
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        // euler Angles
        float yaw;
        float pitch;
        // camera options
        float movementSpeed;
        float mouseSensitivity;
        float zoom;

        Window* window;

        // constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        
        // constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 getViewMatrix();

        void processViewAndProjection() {
            glm::vec2 screenSize(16,9);
            if (window)
                screenSize = window->getWindowSize();
            projection = glm::perspective(glm::radians(zoom), (float)screenSize.x / (float)screenSize.y, range.x, range.y);
            view = getViewMatrix();
        }

        glm::mat4& getProjection() {
            return projection;
        }

        glm::mat4& getView() {
            return view;
        }

        glm::vec2& getRange() { return range; }

        void linkWindow(Window* _window) { window = _window; }

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(CameraMovement direction, float deltaTime);

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset);

    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();
    };
}