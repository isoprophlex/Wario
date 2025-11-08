#include "AIThread.h"
#include <chrono>
#include <cmath>
#include <algorithm>
#include <random>

AIThread::AIThread(Player* player, std::vector<Enemy>* enemies, std::vector<Bullet>* bullets, std::mutex* mtx)
    : player(player), enemies(enemies), bullets(bullets), mtx(mtx), running(false) {}

AIThread::~AIThread() {
    stop();
}

void AIThread::start() {
    running = true;
    worker = std::thread(&AIThread::run, this);
}

void AIThread::stop() {
    running = false;
    if (worker.joinable())
        worker.join();
}

void AIThread::run() {
    using namespace std::chrono;
    auto lastTime = high_resolution_clock::now();
    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<float> shootChance(0.0f, 1.0f);

    while (running) {
        std::this_thread::sleep_for(milliseconds(50));
        auto now = high_resolution_clock::now();
        float dt = duration<float>(now - lastTime).count();
        lastTime = now;

        std::lock_guard<std::mutex> lock(*mtx);

        for (auto& enemy : *enemies) {
            if (!enemy.isAlive()) continue;

            float dx = player->x - enemy.getX();
            float dy = player->y - enemy.getY();
            float dist = std::sqrt(dx*dx + dy*dy);
            if (dist < 0.1f) continue;

            dx /= dist; dy /= dist;
            enemy.move(dx * enemy.getSpeed() * dt, dy * enemy.getSpeed() * dt);

            if (dist < 8.0f && shootChance(rng) < 0.05f) {
                float angle = std::atan2(dy, dx);
                bullets->emplace_back(enemy.getX(), enemy.getY(), angle, 8.0f);
            }
        }
        enemies->erase(std::remove_if(enemies->begin(), enemies->end(),
            [](const Enemy& e){ return !e.isAlive(); }), enemies->end());
    }
}
