#include "Game.h"
#include "ResourceManager.h"

Game::Game(unsigned int width, unsigned int height)
    : m_state(EGameState::Menu)
    , m_keys()
    , m_width(width)
    , m_height(height)
    , m_spriteRenderer()
{

}

Game::~Game()
{
    delete(m_spriteRenderer);
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
    m_spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("res/textures/ball_yellow.png", true, "ball");
}

void Game::ProcessInput(float dt)
{

}

void Game::Update(float dt)
{

}

void Game::Render()
{
    m_spriteRenderer->DrawSprite(ResourceManager::GetTexture("ball"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}