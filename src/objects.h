#include "main.h"
#include "player.h"

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
    bool spawn;
    bool spawn_x;
    float deceleration;
    void effect(Player &Player);
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *handle;    // How an object looks
    VAO *poles; 
};

// Special Flying objects and powerups
class Shield {
public:
    Shield() {}
    Shield(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float speed;
    bool moveup;
    bool collected;
    void draw(glm::mat4 VP);
    void movement();
    // bool detect_collision(Player Player);
    bounding_box_t bounding_box();
private:
    VAO *shield;    // How an object looks 
};

class Health {
public:
    Health() {}
    Health(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float radius;
    float speed;
    bool moveup;
    bool collected;
    void draw(glm::mat4 VP);
    void movement();
    // bool detect_collision(Player Player);
    bounding_box_t bounding_box();
private:
    VAO *heart;    // How an object looks
    VAO *invertedTriangle; 
};

class Ring {
public:
    Ring() {}
    Ring(float x, float y, color_t color);
    glm::vec3 position;
    float radius;
    float rotation;
    bool collected;
    bool reversetunnel(Player Player);
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;    // How an object looks 
};
#endif