#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();
    
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    GLFWwindow* getHandle() const { return window; }
    
private:
    GLFWwindow* window;
    int width, height;
};