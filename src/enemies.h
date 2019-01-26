#include "main.h"
#include "player.h"

#ifndef ENEMIES_H
#define ENEMIES_H

class FireLine {
public:
    FireLine() {}
    FireLine(float x, float y, double angle, color_t color);
    glm::vec3 position;
    float radius;
    float length;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool detect_collision(Player Player);
    bounding_box_t bounding_box();
private:
    VAO *circular;    // How an object looks
    VAO *cylindrical;
};

class FireBeam {
public:
    FireBeam() {}
    FireBeam(float x, float y, double angle, color_t color);
    glm::vec3 position;
    float radius;
    float length;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool detect_collision(Player Player);
    bounding_box_t bounding_box();
private:
    VAO *circular;    // How an object looks
    VAO *cylindrical;
};

#endif
