#include "Enemy.h"

Enemy::Enemy(float x, float y, float speed)
    : x(x), y(y), speed(speed), alive(true) {}

void Enemy::move(float dx, float dy) {
    x += dx;
    y += dy;
}

float Enemy::getX() const { return x; }
float Enemy::getY() const { return y; }
float Enemy::getSpeed() const { return speed; }
void Enemy::setSpeed(float s) { speed = s; }
bool Enemy::isAlive() const { return alive; }
void Enemy::kill() { alive = false; }
