//
//  VertexBuffer.hpp
//  opengl-course
//
//  Created by Túlio Henrique on 30/01/19.
//  Copyright © 2019 Túlio Henrique. All rights reserved.
//

#ifndef IndexBuffer_h
#define IndexBuffer_h

class IndexBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
    
    inline unsigned int GetCount() const { return m_Count; }
};

#endif /* IndexBuffer_h */
