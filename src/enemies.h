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
    bool detect_collision(Player Player);
    bool extinguish(Balloons Balloons);
    // bounding_box_t bounding_box();
private:
    VAO *circular;    // How an object looks
    VAO *cylindrical;
};

class FireBeam {
public:
    FireBeam() {}
    FireBeam(float x, float y, color_t color);
    glm::vec3 position;
    float radius;
    float length;
    float seperation;
    float rotation;
    float speed;
    bool moveup;
    void draw(glm::mat4 VP);
    void movement();
    // bool detect_collision(Player Player);
    bounding_box_t bounding_box();
private:
    VAO *circular;    // How an object looks
    VAO *cylindrical;
};

class Boomerang {
public:
    Boomerang() {}
    Boomerang(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float rotation_speed;
    bool back;
    float speed_x;
    float speed_y;
    bool spawn;
    void draw(glm::mat4 VP);
    void movement();
    void set_position(float x, float y);
    bool detect_collision(Player Player);
    bounding_box_t bounding_box();
private:
    VAO *boomerang;    // How an object looks
};

class Viserion {
public:
    Viserion() {}
    Viserion(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float rotation_speed;
    bool back;
    float speed_x;
    float speed_y;
    bool spawn;
    int health;
    void draw(glm::mat4 VP);
    void movement(Player Player);
    void set_position(float x, float y);
    bool detect_collision(Player Player);
    bool execution(Balloons Balloons);
    bounding_box_t bounding_box();
private:
    VAO *dragon;    // How an object looks
};

#endif
