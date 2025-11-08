#ifndef BULLET_H
#define BULLET_H

#include <cmath>

class Bullet {
public:
    float x, y;     // posición
    float dx, dy;
    float angle;    // dirección de disparo
    float speed;    // velocidad
    bool alive;     // sigue activa o no

    Bullet(float x, float y, float angle, float speed = 5.0f);

    void update(float dt);

    float getX() const;
    float getY() const;
    bool isAlive() const;
    void kill();
};

#endif
