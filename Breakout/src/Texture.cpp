#include "Texture.h"

Texture2D::Texture2D()
    : m_width(0)
    , m_height(0)
    , m_internalFormat(GL_RGB)
    , m_imageFormat(GL_RGB)
    , m_wrapS(GL_REPEAT)
    , m_wrapT(GL_REPEAT)
    , m_filterMin(GL_LINEAR)
    , m_filterMax(GL_LINEAR)
{
    glGenTextures(1, &m_id);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}
