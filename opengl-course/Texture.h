//
//  Texture.hpp
//  opengl-course
//
//  Created by Túlio Henrique on 02/02/19.
//  Copyright © 2019 Túlio Henrique. All rights reserved.
//

#ifndef Texture_h
#define Texture_h

#include <string>

class Texture {
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
public:
    Texture(const std::string& path);
    ~Texture();
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    
    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};

#endif /* Texture_h */
