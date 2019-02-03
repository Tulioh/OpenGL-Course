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
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    GLFWwindow* window = glfwCreateWindow(940, 560, "Hello World", NULL, NULL);
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
    
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    
    // Vertex to create an rectangle
    float positions[] = {
        -50.0f, -50.0f, 0.0f, 0.0f, // 0
         50.0f, -50.0f, 1.0f, 0.0f, // 1
         50.0f,  50.0f, 1.0f, 1.0f, // 2
        -50.0f,  50.0f, 0.0f, 1.0f // 3
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    // Create and bind vao
    VertexArray va;
    // Create the vertex buffer data and bind it to draw on screen
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    
    // Links the buffer with the vao
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    
    // Tell OpenGL how each Vertex are linked to create the rectangle
    IndexBuffer ib(indices, sizeof(indices));
    
    glm::mat4 projection = glm::ortho<float>(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    
    // Load shader source and load into program
    Shader shader("resources/shaders/Basic.shader");
    shader.Bind();
    
    Texture texture("resources/textures/google-logo.png");
    texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);
    
    // Reset everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    
    Renderer renderer;
    
    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);
    
    float r = 0.0f;
    float increment = 0.05f;
    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        renderer.Clear();
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = projection * view * model;
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }
        
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = projection * view * model;
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }
        
        // Logic to change color each frame
        if (r > 1.0f) {
            increment = -0.05f;
        } else if (r < 0.0f) {
            increment = 0.05f;
        }
        r += increment;
        
        {
            ImGui::Begin("Transform");
            ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
            ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
            ImGui::End();
        }
        
        // ImGui Render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // GLFW specific things to clear buffers and get input events
        glfwSwapBuffers(window);
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();
    return 0;
}

