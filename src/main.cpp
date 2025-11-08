#include "core/Window.h"
#include "core/Shader.h"
#include "core/Texture.h"
#include <GL/glew.h>
#include <iostream>

int main() {
    Window window(800, 600, "Phobia");
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Create shader
    Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    
    // Main game loop
    while (!window.shouldClose()) {
        // Clear screen
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // TODO: Implement game rendering here
        
        // Swap buffers and poll events
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}
