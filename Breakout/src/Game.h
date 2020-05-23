#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum EGameState
{
    EGameState_Active,
    EGameState_Menu,
    EGameState_Win
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
};

#endif // GAME_H
