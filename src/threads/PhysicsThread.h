#ifndef PHYSICSTHREAD_H
#define PHYSICSTHREAD_H

#include <thread>
#include <atomic>
#include <mutex>
#include "../entities/Player.h"
#include "../entities/Bullet.h"
#include <vector>

class PhysicsThread {
public:
    PhysicsThread(Player* player, std::vector<Bullet>* bullets, std::mutex* mtx);
    ~PhysicsThread();

    void start();
    void stop();

private:
    void run();
    std::thread worker;
    std::atomic<bool> running;
    Player* player;
    std::vector<Bullet>* bullets;
    std::mutex* mtx;
};

#endif
