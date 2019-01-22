#include "main.h"
#include "player.h"

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
    bool detect_collision(Player Player);
    // bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // PLATFORM_H
