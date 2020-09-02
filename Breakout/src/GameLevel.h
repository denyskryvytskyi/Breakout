#pragma once

#include <vector>

#include "GameObject.h"
#include "SpriteRenderer.h"

class GameLevel
{
public:
    GameLevel();

    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    void Draw(SpriteRenderer& renderer);

    bool IsCompleted();

public:
    std::vector<GameObject> m_Bricks;

private:
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);

};