#include "PhysicsThread.h"
#include <chrono>
#include <algorithm>

PhysicsThread::PhysicsThread(Player* player, std::vector<Bullet>* bullets, std::mutex* mtx)
    : player(player), bullets(bullets), mtx(mtx), running(false) {}

PhysicsThread::~PhysicsThread() {
    stop();
}

void PhysicsThread::start() {
    running = true;
    worker = std::thread(&PhysicsThread::run, this);
}

void PhysicsThread::stop() {
    running = false;
    if (worker.joinable())
        worker.join();
}

void PhysicsThread::run() {
    using namespace std::chrono;
    auto lastTime = high_resolution_clock::now();

    while (running) {
        std::this_thread::sleep_for(milliseconds(16)); // ~60 Hz
        auto now = high_resolution_clock::now();
        float dt = duration<float>(now - lastTime).count();
        lastTime = now;

        std::lock_guard<std::mutex> lock(*mtx);
        player->update(dt);

        for (auto& bullet : *bullets)
            bullet.update(dt);

        bullets->erase(std::remove_if(bullets->begin(), bullets->end(),
            [](const Bullet& b){ return !b.isAlive(); }), bullets->end());
    }
}
