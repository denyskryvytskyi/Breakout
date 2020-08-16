#include "Game.h"
#include "ResourceManager.h"

SpriteRenderer* Renderer;
GameObject* Player;

Game::Game(unsigned int width, unsigned int height)
    : m_state(EGameState::Active)
    , m_keys()
    , m_width(width)
    , m_height(height)
    , m_currentLevel(0)
{

}

Game::~Game()
{
    delete(Renderer);
}

void Game::Init()
{
    // load shaders 
    ResourceManager::LoadShader("res/shaders/sprite.vs", "res/shaders/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)m_width, (GLfloat)m_height, 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("res/textures/ball_yellow.png", true, "ball");
    //ResourceManager::LoadTexture("textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("res/textures/block.png", false, "block");
    ResourceManager::LoadTexture("res/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("res/textures/paddle.png", true, "paddle");
    // load levels
    GameLevel one;
    one.Load("res/levels/one.lvl", this->m_width, this->m_height / 2);
    GameLevel two;
    two.Load("res/levels/two.lvl", this->m_width, this->m_height / 2);
    GameLevel three;
    three.Load("res/levels/three.lvl", this->m_width, this->m_height / 2);
    GameLevel four;
    four.Load("res/levels/four.lvl", this->m_width, this->m_height / 2);
    this->m_levels.push_back(one);
    this->m_levels.push_back(two);
    this->m_levels.push_back(three);
    this->m_levels.push_back(four);
    this->m_currentLevel = 0;

    glm::vec2 playerPos = glm::vec2(m_width / 2.0 - PLAYER_SIZE.x / 2.0f, m_height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::ProcessInput(float dt)
{
    if (m_state == EGameState::Active)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (m_keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (m_keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= m_width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }
}

void Game::Update(float dt)
{

}

void Game::Render()
{
    //m_spriteRenderer->DrawSprite(ResourceManager::GetTexture("ball"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    // draw level
    this->m_levels[m_currentLevel].Draw(*Renderer);

    Player->Draw(*Renderer);
}