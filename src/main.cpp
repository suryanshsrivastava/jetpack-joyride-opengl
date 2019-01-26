#include "main.h"
#include <math.h>
#include "timer.h"
#include "player.h"
#include "platform.h"
#include "objects.h"
#include "enemies.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Player barry;
Player awesome_barry;
Platform platform;
vector<Coins> monies;
vector<FireLine> enemies1;
vector<FireBeam> enemies2;
Boomerang enemy3;
Magnet magnet;
vector<Shield> shields;
vector<Health> hearts;
Balloons bullet;
Ring semi;
Viserion dragon;

int LIFE = 7;
int SCORE = 0;
int LEVEL = 1;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
// float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye (screen_center_x,screen_center_y,1);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (screen_center_x, screen_center_y, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    platform.draw(VP);
    barry.draw(VP);
    for(int i=0; i<monies.size(); i++) {
        monies[i].draw(VP);
    }
    for(int i=0; i<enemies1.size(); i++) {
        enemies1[i].draw(VP);
    }
    for(int i=0; i<enemies2.size(); i++) {
        enemies2[i].draw(VP);
    }
    if(magnet.spawn)
        magnet.draw(VP);

    if(enemy3.spawn)
        enemy3.draw(VP);
    
    for(int i=0; i<shields.size(); i++) {
        shields[i].draw(VP);
    }

    for(int i=0; i<hearts.size(); i++) {
        hearts[i].draw(VP);
    }

    if(bullet.fired)
        bullet.draw(VP);

    semi.draw(VP);

    if(dragon.health>0)
        dragon.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int fire = glfwGetKey(window, GLFW_KEY_SPACE);
    int j = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);
    if (left) {
        barry.tick_left();
    }
    if (right) {
        barry.tick_right();
    }
    if (up) {
        barry.flying=true;
        barry.tick_jump();  
    }
    else
    {
        barry.flying=false;
    }
    if (fire) {
        bullet.fired=true;
        bullet.set_position(barry.position.x, barry.position.y);
    }
    if(j) {
        barry.tick_jump_phy();
    }
}

