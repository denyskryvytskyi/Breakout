#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    virtual void Draw(SpriteRenderer& renderer);

public:
    glm::vec2 m_Position, m_Size, m_Velocity;
    glm::vec3 m_Color;
    float m_Rotation;
    bool m_IsSolid;
    bool m_Destroyed;

    Texture2D m_Sprite;
};