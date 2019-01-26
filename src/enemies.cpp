#include "enemies.h"
#include "main.h"

FireLine::FireLine(float x, float y, double angle, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rand()%180;
    this->radius=0.1;
    this->length=1;

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

    static const GLfloat vertex_buffer_data_cylinderical[]={
        0.0f-0.025f*sin(angle), 0.025f+0.025*cos(angle), 0.0f,
        0.0f+length-0.025f*sin(angle), 0.025f+0.025*cos(angle), 0.0f,
        0.0f+length+0.025f*sin(angle), -0.025f-0.025*cos(angle), 0.0f,

        0.0f-0.025f*sin(angle), 0.025f+0.025*cos(angle), 0.0f,
        0.0f-0.025f*sin(angle), -0.025f-0.025*cos(angle), 0.0f,
        0.0f+length+0.025f*sin(angle), -0.025f-0.025*cos(angle), 0.0f,
    };

    for(int j = 101; j < sides+101; j++)
    {
        vertex_buffer_data_circular[9*j] = 0.0f + length*cos(angle);
        vertex_buffer_data_circular[9*j + 1] = 0.0f + length*sin(angle);  
        vertex_buffer_data_circular[9*j + 2] = 0.0f;

        vertex_buffer_data_circular[9*j + 3] = radius*cos(j*theta) + length*cos(angle); 
        vertex_buffer_data_circular[9*j + 4] = radius*sin(j*theta) + length*sin(angle);
        vertex_buffer_data_circular[9*j + 5] = 0.0f;

        vertex_buffer_data_circular[9*j + 6] = radius*cos((j+1)*theta) + length*cos(angle); 
        vertex_buffer_data_circular[9*j + 7] = radius*sin((j+1)*theta) + length*sin(angle); 
        vertex_buffer_data_circular[9*j + 8] = 0.0f;
    }
    
    this->circular = create3DObject(GL_TRIANGLES, 6*sides, vertex_buffer_data_circular, color, GL_FILL);
    this->cylindrical = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data_cylinderical, color, GL_FILL); 
}

void FireLine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->circular);
    draw3DObject(this->cylindrical);
}

void FireLine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool FireLine::detect_collision(Player Player) {
    return (std::sqrt(std::pow(Player.position.x - this->position.x, 2) + std::pow(Player.position.y - this->position.y, 2)*1.0)
    +      std::sqrt(std::pow(Player.position.x - this->position.x - length*cos(this->rotation* M_PI / 180.0f), 2) + 
           std::pow(Player.position.y - this->position.y -length*sin(this->rotation* M_PI / 180.0f), 2)*1.0))
    - length < 0.1; 
}

FireBeam::FireBeam(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius=0.1;
    this->length=1;

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

    static const GLfloat vertex_buffer_data_cylinderical[]={
        0.0f, 0.05f, 0.0f,
        0.0f+length, 0.05f, 0.0f,
        0.0f+length, -0.05f, 0.0f,

        0.0f-0.025f*sin(angle), 0.025f+0.025*cos(angle), 0.0f,
        0.0f-0.025f*sin(angle), -0.025f-0.025*cos(angle), 0.0f,
        0.0f+length+0.025f*sin(angle), -0.025f-0.025*cos(angle), 0.0f,
    };

    for(int j = 101; j < sides+101; j++)
    {
        vertex_buffer_data_circular[9*j] = 0.0f + length;
        vertex_buffer_data_circular[9*j + 1] = 0.0f + length;  
        vertex_buffer_data_circular[9*j + 2] = 0.0f;

        vertex_buffer_data_circular[9*j + 3] = radius*cos(j*theta) + length*cos(angle); 
        vertex_buffer_data_circular[9*j + 4] = radius*sin(j*theta) + length*sin(angle);
        vertex_buffer_data_circular[9*j + 5] = 0.0f;

        vertex_buffer_data_circular[9*j + 6] = radius*cos((j+1)*theta) + length*cos(angle); 
        vertex_buffer_data_circular[9*j + 7] = radius*sin((j+1)*theta) + length*sin(angle); 
        vertex_buffer_data_circular[9*j + 8] = 0.0f;
    }
    
    this->circular = create3DObject(GL_TRIANGLES, 6*sides, vertex_buffer_data_circular, color, GL_FILL);
    this->cylindrical = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data_cylinderical, color, GL_FILL); 
}

void FireLine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->circular);
    draw3DObject(this->cylindrical);
}

void FireLine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool FireLine::detect_collision(Player Player) {
    return (std::sqrt(std::pow(Player.position.x - this->position.x, 2) + std::pow(Player.position.y - this->position.y, 2)*1.0)
    +      std::sqrt(std::pow(Player.position.x - this->position.x - length*cos(this->rotation* M_PI / 180.0f), 2) + 
           std::pow(Player.position.y - this->position.y -length*sin(this->rotation* M_PI / 180.0f), 2)*1.0))
    - length < 0.1; 
}
