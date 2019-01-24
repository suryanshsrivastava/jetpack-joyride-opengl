#include "player.h"
#include "main.h"
#include "math.h"
#include "timer.h"

Player::Player(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->headradius = 0.1;
    this->rotation = 0;
    speed = 0.1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    GLfloat vertex_buffer_data_head[900]; 
    int sides = 100;
    float angle = (2*M_PIl)/sides;

    for(int i =0; i < sides;i++)
    {
        vertex_buffer_data_head[9*i] = 0.0f;
        vertex_buffer_data_head[9*i + 1] = 0.0f;  
        vertex_buffer_data_head[9*i + 2] = 0.0f;

        vertex_buffer_data_head[9*i + 3] = headradius*cos(i*angle); 
        vertex_buffer_data_head[9*i + 4] = headradius*sin(i*angle) ;
        vertex_buffer_data_head[9*i + 5] = 0.0f; 
        
        vertex_buffer_data_head[9*i + 6] = headradius*cos((i+1)*angle); 
        vertex_buffer_data_head[9*i + 7] = headradius*sin((i+1)*angle); 
        vertex_buffer_data_head[9*i + 8] = 0.0f;
    }

    this->head = create3DObject(GL_TRIANGLES, 3*sides, vertex_buffer_data_head, color, GL_FILL);

    static const GLfloat vertex_buffer_data_body[]={
        0.1f, -0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
        -0.1f, -0.3f, 0.0f,

        0.1f, -0.3f, 0.0f,
        -0.1f, -0.3f, 0.0f,
        0.1f, -0.1f, 0.0f,
    };

    this->body = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data_body, color, GL_FILL); 

    static const GLfloat vertex_buffer_data_legs[]={
        0.0f, -0.3f, 0.0f,
        -0.1f, -0.3f, 0.0f,
        -0.05f, -0.5f, 0.0f,

        0.0f, -0.3f, 0.0f,
        0.1f, -0.3f, 0.0f,
        0.05f, -0.5f, 0.0f,
    };

    this->legs = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data_legs, color, GL_FILL); 
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->head);
    draw3DObject(this->body);
    draw3DObject(this->legs);
}

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::tick_left() {
    this->position.x -= speed;
}

void Player::tick_right() {
    this->position.x += speed;
}

void Player::tick_jump() {
    this->up = 0.2;
    this->position.y += up;
}

void Player::gravity() {
    if (this->position.y <= -1.5f) {
        this->position.y = -1.5f;
    }
    else{
        this->position.y += up;
        this->up -= 0.0163f;
    }
}

bounding_box_t Player::bounding_box() {
    bounding_box_t bbox = { this->position.x, this->position.y, 0.2f, 0.6f };
    return bbox;
}