#include "objects.h"
#include "main.h"
#include "player.h"

Coins::Coins(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius=0.1;
    GLfloat vertex_buffer_data_head[900]; 
    int sides = 100;
    float angle = (2*M_PIl)/sides;

    for(int i =0; i < sides;i++)
    {
        vertex_buffer_data_head[9*i] = 0.0f;
        vertex_buffer_data_head[9*i + 1] = 0.0f;  
        vertex_buffer_data_head[9*i + 2] = 0.0f;

        vertex_buffer_data_head[9*i + 3] = radius*cos(i*angle); 
        vertex_buffer_data_head[9*i + 4] = radius*sin(i*angle) ;
        vertex_buffer_data_head[9*i + 5] = 0.0f;

        vertex_buffer_data_head[9*i + 6] = radius*cos((i+1)*angle); 
        vertex_buffer_data_head[9*i + 7] = radius*sin((i+1)*angle); 
        vertex_buffer_data_head[9*i + 8] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 3*sides, vertex_buffer_data_head, color, GL_FILL);
}

void Coins::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Coins::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Coins::bounding_box() {
    bounding_box_t bbox = { this->position.x, this->position.y, 0.2f, 0.2f };
    return bbox;
}

Magnet::Magnet(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->spawn=true;
    
    static const GLfloat vertex_buffer_data_vertical[]={
        0.0f, 0.6f, 0.0f,
        0.0f, -0.6f, 0.0f,
        0.2f, -0.6f, 0.0f,

        0.0f, 0.6f, 0.0f,
        0.2f, 0.6f, 0.0f,
        0.2f, -0.6f, 0.0f,   
    };

    this->handle = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data_vertical, color, GL_FILL);

    static const GLfloat vertex_buffer_data_horiontal[]={
        0.2f, 0.6f, 0.0f,
        0.6f, 0.6f, 0.0f,
        0.6f, 0.3f, 0.0f,

        0.2f, 0.6f, 0.0f,
        0.2f, 0.3f, 0.0f,
        0.6f, 0.3f, 0.0f,

        0.2f, -0.6f, 0.0f,
        0.6f, -0.6f, 0.0f,
        0.6f, -0.3f, 0.0f,

        0.2f, -0.6f, 0.0f,
        0.2f, -0.3f, 0.0f,
        0.6f, -0.3f, 0.0f,
    };
    this->poles = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data_horiontal, COLOR_BLACK, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->handle);
    draw3DObject(this->poles);
}

void Magnet::effect(Player &Player) {
    this->deceleration = 0.05;
    Player.position.x -= this->deceleration;
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

Shield::Shield(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed=0.025;
    this->collected=false;
    this->moveup=false;
    
    static const GLfloat vertex_buffer_data[]={
        0.125f, 0.25f, 0.0f,
       -0.125f, 0.25f, 0.0f,
        0.0f  , 0.5f , 0.0f,
        
        0.125f, 0.25f, 0.0f,
        -0.125f, 0.25f, 0.0f,
        0.0f, -0.5f, 0.0f,
    };
    this->shield = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Shield::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->shield);
}

void Shield::movement() {
    if(moveup){
        this->position.y += this->speed;
        if(this->position.y >= 4)
            moveup=false;
    }
    else
    {
        this->position.y -= this->speed;
        if(this->position.y < -1.5f)
            moveup=true;
    }
}

bounding_box_t Shield::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.25f, 1.0f };
    return bbox;
}

Health::Health(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius=0.1f;
    this->speed=0.025;
    this->collected=false;
    this->moveup=false;
    
    GLfloat vertex_buffer_data_circular[2000];
    int sides = 100;
    float theta = (2*M_PIl)/sides;
    
    for(int i = 0; i < sides; i++)
    {
        vertex_buffer_data_circular[9*i] = 0.0f;
        vertex_buffer_data_circular[9*i + 1] = 0.0f;  
        vertex_buffer_data_circular[9*i + 2] = 0.0f;

        vertex_buffer_data_circular[9*i + 3] = radius*cos(i*theta); 
        vertex_buffer_data_circular[9*i + 4] = radius*sin(i*theta) ;
        vertex_buffer_data_circular[9*i + 5] = 0.0f;

        vertex_buffer_data_circular[9*i + 6] = radius*cos((i+1)*theta); 
        vertex_buffer_data_circular[9*i + 7] = radius*sin((i+1)*theta); 
        vertex_buffer_data_circular[9*i + 8] = 0.0f;
    }

    static const GLfloat vertex_buffer_data_invertedTriangle[]={
        -radius, 0.0f, 0.0f,
        3*radius, 0.0f, 0.0f,
        radius, -0.4f, 0.0f,
    };

    for(int j = 100; j < sides+101; j++)
    {
        vertex_buffer_data_circular[9*j] = 0.0f + radius*2;
        vertex_buffer_data_circular[9*j + 1] = 0.0f;  
        vertex_buffer_data_circular[9*j + 2] = 0.0f;

        vertex_buffer_data_circular[9*j + 3] = radius*cos(j*theta) + radius*2; 
        vertex_buffer_data_circular[9*j + 4] = radius*sin(j*theta);
        vertex_buffer_data_circular[9*j + 5] = 0.0f;

        vertex_buffer_data_circular[9*j + 6] = radius*cos((j+1)*theta) + radius*2; 
        vertex_buffer_data_circular[9*j + 7] = radius*sin((j+1)*theta); 
        vertex_buffer_data_circular[9*j + 8] = 0.0f;
    }

    this->heart = create3DObject(GL_TRIANGLES, 6*sides, vertex_buffer_data_circular, color, GL_FILL);
    this->invertedTriangle = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data_invertedTriangle, color, GL_FILL);
}

void Health::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->heart);
    draw3DObject(this->invertedTriangle);
}

void Health::movement() {
    if(moveup){
        this->position.y += this->speed;
        if(this->position.y >= 4)
            moveup=false;
    }
    else
    {
        this->position.y -= this->speed;
        if(this->position.y < -1.5f)
            moveup=true;
    }
}

bounding_box_t Health::bounding_box() {
    float x = this->position.x + this->radius, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.25f, 1.0f };
    return bbox;
}

Ring::Ring(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius=1;
    GLfloat vertex_buffer_data_head[900]; 
    int sides = 100;
    float angle = (2*M_PIl)/sides;

    for(int i =0; i < sides/2;i++)
    {
        vertex_buffer_data_head[9*i] = 0.0f;
        vertex_buffer_data_head[9*i + 1] = 0.0f;  
        vertex_buffer_data_head[9*i + 2] = 0.0f;

        vertex_buffer_data_head[9*i + 3] = radius*cos(i*angle); 
        vertex_buffer_data_head[9*i + 4] = radius*sin(i*angle) ;
        vertex_buffer_data_head[9*i + 5] = 0.0f;

        vertex_buffer_data_head[9*i + 6] = radius*cos((i+1)*angle); 
        vertex_buffer_data_head[9*i + 7] = radius*sin((i+1)*angle); 
        vertex_buffer_data_head[9*i + 8] = 0.0f;
    }
    glPointSize(10);
    this->object = create3DObject(GL_TRIANGLES, 1.5f*sides, vertex_buffer_data_head, color, GL_POINT);
}

void Ring::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ring::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Ring::reversetunnel(Player Player) {
    return (std::sqrt(std::pow(Player.position.x - this->position.x, 2) + std::pow(Player.position.y - this->position.y, 2)*1.0))
    - this->radius < 0.1; 
}