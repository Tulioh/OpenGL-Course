#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void) {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }
    
    // Configure OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Create the Window
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    // Attach window to OpenGL context
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);
    
    if (glewInit() != GLEW_OK) {
        std::cout << "Error! glew it not ok" << std::endl;
    }
    
    std::cout << "GL Version " << glGetString(GL_VERSION) << std::endl;
    
    // Vertex to create an rectangle
    float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f, // 3
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    // Create and bind vao
    VertexArray va;
    // Create the vertex buffer data and bind it to draw on screen
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    
    // Links the buffer with the vao
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    
    // Tell OpenGL how each Vertex are linked to create the rectangle
    IndexBuffer ib(indices, sizeof(indices));
    
    // Load shader source and load into program
    Shader shader("resources/shaders/Basic.shader");
    shader.Bind();

    // get u_Color from shader to change dynamically
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    
    // Reset everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    
    Renderer renderer;
    
    float r = 0.0f;
    float increment = 0.05f;
    
    while(!glfwWindowShouldClose(window)) {
        renderer.Clear();
        
        // Use shader program to change the u_Color
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        
        renderer.Draw(va, ib, shader);
        
        // Logic to change color each frame
        if (r > 1.0f) {
            increment = -0.05f;
        } else if (r < 0.0f) {
            increment = 0.05f;
        }
        r += increment;
        
        // GLFW specific things to clear buffers and get input events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

