#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
    float x, y;
    float speed;
    bool alive;

public:
    Enemy(float x = 5.0f, float y = 5.0f, float speed = 1.0f);

    void move(float dx, float dy);
    float getX() const;
    float getY() const;
    float getSpeed() const;
    void setSpeed(float s);
    bool isAlive() const;
    void kill();
};

#endif
