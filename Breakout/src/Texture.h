#pragma once

#include <glad/glad.h>

class Texture2D
{
public:
    Texture2D();

    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    void Bind() const;

    unsigned int& GetId() { return m_id; }

    void SetInternalFormat(unsigned int internalFormat) { m_internalFormat = internalFormat; }
    void SetImageFormat(unsigned int imageFormat) { m_imageFormat = imageFormat; }

private:
    unsigned int m_id;
    unsigned m_width, m_height;

    unsigned int m_internalFormat;
    unsigned int m_imageFormat;

    unsigned int m_wrapS;
    unsigned int m_wrapT;
    unsigned int m_filterMin;
    unsigned int m_filterMax;
};
