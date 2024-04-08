// learnopenglmath.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "learnopenglmath.h" 
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "Shader.h"
#include "math_utils.h"
#include "camera.h"
#include "texture.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int frameIndex = 0;
GLFWwindow* window;
std::unique_ptr<Camera> mainCamera;
std::unique_ptr<Texture> placeholderTexture;

glm::vec2 horizontalInput;
float verticalInput;

glm::vec3 inputVector;
float speed = 0.05f;

glm::vec2 cursorPosition;
glm::vec2 cursorDelta;
float rotationSensitivity = 0.1f;
glm::vec3 lightSourcePos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

bool mmbPressed = false;

Shader cubeShader;
Shader lightSourceShader;
GLuint cubeVAO;

float vertices[] = {
    -0.5,  0.5, 0,  0.0, 1.0, // left top
     0.5,  0.5, 0,  1.0, 1.0, // right top
    -0.5, -0.5, 0,  0.0, 0.0, // left bottom
     0.5, -0.5, 0,  1.0, 0.0  // right bottom
};

GLuint indices[] = {
    0, 1, 2,
    2, 3, 1
};


//done : left top back right bot
float cubeVertices[] = {
    // position       // UV      // face normal
   -0.5,  0.5, -0.5,  0.0, 1.0,  -1.0,  0.0,  0.0, // left top back   n: left |0
   -0.5,  0.5, -0.5,  0.0, 1.0,   0.0,  1.0,  0.0, // left top back   n: top  |1
   -0.5,  0.5, -0.5,  1.0, 1.0,   0.0,  0.0, -1.0, // left top back   n: back |2
                                                                              
    0.5,  0.5, -0.5,  1.0, 1.0,   1.0,  0.0,  0.0, // right top back  n: right|3
    0.5,  0.5, -0.5,  1.0, 1.0,   0.0,  1.0,  0.0, // right top back  n: top  |4
    0.5,  0.5, -0.5,  0.0, 1.0,   0.0,  0.0, -1.0, // right top back  n: back |5
                                                                              
   -0.5, -0.5, -0.5,  0.0, 0.0,  -1.0,  0.0,  0.0, // left bot back   n: left |6
   -0.5, -0.5, -0.5,  0.0, 0.0,   0.0, -1.0,  0.0, // left bot back   n: bot  |7
   -0.5, -0.5, -0.5,  1.0, 0.0,   0.0,  0.0, -1.0, // left bot back   n: back |8
                                                                              
    0.5, -0.5, -0.5,  1.0, 0.0,   1.0,  0.0,  0.0, // right bot back  n: right|9
    0.5, -0.5, -0.5,  1.0, 0.0,   0.0, -1.0,  0.0, // right bot back  n: bot  |10
    0.5, -0.5, -0.5,  0.0, 0.0,   0.0,  0.0, -1.0, // right bot back  n: back |11
                                                                              
   -0.5,  0.5,  0.5,  1.0, 1.0,  -1.0,  0.0,  0.0, // left top front  n: left |12
   -0.5,  0.5,  0.5,  0.0, 0.0,   0.0,  1.0,  0.0, // left top front  n: top  |13
   -0.5,  0.5,  0.5,  0.0, 1.0,   0.0,  0.0,  1.0, // left top front  n: front|14

    0.5,  0.5,  0.5,  0.0, 1.0,   1.0,  0.0,  0.0, // right top front n: right|15
    0.5,  0.5,  0.5,  1.0, 0.0,   0.0,  1.0,  0.0, // right top front n: top  |16
    0.5,  0.5,  0.5,  1.0, 1.0,   0.0,  0.0,  1.0, // right top front n: front|17
   
   -0.5, -0.5,  0.5,  1.0, 0.0,  -1.0,  0.0,  0.0, // left bot front  n: left |18
   -0.5, -0.5,  0.5,  0.0, 1.0,   0.0, -1.0,  0.0, // left bot front  n: bot  |19
   -0.5, -0.5,  0.5,  0.0, 0.0,   0.0,  0.0,  1.0, // left bot front  n: front|20
   
    0.5, -0.5,  0.5,  0.0, 0.0,   1.0,  0.0,  0.0, // right bot front n: right|21
    0.5, -0.5,  0.5,  1.0, 1.0,   0.0, -1.0,  0.0, // right bot front n: bot  |22
    0.5, -0.5,  0.5,  1.0, 0.0,   0.0,  0.0,  1.0, // right bot front n: front|23
};

GLuint cubeIndices[] = {
    13, 1 ,  4 ,  4 , 16, 13, // top
    20, 14,  17,  17, 23, 20, // front
    7 , 19,  22,  22, 10, 7 , // bot
    11, 5 ,  2 ,  2 , 8 , 11, // back
    21, 15,  3 ,  3 , 9 , 21, // right
    6 , 18,  12,  12, 0 , 6 , // left
};

