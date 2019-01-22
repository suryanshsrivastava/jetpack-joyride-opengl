#include "main.h"

#ifndef BALL_H
#define BALL_H


class Player {
public:
    Player() {}
    Player(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void tick_left();
    void tick_right();
    void tick_jump();
    void gravity();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
