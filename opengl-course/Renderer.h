//
//  Renderer.hpp
//  opengl-course
//
//  Created by Túlio Henrique on 30/01/19.
//  Copyright © 2019 Túlio Henrique. All rights reserved.
//

#ifndef Renderer_h
#define Renderer_h

#include <GL/glew.h>
#include "signal.h"

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#endif /* Renderer_h */
