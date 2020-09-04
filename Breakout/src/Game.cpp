#include "Game.h"
#include "ResourceManager.h"
#include "BallObject.h"

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;

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
    delete Renderer;
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
    ResourceManager::LoadTexture("res/textures/awesomeface.png", true, "ball");
    //ResourceManager::LoadTexture("textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("res/textures/block.png", false, "block");
    ResourceManager::LoadTexture("res/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("res/textures/paddle.png", true, "paddle");
    // load levels
    GameLevel one;
    one.Load("res/levels/one.lvl", m_width, m_height / 2);
    GameLevel two;
    two.Load("res/levels/two.lvl", m_width, m_height / 2);
    GameLevel three;
    three.Load("res/levels/three.lvl", m_width, m_height / 2);
    GameLevel four;
    four.Load("res/levels/four.lvl", m_width, m_height / 2);
    m_levels.push_back(one);
    m_levels.push_back(two);
    m_levels.push_back(three);
    m_levels.push_back(four);
    m_currentLevel = 0;

    // player
    glm::vec2 playerPos = glm::vec2(m_width / 2.0 - PLAYER_SIZE.x / 2.0f, m_height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    // ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));
}

void Game::ProcessInput(float dt)
{
    if (m_state == EGameState::Active)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (m_keys[GLFW_KEY_A])
        {
            if (Player->m_Position.x >= 0.0f)
            {
                Player->m_Position.x -= velocity;
                if (Ball->m_Stuck)
                    Ball->m_Position.x -= velocity;
            }
        }
        if (m_keys[GLFW_KEY_D])
        {
            if (Player->m_Position.x <= m_width - Player->m_Size.x)
            {
                Player->m_Position.x += velocity;
                if (Ball->m_Stuck)
                    Ball->m_Position.x += velocity;
            }
        }
        if (m_keys[GLFW_KEY_SPACE])
            Ball->m_Stuck = false;
    }
}

void Game::Update(float dt)
{
    Ball->Move(dt, m_width);

    DoCollisions();

    if (Ball->m_Position.y >= m_height)
    {
        ResetLevel();
        ResetPlayer();
    }
}

void Game::Render()
{
    if (m_state == EGameState::Active)
    {
        m_levels[m_currentLevel].Draw(*Renderer);

        Player->Draw(*Renderer);
        Ball->Draw(*Renderer);
    }
}

// collision detection
bool CheckCollision(GameObject& one, GameObject& two);
Collision CheckCollision(BallObject& one, GameObject& two);
EDirection VectorDirection(glm::vec2 target);

void Game::DoCollisions()
{
    for (GameObject& box : m_levels[m_currentLevel].m_Bricks)
    {
        if (!box.m_Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision))
            {
                if (!box.m_IsSolid)
                    box.m_Destroyed = true;
                // collision resolution
                EDirection dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == EDirection::Left || dir == EDirection::Right) // horizontal collision
                {
                    Ball->m_Velocity.x = -Ball->m_Velocity.x; // reverse horizontal velocity
                    // relocate
                    float penetration = Ball->m_Radius - std::abs(diff_vector.x);
                    if (dir == EDirection::Left)
                        Ball->m_Position.x += penetration; // move ball to right
                    else
                        Ball->m_Position.x -= penetration; // move ball to left;
                }
                else // vertical collision
                {
                    Ball->m_Velocity.y = -Ball->m_Velocity.y; // reverse vertical velocity
                    // relocate
                    float penetration = Ball->m_Radius - std::abs(diff_vector.y);
                    if (dir == EDirection::Up)
                        Ball->m_Position.y -= penetration; // move ball back up
                    else
                        Ball->m_Position.y += penetration; // move ball back down
                }
            }
        }
    }
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->m_Stuck && std::get<0>(result))
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = Player->m_Position.x + Player->m_Size.x / 2.0f;
        float distance = (Ball->m_Position.x + Ball->m_Radius) - centerBoard;
        float percentage = distance / (Player->m_Size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->m_Velocity;
        Ball->m_Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->m_Velocity.y = -1.0f * abs(Ball->m_Velocity.y);
        Ball->m_Velocity = glm::normalize(Ball->m_Velocity) * glm::length(oldVelocity);
    }
}

void Game::ResetLevel()
{
    if (m_currentLevel == 0)
        m_levels[0].Load("res/levels/one.lvl", m_width, m_height / 2);
    else if (m_currentLevel == 1)
        m_levels[1].Load("levels/two.lvl", m_width, m_height / 2);
    else if (m_currentLevel == 2)
        m_levels[2].Load("levels/three.lvl", m_width, m_height / 2);
    else if (m_currentLevel == 3)
        m_levels[3].Load("levels/four.lvl", m_width, m_height / 2);
}

void Game::ResetPlayer()
{
    Player->m_Size = PLAYER_SIZE;
    Player->m_Position = glm::vec2(m_width / 2.0f - PLAYER_SIZE.x / 2.0f, m_height - PLAYER_SIZE.y);
    Ball->Reset(Player->m_Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

// AABB - AABB collision
bool CheckCollision(GameObject& first, GameObject& second)
{
    // collision x-axis?
    bool collisionX = first.m_Position.x + first.m_Size.x >= second.m_Position.x &&
        second.m_Position.x + second.m_Size.x >= first.m_Position.x;
    // collision y-axis?
    bool collisionY = first.m_Position.y + first.m_Size.y >= second.m_Position.y &&
        second.m_Position.y + second.m_Size.y >= first.m_Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

// AABB - Circle collision
Collision CheckCollision(BallObject& first, GameObject& second)
{
    glm::vec2 center(first.m_Position + first.m_Radius);

    glm::vec2 aabbHalfExtens(second.m_Size.x / 2.0f, second.m_Size.y / 2.0f);
    glm::vec2 aabbCenter(second.m_Position.x + aabbHalfExtens.x, second.m_Position.y + aabbHalfExtens.y);

    glm::vec2 difference = center - aabbCenter;
    glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtens, aabbHalfExtens);

    glm::vec2 closest = aabbCenter + clamped;
    difference = closest - center;

    if (glm::length(difference) <= first.m_Radius)
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, EDirection::Up, glm::vec2(0.0f, 0.0f));
}

EDirection VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };

    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (EDirection)best_match;
}
