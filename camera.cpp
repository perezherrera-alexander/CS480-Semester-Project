#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  cameraPos = glm::vec3(x, y, z);
  cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  
  view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

  projection = glm::perspective( glm::radians(fov), //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::UpdateCameraFront(glm::vec3 front)
{
    cameraFront = front;
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::UpdateDirection(int direction)
{
    const float cameraSpeed = 0.05f;
    switch (direction)
    {
    case 1:
        cameraPos += cameraSpeed * cameraFront;
        break;
    case 2:
        cameraPos -= cameraSpeed * cameraFront;
        break;
    case 3:
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        break;
    case 4:
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        break;
    case 5:
        cameraPos += cameraSpeed * cameraUp;
        break;
    case 6:
        cameraPos -= cameraSpeed * cameraUp;
        break;
    default:
        std::cout << "Invalid movement" << std::endl;
    }
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::UpdateFOV(float FOV, int w, int h)
{
    fov = FOV;
    projection = glm::perspective(  glm::radians(FOV), // FOV is already divided by 2
                                    float(w) / float(h), //Aspect Ratio
                                    0.01f, //Distance to the near plane
                                    100.0f); //Distance to the far plane
}