

#include "engine.h"
#include "glm/ext.hpp"

Engine::Engine(const char* name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
  if(!m_window->Initialize())
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  m_camera = m_graphics->getCamera();

  glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);

  // Initilize static variables for the cursor function to use later
  firstMouse = true;
  yaw = m_camera->getYaw();
  pitch = m_camera->getPitch();
  cameraFront = m_camera->getCameraFront();
  fov = m_camera->getFOV();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while (!glfwWindowShouldClose(m_window->getWindow()))
  {
      ProcessInput();
      Display(m_window->getWindow(), glfwGetTime());
      glfwPollEvents();
  }
  m_running = false;

}

void Engine::ProcessInput()
{
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);

    // Update camera animation here.
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_camera->UpdateDirection(1);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_camera->UpdateDirection(2);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_camera->UpdateDirection(3);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_camera->UpdateDirection(4);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
        m_camera->UpdateDirection(5);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
        m_camera->UpdateDirection(6);

    // Mouse input
    glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);
    // We don't need to update the camera's yaw and pitch as they don't get used in the camera class but if I did this properly then we probably would need them.
    m_camera->setYaw(yaw);
    m_camera->setPitch(pitch);
    m_camera->UpdateCameraFront(cameraFront);

    // Scroll input
    glfwSetScrollCallback(m_window->getWindow(), scroll_callback);
    m_camera->UpdateFOV(fov, m_WINDOW_WIDTH, m_WINDOW_HEIGHT);
}

/*
void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Position: (" << xpos << ":" << ypos << ")";
}*/

unsigned int Engine::getDT()
{
  //long long TimeNowMillis = GetCurrentTimeMillis();
  //assert(TimeNowMillis >= m_currentTimeMillis);
  //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  //m_currentTimeMillis = TimeNowMillis;
  //return DeltaTimeMillis;
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
  //timeval t;
  //gettimeofday(&t, NULL);
  //long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  //return ret;
    return (long long) glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();
    m_graphics->HierarchicalUpdate2(time);
}

// I couldn't figure out how to implement mouse functionality last time so I ended up consulting an online resource as there doesn't seem to be any inforamtion in the textbook (or lectures)
// The resource I used is the following website: (https://learnopengl.com/Getting-started/Camera)
// If this is not allowed then please grade as if mouse functionality & zoom functionality was not implemented
static void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}