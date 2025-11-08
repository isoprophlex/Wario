#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SDL2/SDL.h>
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Bullet.h"
#include <vector>

class Raycaster {
public:
    Raycaster();
    void init(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer, const Player& player, const std::vector<Enemy>& enemies, const std::vector<Bullet>& bullets);

private:
    int mapWidth, mapHeight;
    int map[16][16];
};

#endif
