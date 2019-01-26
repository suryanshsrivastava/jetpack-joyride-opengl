#include "main.h"
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
Platform platform;
vector<Coins> monies;
vector<FireLine> enemies1;

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
    enemies1[0].set_position(0,0);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if (left) {
        barry.tick_left();
    }
    if (right) {
        barry.tick_right();
    }
    if (up) {
        barry.tick_jump();  
    }
}

void tick_elements() {
    //Gravity
    barry.gravity();

    //Coin Collection
    for(int i=0; i<monies.size(); i++) {
        if (detect_collision(monies[i].bounding_box(), barry.bounding_box())) {
        monies.erase(monies.begin()+i);
        // monies[i].collected=true;
        printf("ka ching\n");
        }
    }

    // Collision detection with enemies
    for(int i=0; i<enemies1.size(); i++) {
        if (enemies1[i].detect_collision(barry)) {
        barry.set_position(screen_center_x-4,0);
        }
    }

    // if (enemies1[0].detect_collision(barry)) {
    //     barry.set_position(screen_center_x-4,0);
    // }

    //Panning
    if(barry.position.x > screen_center_x+4 || barry.position.x < screen_center_x-4) {
       screen_center_x = barry.position.x;
       printf("%i\n", screen_center_x);
       platform.set_position(barry.position.x, 0.0f); 
    }
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    barry   = Player(0, 0, COLOR_RED);

    platform    = Platform(0.0f ,0.0f ,18.0f, 4.0f, COLOR_GREEN);

    for(int i=0; i<1000; i++){
        if(rand()%2){
        monies.push_back(Coins(rand()%1000, rand()%5 - 1, COLOR_YELLOW));
        }
        else{
        monies.push_back(Coins(rand()%1000, rand()%5 - 1, COLOR_ORANGE));
        }
    }

    for(int i=0; i<500; i++) {
        enemies1.push_back(FireLine(rand()%1000, rand()%5 - 1, 0 ,COLOR_RED));
    }    
    
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
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            tick_elements();
            tick_input(window);
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
