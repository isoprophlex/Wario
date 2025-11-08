#ifndef AITHREAD_H
#define AITHREAD_H

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../entities/Bullet.h"

class AIThread {
public:
    AIThread(Player* player, std::vector<Enemy>* enemies, std::vector<Bullet>* bullets, std::mutex* mtx);
    ~AIThread();

    void start();
    void stop();

private:
    void run();

    std::thread worker;
    std::atomic<bool> running;

    Player* player;
    std::vector<Enemy>* enemies;
    std::vector<Bullet>* bullets;
    std::mutex* mtx;
};

#endif
