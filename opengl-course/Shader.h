//
//  Shader.hpp
//  opengl-course
//
//  Created by Túlio Henrique on 31/01/19.
//  Copyright © 2019 Túlio Henrique. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include <string>
#include <unordered_map>

struct ShaderProgramSources {
    std::string VertexSources;
    std::string FragmentSources;
};

class Shader {
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, unsigned int> m_UniformLocationCache;
public:
    Shader(const std::string& filename);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    // Set uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    
private:
    ShaderProgramSources ParseShader(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    
    unsigned int GetUniformLocation(const std::string& name);
};

#endif /* Shader_h */
