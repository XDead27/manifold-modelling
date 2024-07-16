#include "Model.h"
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <Shaders/Shader.h>
#include <Models/Mesh.h>
#include <Utils/TextureUtils.h>
#include <Scene/Camera.h>
#include <Scene/Light.h>
#include <Scene/Actor.h>
#include <Renderer/Renderer.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// input variables
bool mousePressed = false;
Actor* selectedActor = NULL;
double lastX = SCR_WIDTH / 2.0f; 
double lastY = SCR_HEIGHT / 2.0f;

// modes
enum EditorMode {
    EM_IDLE,
    EM_MOVEMENT,
    EM_OBJECT_SELECTED
} currentMode = EM_IDLE;

// input actions
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

// picking
Renderer *renderer;

// camera
Camera camera("c1");
double lastXc = SCR_WIDTH / 2.0f; 
double lastYc = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Actor* cubeActor1, *cubeActor2;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Manifold Modelling", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    Shader shdr(
        std::string(SHADER_DIR) + "/klein-cube.vs", 
        std::string(SHADER_DIR) + "/material-cube.fs"
    );

    Shader pickingShader(
        std::string(SHADER_DIR) + "/klein-cube.vs",
        std::string(SHADER_DIR) + "/picking.fs"
    );
    renderer = new Renderer(&shdr, &pickingShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<Vertex> vertices = {
        // positions          // normals           // texture coords
        {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, { 1.0f,  0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, { 1.0f,  1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, { 1.0f,  1.0f}},
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  1.0f}},
        {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f}},

        {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, { 1.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, { 1.0f,  1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, { 1.0f,  1.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, { 0.0f,  1.0f}},
        {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f}},

        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, { 1.0f,  1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, { 0.0f,  1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, { 0.0f,  1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f}},

        {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, { 1.0f,  1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, { 0.0f,  1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, { 0.0f,  1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f}},

        {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, { 0.0f,  1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, { 1.0f,  1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, { 1.0f,  0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, { 1.0f,  0.0f}},
        {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, { 0.0f,  0.0f}},
        {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, { 0.0f,  1.0f}},

        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, { 1.0f,  1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, { 1.0f,  0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, { 1.0f,  0.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, { 0.0f,  0.0f}},
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, { 0.0f,  1.0f}}
    };

    // load and create a texture 
    // -------------------------
    std::string texture_dir = std::string(TEXTURE_DIR);
    // GLuint texture1 = loadTexture("rainbow-checkerboard.jpg", texture_dir);
    // GLuint texture2 = loadTexture("rainbow-checkerboard-specular.jpg", texture_dir);

    // std::vector<Texture> textures = {{texture1, TT_DIFFUSE}, {texture2, TT_SPECULAR}};
    // std::vector<Texture> textures = {{texture1, TT_DIFFUSE}};
    //
    // Mesh* cube1 = new Mesh(vertices, textures);
    // Mesh* cube2 = new Mesh(vertices, textures);
    // std::vector<Mesh*> cubeVec1 = { cube1 };
    // std::vector<Mesh*> cubeVec2 = { cube2 };
    // Model cubeModel1(cubeVec1);
    // Model cubeModel2(cubeVec2);
    // cubeActor1 = new Actor("cube1", &cubeModel1);
    // cubeActor2 = new Actor("cube2", &cubeModel2);

    Model* backpackModel = new Model((std::string(RESOURCE_DIR) + "/assets/backpack/backpack.obj").c_str());
    Actor* backpack = new Actor("backpack", backpackModel);

    Model* sphereModel = new Model((std::string(RESOURCE_DIR) + "/assets/sphere/sphere.obj").c_str());
    Actor* sphere = new Actor("kleinsphere", sphereModel);

    Light light("l1", {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
    light.setTranslation({-5.0f, 0.0f, 3.0f});

    Scene scene = {
        { backpack },
        { &light },
        &camera
    };
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    pickingShader.use();
    pickingShader.setMat4("projection", projection);
    shdr.use();
    shdr.setMat4("projection", projection);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // create transformations
        // cubeActor1->setTranslationOffset(-camera.getTranslation());
        // cubeActor2->setTranslationOffset(-camera.getTranslation());


        renderer->renderScene(scene);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete all created objects
    // --------------------------
    // delete cubeActor1;
    // delete cubeActor2;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// handle mouse click
// ------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        selectedActor = renderer->getActorAtPosition({ xpos, ypos });

        if (selectedActor != NULL) {
            mousePressed = true;
            currentMode = EM_OBJECT_SELECTED;
            glfwGetCursorPos(window, &lastX, &lastY);
        }
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mousePressed = false;
        currentMode = EM_IDLE;
        selectedActor = NULL;  // Reset selected model ID
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        currentMode = EM_MOVEMENT;
        glfwGetCursorPos(window, &lastX, &lastY);
        firstMouse = true;
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        currentMode = EM_IDLE;
        glfwSetCursorPos(window, lastX, lastY);
    }
}

void cursor_position_callback(GLFWwindow*, double xpos, double ypos) {
    if (currentMode == EM_OBJECT_SELECTED) {
        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.01f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // Update model matrix for the selected model
        selectedActor->addRotation({xoffset, yoffset, 0.0f});
    }
    else if (currentMode == EM_MOVEMENT) {
        float xposloc = static_cast<float>(xpos);
        float yposloc = static_cast<float>(ypos);

        if (firstMouse)
        {
            lastXc = xposloc;
            lastYc = yposloc;
            firstMouse = false;
        }

        float xoffset = xposloc - lastXc;
        float yoffset = lastYc - yposloc; // reversed since y-coordinates go from bottom to top

        lastXc = xposloc;
        lastYc = yposloc;

        camera.processMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow*, double, double yoffset)
{
    camera.processMouseScroll(static_cast<float>(yoffset));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    switch (currentMode) {
        case EM_MOVEMENT:
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera.processKeyboard(CM_FORWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera.processKeyboard(CM_BACKWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera.processKeyboard(CM_LEFT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera.processKeyboard(CM_RIGHT, deltaTime);
            break;
        case EM_OBJECT_SELECTED:
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                selectedActor->addTranslation({0.0f, 0.0f, 0.2f});
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                selectedActor->addTranslation({0.0f, 0.0f, -0.2f});
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                selectedActor->addTranslation({0.2f, 0.0f, 0.0f});
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                selectedActor->addTranslation({-0.2f, 0.0f, 0.0f});

            break;
        default:
            break;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
