#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>

class Player {
public:
    float x, y, z;      // posición (z = altura)
    float vx, vy, vz;   // velocidades
    float angle;        // dirección en radianes
    float health;
    bool onGround;

    Player(float x = 2.0f, float y = 2.0f);

    void move(float dx, float dy);
    void jump();
    void update(float dt);
    void updatePhysics(float dt);
};

#endif
