#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SpriteRenderer.h"

enum class EGameState
{
    Active,
    Menu,
    Win
};

class Game
{
public:
    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();

    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    EGameState GetState()
    {
        return m_state;
    };

private:
    EGameState m_state;
    bool m_keys[1024];
    unsigned m_width, m_height;

    SpriteRenderer* m_spriteRenderer;
};

#endif // GAME_H
