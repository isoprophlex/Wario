#include "GameEngine.h"
#include <iostream>
#include <cmath>
#include "Bullet.h"
#include <cmath>

GameEngine::GameEngine(int difficulty)
    : window(nullptr), renderer(nullptr), running(false),
      physicsThread(nullptr), aiThread(nullptr), raycaster(),
      lastTime(0), difficulty(difficulty)
{
    // setear player defaults según dificultad
    if (difficulty == 0) player.health = 150;
    else if (difficulty == 1) player.health = 100;
    else player.health = 60;
}

GameEngine::~GameEngine() {
    shutdown();
}

bool GameEngine::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        std::cerr << "Error inicializando SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Shooter 2.5D - Franco Edition",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1024, 768, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Error creando ventana: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creando renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    // Crear enemigos según dificultad
    int base = 3 + difficulty * 4; // Fac:3, Norm:7, Dif:11
    enemies.clear();
    for (int i = 0; i < base; ++i) {
        float ex = 4.0f + (i % 5) * 2.0f;
        float ey = 4.0f + (i / 5) * 2.0f;
        Enemy e(ex, ey, 1.0f + difficulty * 0.4f);
        enemies.push_back(e);
    }

    raycaster.init(renderer);

    physicsThread = new PhysicsThread(&player, &bullets, &mtx);
    aiThread = new AIThread(&player, &enemies, &bullets, &mtx);

    physicsThread->start();
    aiThread->start();

    lastTime = SDL_GetPerformanceCounter();
    running = true;
    return true;
}

void GameEngine::run() {
    while (running) {
      	std::cout << "Loop tick" << std::endl;
        Uint64 current = SDL_GetPerformanceCounter();
        float deltaTime = (float)(current - lastTime) / SDL_GetPerformanceFrequency();
        lastTime = current;

        processInput(deltaTime);
        update(deltaTime);
        player.updatePhysics(deltaTime);
        render();

        SDL_Delay(16);
    }
}



void GameEngine::processInput(float dt) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    float speed = 3.0f;

    if (state[SDL_SCANCODE_W])
        player.move(std::cos(player.angle) * speed * dt, std::sin(player.angle) * speed * dt);
    if (state[SDL_SCANCODE_S])
        player.move(-std::cos(player.angle) * speed * dt, -std::sin(player.angle) * speed * dt);
    if (state[SDL_SCANCODE_A])
        player.angle -= 2.5f * dt;
    if (state[SDL_SCANCODE_D])
        player.angle += 2.5f * dt;

    // salto con barra
    if (state[SDL_SCANCODE_SPACE])
        player.jump();

    // disparo con LCTRL o ENTER
    if (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RETURN]) {
        float bulletStartX = player.x + std::cos(player.angle) * 0.3f;
        float bulletStartY = player.y + std::sin(player.angle) * 0.3f;
        bullets.emplace_back(bulletStartX, bulletStartY, player.angle, 8.0f);
    }
}



void GameEngine::update(float dt) {
    player.update(dt);

    // Actualizar balas
    for (auto& b : bullets)
        b.update(dt);

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
                       [](const Bullet& b) { return !b.isAlive(); }),
        bullets.end());
}

void GameEngine::render() {
    if (!renderer) return;

    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    std::lock_guard<std::mutex> lock(mtx);
    raycaster.render(renderer, player, enemies, bullets);

    // HUD simple: barra de HP
    int hpW = int((player.health / 150.0f) * 200.0f);
    SDL_Rect hpBg = {20, 20, 204, 18};
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &hpBg);
    SDL_Rect hpRect = {22, 22, std::max(0, hpW), 14};
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    SDL_RenderFillRect(renderer, &hpRect);
// --- Dibuja la mira (crosshair) ---
    int cx = 1024 / 2;
    int cy = 768 / 2;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, cx - 10, cy, cx + 10, cy); // línea horizontal
    SDL_RenderDrawLine(renderer, cx, cy - 10, cx, cy + 10); // línea vertical
// --- Dibujar balas ---
    for (auto& b : bullets) {
        if (!b.isAlive()) continue;
        int bx = int(b.getX() * 64);  // escala mundo→pantalla (ajustá según Raycaster)
        int by = int(b.getY() * 64);
        SDL_Rect bulletRect = {bx - 2, by - 2, 4, 4};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &bulletRect);
}

    SDL_RenderPresent(renderer);
}

void GameEngine::shutdown() {
    running = false;
    if (physicsThread) { physicsThread->stop(); delete physicsThread; physicsThread = nullptr; }
    if (aiThread) { aiThread->stop(); delete aiThread; aiThread = nullptr; }

    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window) { SDL_DestroyWindow(window); window = nullptr; }
    SDL_Quit();
}
