#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include "window.h"
#include "graphics.h"

static void cursorPositionCallBack(GLFWwindow*, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// These variables need to be here for cursorPositionCallBack to use. There's probably a better way to do this but I couldn't figure it out/
static bool firstMouse;
static float lastX, lastY, yaw, pitch;
static glm::vec3 cameraFront;
static float fov;

class Engine
{
  public:
    Engine(const char*  name, int width, int height);
    
    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);
    //static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
  
  private:
    // Window related variables
    Window *m_window;    
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;

    Graphics *m_graphics;
    Camera* m_camera;

    bool m_running;

};

#endif // ENGINE_H

