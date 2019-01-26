#include "main.h"

#ifndef OBJECT_H
#define OBJECT_H

class Coins {
public:
    Coins() {}
    Coins(float x, float y, color_t color);
    glm::vec3 position;
    float radius;
    float rotation;
    bool collected;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;    // How an object looks 
};

class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void set_rotation(float rotation);
    // bool detect_collision(Ball ball);
    bounding_box_t bounding_box();
private:
    VAO *handle;    // How an object looks
    VAO *poles; 
};

#endif