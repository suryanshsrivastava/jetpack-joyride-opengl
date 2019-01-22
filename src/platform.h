#include "main.h"

#ifndef PLATFORM_H
#define PLATFORM_H


class Platform {
public:
    Platform() {}
    Platform(float x, float y, float width, float height, color_t color);
    glm::vec3 position;
    float width;
    float height;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // PLATFORM_H

// class Ground {
// public:
//     Ground() {}
//     Ground(float x, float y,float width,float height,float rotation,float xpeed, color_t color);
//     glm::vec3 position;
//     float speed;
//     float rotation;
//     float width;
//     float height;
//     void draw(glm::mat4 VP);
//     void tick();
//     void set_position(float x, float y);
//     bool detect_collision(Ball ball);
//     bounding_box_t bounding_box();
// private:
//     VAO *object;    // How an object looks 
// };

// class Plank :public Ground{
// public:
//     Plank(): Ground() {}
//     Plank(float x, float y,float width,float height,float rotation,float xpeed, color_t color) : Ground(x,y,width,height,rotation,xpeed,color){};
//     bool detect_collision(Ball ball); 
// };

// #endif