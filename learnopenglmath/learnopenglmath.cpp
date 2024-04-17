// learnopenglmath.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <map>
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
#include "models.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int frameIndex = 0;
float startFrameTime = 0.;
float deltaTime = 0.;
float averageFPS = 0.;
GLFWwindow* window;
std::unique_ptr<Camera> mainCamera;
std::unique_ptr<Texture> placeholderTexture;
std::unique_ptr<Texture> sampleDiffuseMap;
std::unique_ptr<Texture> sampleSpecularMap;

glm::vec2 horizontalInput;
float verticalInput;

glm::vec3 inputVector;
float speed = 0.05f;

glm::vec2 cursorPosition;
glm::vec2 cursorDelta;
float rotationSensitivity = 0.1f;
glm::vec3 lightSourcePos(0.0f, 1.0f, 3.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 cubePosTest = glm::vec3(0.0, 0.0, -2.0);
bool mmbPressed = false;

Shader cubeShader;
Shader cubeShaderLitView;
Shader lightSourceShader;
GLuint cubeVAO;

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

std::map<std::string, int> logsDict;

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

    std::string logText = "GL Callback: " + std::string(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
        + "type = " + std::to_string(type)
        + ", severity = " + std::to_string(severity)
        + ", message = " + message;
    if (logsDict.contains(logText))
    {
        logsDict[logText]++;
    }
    else
    {
        logsDict[logText] = 1;
        std::cerr << logText << "\n";
    
    }
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

void drawFrame()
{
    lightSourcePos.x += sin(glfwGetTime()) / 15;
    lightSourcePos.z += cos(glfwGetTime()) / 15;
    glBindVertexArray(cubeVAO);
    cubeShader.use();
    cubeShader.setMat4("view", mainCamera->getViewMatrix());
    cubeShader.setMat4("projection", mainCamera->getProjectionMatrix());
    placeholderTexture->bind();
    cubeShader.setInt("imageTex", 1);
    cubeShader.setVec3("objectColor", 1.f, 1.f, 1.f);
    cubeShader.setVec3("lightColor", lightColor);
    cubeShader.setVec3("lightPos", lightSourcePos);
    cubeShader.setVec3("viewPos", mainCamera->cameraPosition);

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


    glm::mat4 model = glm::translate(glm::mat4(1.), glm::vec3(0, 0, 0));
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
}


void drawFrameTest()
{
    glBindVertexArray(cubeVAO);
    //cubePosTest.x = sin(glm::radians(startFrameTime * 20))*3;

    sampleDiffuseMap->bind();
    sampleSpecularMap->bind();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePosTest);

    cubeShaderLitView.use();
    cubeShaderLitView.setMat4("view", mainCamera->getViewMatrix());
    cubeShaderLitView.setMat4("projection", mainCamera->getProjectionMatrix());
    cubeShaderLitView.setVec3("lightColor", lightColor);
    cubeShaderLitView.setMat4("model", model);
    cubeShaderLitView.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    cubeShaderLitView.setInt("material.diffuse", sampleDiffuseMap->getTextureUnit() - GL_TEXTURE0);
    cubeShaderLitView.setInt("material.specular", sampleSpecularMap->getTextureUnit() - GL_TEXTURE0);
    cubeShaderLitView.setFloat("material.shininess", 32.0f);
    cubeShaderLitView.setVec3("light.position", lightSourcePos);
    cubeShaderLitView.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeShaderLitView.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    cubeShaderLitView.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

    lightSourceShader.use();
    lightSourceShader.setMat4("view", mainCamera->getViewMatrix());
    lightSourceShader.setMat4("projection", mainCamera->getProjectionMatrix());
    lightSourceShader.setVec3("lightColor", lightColor);
    model = glm::translate(glm::mat4(1.f), lightSourcePos);
    lightSourceShader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void update()
{
    auto now = glfwGetTime();
    deltaTime = now - startFrameTime;
    startFrameTime = now;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0.1, 0.2, 1);


    if (mmbPressed)
    {
        mainCamera->rotate(cursorDelta*rotationSensitivity);
    }
    mainCamera->moveLocal(inputVector * speed);


    drawFrameTest();
    //drawFrame();

    glfwSwapBuffers(window);

    glm::vec2 oldCursorPos = cursorPosition;
    glfwPollEvents();
    glm::vec2 newCursorPos = cursorPosition;

    cursorDelta = newCursorPos - oldCursorPos;
    
    frameIndex++;
    averageFPS = frameIndex / glfwGetTime();
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

    cubeShader = Shader(PROJECT_SRC + "\\Shaders\\phong_lit_world.vert", PROJECT_SRC + "\\Shaders\\phong_lit_world.frag");
    cubeShaderLitView = Shader(PROJECT_SRC + "\\Shaders\\phong_lit.vert", PROJECT_SRC + "\\Shaders\\phong_lit.frag");
    lightSourceShader = Shader(PROJECT_SRC + "\\Shaders\\light_source.vert", PROJECT_SRC + "\\Shaders\\light_source.frag");
    mainCamera = std::unique_ptr<Camera>(new Camera((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT));
    
    placeholderTexture = std::unique_ptr<Texture>(new Texture(GL_TEXTURE1, GL_TEXTURE_2D));
    placeholderTexture->setImage2DFromFile(PROJECT_ROOT + "\\resources\\images\\MainTopGreen.jpg");

    sampleDiffuseMap = std::unique_ptr<Texture>(new Texture(GL_TEXTURE2, GL_TEXTURE_2D));
    sampleDiffuseMap->setImage2DFromFile(PROJECT_ROOT + "\\resources\\images\\container2.png");

    sampleSpecularMap = std::unique_ptr<Texture>(new Texture(GL_TEXTURE3, GL_TEXTURE_2D));
    sampleSpecularMap->setImage2DFromFile(PROJECT_ROOT + "\\resources\\images\\container2_specular.png");

    while (!glfwWindowShouldClose(window))
    {
        update();
    }
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