void tick_elements() {
    //Gravity
    barry.gravity();

    //Coin Collection
    for(int i=0; i<monies.size(); i++) {
        if (detect_collision(monies[i].bounding_box(), barry.bounding_box())) {
        monies.erase(monies.begin()+i);
        }
    }

    //Collision detection with Fireline
    for(int i=0; i<enemies1.size(); i++) {
        if (enemies1[i].detect_collision(barry) && barry.shield==-1) {
        barry.set_position(screen_center_x-4,0);
        SCORE++;
        LIFE--;
        }
        if (enemies1[i].extinguish(bullet)) {
            enemies1.erase(enemies1.begin()+i);
        }
    }

    //Collision detection with Firebeams
    for(int i=0; i<enemies2.size(); i++) {
        if (detect_collision(enemies2[i].bounding_box(), barry.bounding_box()) && barry.shield==-1) {
            enemies2.erase(enemies2.begin()+i);
        }
    }

    //Movement of Firebeams
    for(int i=0; i<enemies2.size(); i++)
        enemies2[i].movement(); 

    //Spawning magnets
    if((int)screen_center_x%50>0 && (int)screen_center_x%50<8){
        magnet.set_position(screen_center_x-4/screen_zoom,screen_center_y/screen_zoom);
        magnet.spawn=true;
    }
    else
    {
        magnet.spawn=false;
    }

    //Effect of magnets
    if(barry.position.y < magnet.position.y+0.6f && barry.position.y > magnet.position.y - 0.6f){
        if(magnet.spawn){
            magnet.effect(barry);
        }
    }

    //Spawning boomerang
    if((int)screen_center_x%50>=0 && (int)screen_center_x%50<=8){
        if((int)barry.position.x%50>=0 && (int)barry.position.x%50<=1){
        }
        enemy3.spawn=true;
        enemy3.movement();
    }
    else
    {
        enemy3.spawn=false;
    }

    //Collision with boomerang
    if(enemy3.spawn){
        if(detect_collision(enemy3.bounding_box(), barry.bounding_box()) || enemy3.position.y < screen_center_y-4){
            enemy3.spawn = false;
            enemy3.speed_x = -0.15;
            enemy3.set_position(screen_center_x+4/screen_zoom,screen_center_y+4/screen_zoom);
            SCORE -= 10;
        }
    }

    //Movement of shields
    for(int i=0; i<shields.size(); i++)
        shields[i].movement();

    // Collision detection with shields
    for(int i=0; i<shields.size(); i++) {
        if (detect_collision(shields[i].bounding_box(), barry.bounding_box())) {
            barry.shield=barry.position.x;
            shields.erase(shields.begin()+i);
        }
    }
    // printf("%f", barry.shield);
    if(barry.shield!=-1){
        if(barry.position.x - barry.shield > 10)
        {
            barry.shield=-1;
        }
    }

    //Movement of hearts
    for(int i=0; i<shields.size(); i++)
        shields[i].movement();

    // Collision detection with hearts
    for(int i=0; i<hearts.size(); i++) {
        if (detect_collision(hearts[i].bounding_box(), barry.bounding_box())) {
            LIFE++;
            hearts.erase(hearts.begin()+i);
        }
    }

    //Firing water balloons
    if(bullet.fired){
        bullet.fire(barry);
    }
    if(bullet.position.y < screen_center_y-2){ 
        bullet.fired=false;
        bullet.speed_x=0.05f;
        bullet.speed_y=0.05f;
    }

    // Collision detection with semi circular ring
    if (semi.reversetunnel(barry)) {
        barry.tunnel = true;
        barry.shield = true;
        barry.position.y = semi.position.y + std::sqrt(std::pow(semi.radius,2) - std::pow((barry.position.x-semi.position.x),2));
        // barry.up=0;
    }

    if(barry.position.x >= semi.position.x + semi.radius){
        barry.tunnel = false;
    }        

    // Spawning dragon
    if((int)screen_center_x>=50){
        dragon.movement(barry);
    }
    //Collision with dragon
    if(dragon.health){
        if(detect_collision(dragon.bounding_box(), barry.bounding_box())){
            barry.set_position(screen_center_x-4,0);
            LIFE--;
        }
        if(detect_collision(bullet.bounding_box(), dragon.bounding_box())){
            dragon.health--;
            SCORE +=10;
        }
    }

    //Panning
    if(barry.position.x > screen_center_x+4 || barry.position.x < screen_center_x-4) {
       screen_center_x = barry.position.x;
       platform.set_position(barry.position.x, 0.0f); 
    }
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    barry   = Player(-4, -1.5, COLOR_RED);

    platform    = Platform(0.0f ,0.0f ,18.0f, 4.0f, COLOR_GREEN);

    for(int i=0; i<2000; i++){
        if(rand()%2){
        monies.push_back(Coins(rand()%1000, rand()%5 - 1, COLOR_YELLOW));
        }
        else{
        monies.push_back(Coins(rand()%1000, rand()%5 - 1, COLOR_ORANGE));
        }
    }

    for(int i=0; i<250; i++) {
        enemies1.push_back(FireLine(rand()%1000, rand()%5 - 1, 0 ,COLOR_RED));
    }

    for(int i=0; i<250; i++) {
        enemies2.push_back(FireBeam(rand()%1000, rand()%5 - 1, COLOR_NEONPINK));
    }    

    enemy3 = Boomerang(screen_center_y+4, screen_center_x+4, COLOR_BLUE);
    
    magnet = (Magnet(screen_center_x-4/screen_zoom,0,COLOR_RED));

    for(int i=0; i<100; i++) {
        shields.push_back(Shield(rand()%1000, rand()%5 - 1, COLOR_BLUE));
    } 

    for(int i=0; i<50; i++) {
        hearts.push_back(Health(rand()%1000, rand()%5 - 1, COLOR_HEARTRED));
    }

    bullet = Balloons(barry.position.x, barry.position.y, COLOR_WATERBLUE);

    semi = Ring(2, 0, COLOR_WATERBLUE);

    dragon = Viserion(screen_center_x+3+50, 0, COLOR_BLACK);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 800;
    int height = 800;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window) && LIFE) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            tick_elements();
            tick_input(window);
            printf("Lives: %i  Score: %i\n", LIFE, SCORE);
            // glutSetWindowTitle("");
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
