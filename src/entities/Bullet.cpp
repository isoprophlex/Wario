#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float x, float y, float angle, float speed)
    : x(x), y(y), angle(angle), speed(speed), alive(true) {
    dx = std::cos(angle);
    dy = std::sin(angle);
}


void Bullet::update(float dt) {
    x += dx * speed * dt;
    y += dy * speed * dt;

    if (x < 0 || y < 0 || x > 50 || y > 50)
        alive = false;
}



float Bullet::getX() const { return x; }
float Bullet::getY() const { return y; }
bool Bullet::isAlive() const { return alive; }
void Bullet::kill() { alive = false; }
