#include "Raycaster.h"
#include <cmath>

Raycaster::Raycaster() : mapWidth(16), mapHeight(16) {
    int worldMap[16][16] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    for (int y = 0; y < mapHeight; y++)
        for (int x = 0; x < mapWidth; x++)
            map[y][x] = worldMap[y][x];
}

void Raycaster::init(SDL_Renderer* renderer) {}

void Raycaster::render(SDL_Renderer* renderer, const Player& player, const std::vector<Enemy>& enemies, const std::vector<Bullet>& bullets) {
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    for (int x = 0; x < w; ++x) {
        float cameraX = 2 * x / float(w) - 1;
        float rayDirX = std::cos(player.angle) + cameraX * std::sin(player.angle);
        float rayDirY = std::sin(player.angle) - cameraX * std::cos(player.angle);

        int mapX = int(player.x);
        int mapY = int(player.y);

        float sideDistX, sideDistY;
        float deltaDistX = std::abs(1 / rayDirX);
        float deltaDistY = std::abs(1 / rayDirY);
        float perpWallDist;
        int stepX, stepY, hit = 0, side;

        if (rayDirX < 0) { stepX = -1; sideDistX = (player.x - mapX) * deltaDistX; }
        else { stepX = 1; sideDistX = (mapX + 1.0 - player.x) * deltaDistX; }
        if (rayDirY < 0) { stepY = -1; sideDistY = (player.y - mapY) * deltaDistY; }
        else { stepY = 1; sideDistY = (mapY + 1.0 - player.y) * deltaDistY; }

        while (!hit) {
            if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
            else { sideDistY += deltaDistY; mapY += stepY; side = 1; }
            if (map[mapY][mapX] > 0) hit = 1;
        }

        if (side == 0) perpWallDist = (sideDistX - deltaDistX);
        else perpWallDist = (sideDistY - deltaDistY);

        int lineHeight = int(h / perpWallDist);
        int drawStart = -lineHeight / 2 + h / 2;
        int drawEnd = lineHeight / 2 + h / 2;
        SDL_SetRenderDrawColor(renderer, side ? 150 : 200, 150, 150, 255);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}
