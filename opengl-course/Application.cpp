#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "signal.h"

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
        " " << file << ":" << line << std::endl;
        return false;
    }
    
    return true;
}

struct ShaderProgramSources {
    std::string VertexSources;
    std::string FragmentSources;
};

static ShaderProgramSources ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);
    
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    
    std::string line;
    std::stringstream stringStream[2];
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            stringStream[(int)type] << line << '\n';
        }
    }
    
    return { stringStream[0].str(), stringStream[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::string shaderType = (type == GL_FRAGMENT_SHADER) ? "fragment" : "vertex";
        std::cout << "Failed to compile " << shaderType << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

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
    
    glfwSwapInterval(1);
    
    // Attach window to OpenGL context
    glfwMakeContextCurrent(window);
    
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
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Create the vertex buffer data and bind it to draw on screen
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
    
    // Links the buffer with the vao
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    // Tell OpenGL how each Vertex are linked to create the rectangle
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Load shader source and load into program
    ShaderProgramSources source = ParseShader("resources/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSources, source.FragmentSources);
    glUseProgram(shader);
    
    // get u_Color from shader to change dynamically
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);
    
    // Reset everything
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    float r = 0.0f;
    float increment = 0.05f;
    
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Use shader program to change the u_Color
        glUseProgram(shader);
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
        
        // Bind vao and ibo
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        
        // Send draw command
        GLCALL(glDrawElements(GL_TRIANGLE_STRIP, sizeof(indices), GL_UNSIGNED_INT, nullptr));
        
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
    
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
