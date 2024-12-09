#pragma once
#include "/opt/homebrew/opt/glm/include/glm/glm.hpp"
#include "/opt/homebrew/opt/glm/include/glm/gtc/matrix_transform.hpp"
#include "/opt/homebrew/opt/glm/include/glm/gtc/type_ptr.hpp"
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;

    float MovementSpeed;
    float Sensitivity;
    float Yaw, Pitch;

    Camera(glm::vec3 startPosition)
        : Position(startPosition), Front(glm::vec3(0.0f, 0.0f, -1.0f)),
          Up(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(-90.0f), Pitch(0.0f),
          MovementSpeed(2.5f), Sensitivity(0.1f) {}

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboardInput(int key, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (key == GLFW_KEY_W)
            Position += Front * velocity;
        if (key == GLFW_KEY_S)
            Position -= Front * velocity;
        if (key == GLFW_KEY_A)
            Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
        if (key == GLFW_KEY_D)
            Position += glm::normalize(glm::cross(Front, Up)) * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= Sensitivity;
        yoffset *= Sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        UpdateCameraVectors();
    }

private:
    void UpdateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
    }
};
