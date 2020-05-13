#include<iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include "qLearning.h"
#include "gridWorld.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "opengl/shader.h"
#include "opengl/vertexBuffer.h"
#include "opengl/vertexBufferLayout.h"
#include "opengl/indexBuffer.h"
#include "opengl/vertexArray.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace std;

/*--------------------------------------------*/
/*Variables for geometry and color data.*/
unsigned int quadCount = 0;
unsigned int indexCount = 0;
float c_GRAY[3] = { 0.8f,0.8f,0.8f };
float c_BLACK[3] = { 0.0f,0.0f,0.0f };
float c_GREEN[3] = { 0.0f,1.0f,0.0f };
float c_RED[3] = { 1.0f,0.0f,0.0f };
/*Grid vertex and index variables*/
std::vector<float> vertex;
std::vector<GLuint> indices;
GLuint offset = 0;//offset for indices.
int player = 0;//player buffer data offset.
/*--------------------------------------------*/


//Insert normalized coordinates according to grid x,y values.
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
//Create the indices for the grid quads.
void makeIndices() {
	for (unsigned int i = 0; i <= indexCount; i += 6)
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
//Create grid data and color.
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
	posFromMap(START[1], START[0], c_GREEN, &vertex);

	quadCount = vertex.size() / 20;
	indexCount = quadCount * 6;

}

int qLearning::epsilonGreedy(std::vector<double> observation, float epsilon, int numActions){
    std::vector<double> actions(numActions,1);
	for(double a : actions)
	{
		a = a * epsilon / numActions;
	}
	int maxIdx = std::distance(observation.begin(), std::max_element(observation.begin(), observation.end()));
	actions[maxIdx] += (1.0 - epsilon);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> d(actions.begin(), actions.end());

	return d(gen);
}

int qLearning::stateToTable(int* state, int size)
{
	return (state[0] - 1) * (size + (state[1]-1));
}

//Main function for learning and rendering.
std::map<int, std::vector<double>> qLearning::q_learning(gridWorld* environment, int numEpisodes, float discount_factor, float alpha, float epsilon)
{
	//GLFW INIT.
	/*----------------------------------------------------------------------*/
    GLFWwindow* window;
	glewExperimental = GL_TRUE;
    /* Initialize the library */
	if (!glfwInit())
		std::cerr << "Glfw init error." << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(350, 350, "Grid World", NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cerr << "Error !" << std::endl;
    /*----------------------------------------------------------------------*/

	//MAKE GRID QUADS
	makeGrid();
	//MAKE QUAD INDICES
	makeIndices();

	//OPENGL INIT.
	/*----------------------------------------------------------------------*/
	vertexArray va; // Create, bind vertex array.
	vertexBuffer vbo(vertex.data(), vertex.size() * sizeof(GLfloat), GL_DYNAMIC_DRAW);//Create and bind grid data to the vertex buffer.
	vertexBufferLayout vbl;

	vbl.push<float>(2);// For grid X,Y positions.
	vbl.push<float>(3);// For grid colors.
	va.addLayout(vbo, vbl);

	indexBuffer ibo(indices.data(), indices.size(), GL_STATIC_DRAW);//Index buffer for the grid quads.

	/*OpenGL model view projection matrix setup.*/
	shader program = shader("resources\\core.vs", "resources\\core.fs");
	program.useShader();
	glm::vec3 translation(.0f, .0f, .0f);
	glm::mat4 projection = glm::ortho(-1.1f, 1.1f, 1.1f, -1.1f, -1.0f, 1.0f);//Use ortho since 2D scene.
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 mvp = projection * view * model;
	program.setUniformMat4f("mvp", mvp);
	program.setUniform4f("incolor", 1.0f, 0.0f, 1.0f, 1.0f);
	/*------------------------------------------*/
	va.unbind();
	vbo.unbind();
	ibo.unbind();
	program.removeShader();
	renderer r;
	/*----------------------------------------------------------------------*/
	//OPENGL INIT. END.


	//Q LEARNING START.
	/*----------------------------------------------------------------------*/
	currentState = stateToTable(environment->state, environment->size);
	if (!checkQTable(currentState)) {
		qTable.emplace(currentState, std::vector<double>{0, 0, 0, 0});
	}

	for (int i = 0; i < numEpisodes; i++) {
		int c = 0;

		environment->reset();
		currentState = stateToTable(environment->state, environment->size);

		//Run until the learning is finished or the window is closed.
		while (true && !glfwWindowShouldClose(window)) {


			if (!checkQTable(currentState)) {
				qTable.emplace(currentState, std::vector<double>{0, 0, 0, 0});
			}
			system("cls");
			std::cout << "Episode : "<< i+1 <<endl;
			action = epsilonGreedy(qTable[currentState], epsilon, environment->numActions);

			/*Take a step in the environment*/
			environment->step(action);
			/*Print to console*/
			environment->toString();


			/* Update only the new step position inside the buffer. */
			std::vector<float> t_pos;
			posFromMap(environment->state[1], environment->state[0], c_GREEN, &t_pos);
			vbo.update(t_pos.data(), t_pos.size() * sizeof(GLfloat), player * sizeof(GLfloat));
			
			/*Render*/
			r.clear();
			r.draw(va, ibo, program);
			/* Swap front and back buffers */
			glfwSwapBuffers(window);
			/* Poll for and process events */
			glfwPollEvents();


			int best_next_state = stateToTable(environment->state, environment->size);
			if (!checkQTable(best_next_state)) {
				qTable.emplace(best_next_state, std::vector<double>{0, 0, 0, 0});
			}

			// TD Update
			int best_next_action = std::distance(qTable[best_next_state].begin(), std::max_element(qTable[best_next_state].begin(), qTable[best_next_state].end()));
			double td_target = environment->reward + discount_factor * qTable[best_next_state][best_next_action];
			double td_delta = td_target - qTable[currentState][action];
			qTable[currentState][action] += alpha * td_delta;
			if (environment->done) {
				break;
			}
			

			currentState = best_next_state;
		
				
		
		}
	}


	glfwTerminate();
	return std::map<int, std::vector<double>>();
}
/*----------------------------------------------------------------------*/
//Q LEARNING END.


bool qLearning::checkQTable(int state)
{
	if (qTable.find(state) == qTable.end()) {
		// not found
		return false;
	}
	else {
		// found
		return true;
	}
	
}



