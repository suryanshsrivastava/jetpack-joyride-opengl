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

#endif