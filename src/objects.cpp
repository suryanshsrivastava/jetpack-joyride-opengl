#include "objects.h"
#include "main.h"

Coins::Coins(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius=0.1;
    this->collected=false;
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

    if(!this->collected){
        draw3DObject(this->object);
    }
}

void Coins::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Coins::bounding_box() {
    bounding_box_t bbox = { this->position.x, this->position.y, 0.2f, 0.2f };
    return bbox;
}