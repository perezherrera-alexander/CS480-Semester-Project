#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void UpdateDirection(int direction);
    void UpdateCameraFront(glm::vec3 front);
    void UpdateFOV(float FOV, int w, int h);

    float getYaw() { return yaw; }
    void setYaw(float ya) { yaw = ya; }
    float getPitch() { return pitch; }
    void setPitch(float pit) { pitch = pit; }
    float getFOV() { return fov; }
    void setFOV(float FOV) { fov = FOV; }
    glm::vec3 getCameraFront() { return cameraFront; }
    void setCameraFront(glm::vec3 update) { cameraFront = update; }

  
  protected:
    glm::mat4 projection;
    glm::mat4 view;

    // Starting position
    float x = 0.0f;
    float y = 2.0f;
    float z = 20.0f;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw = -90.0f;
    float pitch = 0.0f;

    float fov = 45.0f;
};

#endif /* CAMERA_H */
