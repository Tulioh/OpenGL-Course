//
//  Renderer.cpp
//  opengl-course
//
//  Created by Túlio Henrique on 30/01/19.
//  Copyright © 2019 Túlio Henrique. All rights reserved.
//

#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
        " " << file << ":" << line << std::endl;
        return false;
    }
    
    return true;
}
