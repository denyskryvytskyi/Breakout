#ifndef GAMELEVEL_H
#define GAMELEVEL_H

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

private:
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);

private:
    std::vector<GameObject> m_bricks;
};

#endif // GAMELEVEL_H