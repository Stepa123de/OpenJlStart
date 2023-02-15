#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// GLEW
#define GLEW_STATIC
#include <gl/GL.h>

const int WIDTH = 1920, HEIGHT = 1080, shedersCount = 3;

// шейдер вершин (шейдер 1)
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, 0, 1);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.0f, 0.4f, 0.8f, 1.0f);\n"
"}\n\0";
const GLchar* downVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y-0.5f, 0, 1);\n"
"}\0";
const GLchar* yellowFragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.8f, 0.0f, 1.0f);\n"
"}\n\0";
 

int main() 
{

	if (!glfwInit()) // инициализация библиотеки
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // постоянный размер окна

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "TEST", glfwGetPrimaryMonitor(), NULL);

	GLfloat vertices[] = {
		//flag
	 -0.5f, 0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
	 -0.5f,  0.0f, 0.0f,
	 0.5,0.0f,0.0f
	};
	GLuint indices[] = {
		0,1,2,
		1,2,3
	};

	if (!window) 
	{
		std::cout<< "Faild to create GLFW window" << std::endl;
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);// говорим что будем использовать window
	
	gladLoadGL(); // подгружаем конфигурации для openjl(не понял зачем https://russianblogs.com/article/5959679629/)
	
	glfwSwapBuffers(window);

	std::pair< const GLchar*, const GLchar*> vss[shedersCount]
	{
		std::make_pair(vertexShaderSource,fragmentShaderSource),
		std::make_pair(downVertexShaderSource, yellowFragmentShaderSource)
	};
	GLuint shaderPrograms[shedersCount];
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	for (int i = 0; i < shedersCount; i++)
	{
		glShaderSource(vertexShader, 1, &vss[i].first, NULL);
		glCompileShader(vertexShader);
		glShaderSource(fragmentShader, 1, &vss[i].second, NULL);
		glCompileShader(fragmentShader);

		shaderPrograms[i] = glCreateProgram();
		glAttachShader(shaderPrograms[i], vertexShader);
		glAttachShader(shaderPrograms[i], fragmentShader);
		glLinkProgram(shaderPrograms[i]);
	}


	//УСТАЛУСТАЛ
	GLuint VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	
	while(!glfwWindowShouldClose(window)&&!glfwGetKey(window,GLFW_KEY_ESCAPE))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//работа с шейдорами
		glUseProgram(shaderPrograms[0]);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glUseProgram(shaderPrograms[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window); // ?
	glfwTerminate(); // освобождение ресурсов

	return 0;
}