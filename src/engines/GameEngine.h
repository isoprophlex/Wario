#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <vector>
#include <mutex>
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Bullet.h"
#include "../threads/PhysicsThread.h"
#include "../threads/AIThread.h"
#include "../render/Raycaster.h"

class GameEngine {
public:
    explicit GameEngine(int difficulty = 1);
    ~GameEngine();

    bool init();
    void run();
    void shutdown();

private:
    void processInput(float dt);
    void update(float dt);
    void render();

    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    std::mutex mtx;

    PhysicsThread* physicsThread;
    AIThread* aiThread;
    Raycaster raycaster;

    Uint64 lastTime;
    int difficulty;
};

#endif
