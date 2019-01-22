#include "main.h"

#ifndef BALL_H
#define BALL_H


class Player {
public:
    Player() {}
    Player(float x, float y, color_t color);
    glm::vec3 position;
    float headradius;
    float width;
    float height;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void tick_left();
    void tick_right();
    void tick_jump();
    void gravity();
    bounding_box_t bounding_box();
    double speed;
    double up;
private:
    VAO *head;
    VAO *body;
    VAO *legs;
};

#endif // BALL_H
