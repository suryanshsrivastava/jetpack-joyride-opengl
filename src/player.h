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
    float shield;
    bool flying;
    bool tunnel;
    float gravityval;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void tick_left();
    void tick_right();
    void tick_jump();
    void tick_jump_phy();
    void gravity();
    bounding_box_t bounding_box();
    double speed;
    double up;
private:
    VAO *head;
    VAO *body;
    VAO *legs;
    VAO *jet;
    VAO *shielded;
};

class Balloons {
public:
    Balloons() {}
    Balloons(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float radius;
    float fired;
    float speed_x;
    float speed_y;
    bool moveup;
    void draw(glm::mat4 VP);
    void fire(Player Player);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *water;    // How an object looks
};
#endif // BALL_H