glm::vec3 cubePositions[] = {
    glm::vec3(1.0f,  -1.0f,  2.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};


glm::mat4 getModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    return model;
}

#pragma region GLFW_CALLBACKS


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        mmbPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        mmbPressed = false;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    cursorPosition = glm::vec2(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    glm::vec2 horizontalForward = -glm::vec2(0, 1);
    glm::vec2 horizontalRight = glm::vec2(1, 0);

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            horizontalInput += horizontalForward;
            break;
        case GLFW_KEY_A:
            horizontalInput -= horizontalRight;
            break;
        case GLFW_KEY_S:
            horizontalInput -= horizontalForward;
            break;
        case GLFW_KEY_D:
            horizontalInput += horizontalRight;
            break;
        case GLFW_KEY_SPACE:
            verticalInput++;
            break;
        case GLFW_KEY_C:
            verticalInput--;
            break;
        default:
            break;
        }

    }
    
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            horizontalInput -= horizontalForward;
            break;
        case GLFW_KEY_A:
            horizontalInput += horizontalRight;
            break;
        case GLFW_KEY_S:
            horizontalInput += horizontalForward;
            break;
        case GLFW_KEY_D:
            horizontalInput -= horizontalRight;
            break;
        case GLFW_KEY_SPACE:
            verticalInput--;
            break;
        case GLFW_KEY_C:
            verticalInput++;
            break;
        default:
            break;
        }
    }

    glm::vec2 normalizedHorizontal = length2(horizontalInput) != 0 ? glm::normalize(horizontalInput) : glm::vec2(0,0);
    inputVector = glm::vec3(normalizedHorizontal.x, verticalInput, normalizedHorizontal.y);
}


#pragma endregion

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "GL Callback: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << "type = " << type << ", severity = " << severity << ", message = " << message << "\n";
}

void initGLFW()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(error_callback);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Title", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, nullptr);
}

void update()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0.1, 0.2, 1);

    glBindVertexArray(cubeVAO);

    if (mmbPressed)
    {
        mainCamera->rotate(cursorDelta*rotationSensitivity);
    }
    mainCamera->moveLocal(inputVector * speed);

    cubeShader.use();
    cubeShader.setMat4("view", mainCamera->getViewMatrix());
    cubeShader.setMat4("projection", mainCamera->getProjectionMatrix());
    placeholderTexture->bind();
    cubeShader.setInt("imageTex", 1);
    cubeShader.setVec3("objectColor", 1.0f, 1.f, 1.f);
    cubeShader.setVec3("lightColor", lightColor);

    for (int i = 0; i < std::size(cubePositions); i++)
    {
        glm::mat4 model = glm::mat4(1.);

        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        if (i % 3 == 0)
        {
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
        }
        cubeShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    }
    glBindVertexArray(0);


    glm::mat4 model = glm::translate(glm::mat4(1.), glm::vec3(0,0,0));
    model = glm::translate(model, lightSourcePos);
    model = glm::scale(model, glm::vec3(0.2f));
    glBindVertexArray(cubeVAO);
    lightSourceShader.use();
    lightSourceShader.setMat4("view", mainCamera->getViewMatrix());
    lightSourceShader.setMat4("projection", mainCamera->getProjectionMatrix());
    lightSourceShader.setVec3("lightColor", lightColor);
    lightSourceShader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);


    glfwSwapBuffers(window);

    glm::vec2 oldCursorPos = cursorPosition;
    glfwPollEvents();
    glm::vec2 newCursorPos = cursorPosition;

    cursorDelta = newCursorPos - oldCursorPos;

    frameIndex++;
}

GLuint createLightSourceVAO()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindVertexArray(0);

    return vao;
}

GLuint createCubeVAO()
{

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindVertexArray(0);

    return vao;
}

int main()
{
    initGLFW();
    cubeVAO = createCubeVAO();

    cubeShader = Shader(PROJECT_SRC + "\\Shaders\\square.vert", PROJECT_SRC + "\\Shaders\\square.frag");
    lightSourceShader = Shader(PROJECT_SRC + "\\Shaders\\light_source.vert", PROJECT_SRC + "\\Shaders\\light_source.frag");
    mainCamera = std::unique_ptr<Camera>(new Camera((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT));
    
    placeholderTexture = std::unique_ptr<Texture>(new Texture(GL_TEXTURE1, GL_TEXTURE_2D));
    placeholderTexture->setImage2DFromFile("D:\\cpp-projects\\learnopenglmath\\resources\\images\\MainTopGreen.jpg");

    while (!glfwWindowShouldClose(window))
    {
        update();
    }
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
