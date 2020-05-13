/*_______________________________________________________________________________
* Basic OpenGL Template/Framework for testing. | Ogulcan Ozer. | 11 May 2020    *
*                                                                               *
*   UNFINISHED.   Last updated : 12 May 2020                                    *
*                                                                               *
* Adapted from Yan Chernikov`s OpenGL tutorials.                                *
*_______________________________________________________________________________*/
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gl/glut.h>
#include "shader.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../gridWorld.h"
#include "../qLearning.h"
#include <thread>

float c_GRAY[3] = { 0.8f,0.8f,0.8f };
float c_BLACK[3] = { 0.0f,0.0f,0.0f };
float c_GREEN[3] = { 0.0f,1.0f,0.0f };
float c_RED[3] = { 1.0f,0.0f,0.0f };
int state[2] = {2,2};

const double fpsLimit = 1.0 / 10.0;
double lastUpdateTime = 0;  // number of seconds since the last loop
double lastFrameTime = 0;   // number of seconds since the last frame
int m = 0;
int trj[10] = { 1,1,1,1,1,1,1,1,1, };

/*Grid vertex and index variables*/
std::vector<float> vertex;
std::vector<GLuint> indices;
GLuint offset = 0;
int player = 0;
gridWorld g;
qLearning q;
/*-------------------------------*/

unsigned int quadCount = 0;
unsigned int indexCount = 0;

void posFromMap(int x, int y, float* color, std::vector<float>* v) {

    float min = -64.0f;
    float max = 64.0f;
    GLfloat x1, y1, x2, y2;
    /*Get grid vertex positions*/
    x1 = (x - 4) * 16;
    y1 = (y - 4) * 16;
    x2 = x1 + 15;
    y2 = y1 + 15;
    /*Normalize vertex positions*/
    x1 = (2 * (x1 - min) / (max - min)) - 1;
    x2 = (2 * (x2 - min) / (max - min)) - 1;
    y1 = (2 * (y1 - min) / (max - min)) - 1;
    y2 = (2 * (y2 - min) / (max - min)) - 1;

    v->insert(v->end(), { x1,y1 ,color[0],color[1],color[2] , x2,y1, color[0],color[1],color[2] ,x2,y2, color[0],color[1],color[2] ,x1,y2, color[0],color[1],color[2] });

}


void makeGrid() {

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            if (MAP[j][i] == 0) {        
                if (j == GOAL[0] && i == GOAL[1]) {
                    posFromMap(i, j, c_RED, &vertex);
                }
                else {
                    posFromMap(i, j, c_BLACK, &vertex);
                }
            }
            else if (MAP[j][i] == 1) {
                posFromMap(i, j, c_GRAY, &vertex);
            }


        }

    }

    player = vertex.size();
    posFromMap(state[1], state[0], c_GREEN, &vertex);
    
    quadCount = vertex.size() / 20;
    indexCount = quadCount * 6;

}

void movePlayer(const vertexBuffer* vbo ,int action) {


        if (m >= 9) {
            m = 0;
            state[0] = 2;
            state[1] = 2;
        }


        m++;


        int i = state[0];
        int j = state[1];

        if (action == ACTION_UP) {
            state[0] = i - 1;
            state[1] = j;
        }
        else if (action == ACTION_DOWN)
        {
            state[0] = i + 1;
            state[1] = j;
        }
        else if (action == ACTION_LEFT)
        {
            state[0] = i;
            state[1] = j - 1;
        }
        else if (action == ACTION_RIGHT)
        {
            state[0] = i;
            state[1] = j + 1;
        }


        std::vector<float> tmp;
        posFromMap(state[1], state[0], c_GREEN, &tmp);
        vbo->bind();
        glBufferSubData(GL_ARRAY_BUFFER, player * sizeof(GLfloat), tmp.size() * sizeof(GLfloat), tmp.data());
               
}


void makeIndices() {
    for (unsigned int i = 0; i <= indexCount; i+=6)
    {
       GLuint indices0 = 0 + offset;
       GLuint indices1 = 1 + offset;
       GLuint indices2 = 2 + offset;

       GLuint indices3 = 2 + offset;
       GLuint indices4 = 3 + offset;
       GLuint indices5 = 0 + offset;

       indices.insert(indices.end(), { indices0,indices1,indices2,indices3,indices4,indices5 });
        offset += 4;
    }

}


int main(void)
{
    /*----------------------------------------------------------------------*/
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(350, 350, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cerr << "Error !" << std::endl;
    /*----------------------------------------------------------------------*/

    makeGrid();
    makeIndices();



    vertexArray va;
    vertexBuffer vbo(vertex.data(), vertex.size() * sizeof(GLfloat), GL_DYNAMIC_DRAW);
    vertexBufferLayout vbl;

    vbl.push<float>(2);
    vbl.push<float>(3);
    va.addLayout(vbo, vbl);

    indexBuffer ibo(indices.data(), indices.size() , GL_STATIC_DRAW);

    shader program = shader("resources\\core.vs", "resources\\core.fs");
    program.useShader();
    glm::mat4 projection = glm::ortho(-2.1f, 2.1f, 2.1f, -2.1f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate( glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
    program.setUniform4f("incolor", 1.0f, 0.0f, 1.0f, 1.0f);

    va.unbind();
    vbo.unbind();
    ibo.unbind();
    program.removeShader();

    renderer r;
    glm::vec3 translation(.0f,.0f,.0f);


    /*
    
        .INITIALIZE HERE.
        
    */






    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
            
        

        //Example
        r.clear();
        program.useShader();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = projection * view * model;
        program.setUniformMat4f("mvp", mvp);


        double now = glfwGetTime();
        double deltaTime = now - lastUpdateTime;
        
        if ((now - lastFrameTime) >= fpsLimit)
        {

            movePlayer(&vbo, trj[m]);
            r.draw(va, ibo, program);
            glfwSwapBuffers(window);
            lastFrameTime = now;
        }
        //{
        //    ImGui::SliderFloat3("float", &translation.x, -4.0f, 4.0f);            
        //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //}

        /*
        
            .RENDER HERE.
        
        */


        lastUpdateTime = now;
        /* Swap front and back buffers */
        

        /* Poll for and process events */
        glfwPollEvents();
    }
  
    
    glfwTerminate();

    return 0;
}
