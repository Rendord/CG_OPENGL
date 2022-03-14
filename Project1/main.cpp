#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glsl.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 800, HEIGHT = 600;
GLuint program_id;
GLuint vao;
const GLfloat vertices[] = {
	0.5,-0.5,0.0,1.0,
	-0.5,-0.5,0.0,1.0,
	0.0,0.5,0.0,1.0
};

const GLfloat color[] = {
	1,-0.5,0.0,1.0,
	-0.5,-0.5,0.0,1.0,
	0.0,0.5,1,1.0
};
GLuint position_id;
GLuint vbo_vertices;
GLuint vbo_color;
GLuint color_id;

bool up = true;
float f = 0.05;
unsigned const int DELTA_TIME = 10;


//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
    if (key == 27)
        glutExit();
}

//--------------------------------------------------------------------------------
// VEEEEEEEEEEECTOOOOOOOOOOOOORRRRRRRRR
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    //Define background
    static const GLfloat blue[] = { 0.5, 0.0, 0.4, 1.0 };
    glClearBufferfv(GL_COLOR, 0, blue);


	glUseProgram(program_id);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glm::vec4 v = glm::vec4(0.0, f, 0.0, 1.0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferSubData(GL_ARRAY_BUFFER,  // target
		4 * sizeof(glm::vec4),         // offset
		sizeof(glm::vec4),             // size
		&v);                           // data
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);

	if (up) {
		if (f < 1.0) {
			f += 0.1f;
		}
		else {
			up = false;
		}
	} if (!up) {
		f -= 0.1f;
		if (f <= 0.0) {
			up = true;
		}
	}





    // Swap buffers
    glutSwapBuffers();
}

void Render(int n)
{
	Render();
	glutTimerFunc(DELTA_TIME, Render, 0);
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);
	glutTimerFunc(DELTA_TIME, Render, 0);

    glewInit();
}

void initShaders() {
	char* vertexshader = glsl::readFile("vertexshader.vert");
	GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	char* fragshader = glsl::readFile("fragmentshader.frag");
	GLuint fsh_id = glsl::makeFragmentShader(fragshader);

	program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}

void initBuffers() {
	// Generate buffer object (in this case 1)
	glGenBuffers(1, &vbo_vertices);

	// Bind named buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);

	// Create and initialize buffer object’s data store
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
		vertices, GL_STATIC_DRAW);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Location of attribute variable
	position_id = glGetAttribLocation(program_id, "position");

	//-------------------------------------------------------------------------
	glGenBuffers(1, &vbo_color);

	// Bind named buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);

	// Create and initialize buffer object’s data store
	glBufferData(GL_ARRAY_BUFFER, sizeof(color),
		color, GL_STATIC_DRAW);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Location of attribute variable
	color_id = glGetAttribLocation(program_id, "color");
}


int main(int argc, char** argv)
{
	
    InitGlutGlew(argc, argv);
	initShaders();
	initBuffers();

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glVertexAttribPointer(position_id, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glVertexAttribPointer(color_id, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(color_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	

	// Create vector
	//glm::vec4 position = glm::vec4(0.3f, 0.4f, 0.0f, 1.0f);
	//glm::vec4 gl_FragColor = glm::vec4(.0, .0, .0, 0.0);

	// Get location of uniform variable (create if new)
	//GLuint position_id = glGetUniformLocation(program_id, "position");
	//GLuint color_id = glGetUniformLocation(program_id, "color");

	// Attach to program (needed to fill uniform vars)
	//glUseProgram(program_id);

	// Specify the value of the uniform variable
	// Args: location, count, value
	//glUniform4fv(position_id, 1, glm::value_ptr(position));
	//glUniform4fv(color_id, 1, glm::value_ptr(gl_FragColor));


    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
