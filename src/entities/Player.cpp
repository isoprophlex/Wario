#include "Player.h"

Player::Player(float x, float y)
    : x(x), y(y), z(0), vx(0), vy(0), vz(0),
      angle(0), health(100), onGround(true) {}

void Player::move(float dx, float dy) {
    x += dx;
    y += dy;
}

void Player::jump() {
    if (onGround) {
        vz = 5.0f;      // impulso hacia arriba
        onGround = false;
    }
}

void Player::update(float dt) {
    updatePhysics(dt);
}

void Player::updatePhysics(float dt) {
    if (!onGround) {
        vz -= 9.8f * dt;  // gravedad
        z += vz * dt;
        if (z <= 0) {
            z = 0;
            vz = 0;
            onGround = true;
        }
    }
}
