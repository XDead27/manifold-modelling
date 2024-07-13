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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// input variables
bool mousePressed = false;
int selectedModelID = -1;
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
GLuint pickingFBO;

// camera
Camera camera("c1");
double lastXc = SCR_WIDTH / 2.0f; 
double lastYc = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Actor* cubeActor;

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
        std::string(SHADER_DIR) + "/simple-cube.fs"
    );

    Shader pickingShader(
        std::string(SHADER_DIR) + "/klein-cube.vs",
        std::string(SHADER_DIR) + "/picking.fs"
    );

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
    GLuint texture1 = loadTexture((std::string(TEXTURE_DIR) + "/rainbow-checkerboard.jpg").c_str());
    GLuint texture2 = loadTexture((std::string(TEXTURE_DIR) + "/rainbow-checkerboard-specular.jpg").c_str());

    std::vector<Texture> textures = {{texture1, TT_DIFFUSE}, {texture2, TT_SPECULAR}};

    Mesh* cube = new Mesh(vertices, textures);
    std::vector<Mesh*> cubeVec = { cube };
    Model cubeModel(cubeVec);
    cubeActor = new Actor("cube1", &cubeModel);

    Light light("l1");
    light.setTranslation({-5.0f, 0.0f, 3.0f});

    // setup picking buffers
    // ---------------------
    GLuint pickingColorBuffer, pickingDepthBuffer;
    glGenFramebuffers(1, &pickingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);

    // create color attachment
    // -----------------------
    glGenTextures(1, &pickingColorBuffer);
    glBindTexture(GL_TEXTURE_2D, pickingColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingColorBuffer, 0);

    // create depth attachment
    // -----------------------
    glGenRenderbuffers(1, &pickingDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, pickingDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pickingDepthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Error: Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        glm::mat4 projection    = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        cubeActor->setTranslationOffset(-camera.getTranslation());

        // first pass: picking framebuffer
        // -------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pickingShader.use();
        pickingShader.setVec3("objectColor", {0.3f, 0.3f, 1.0f});
        pickingShader.setMat4("projection", projection);
        camera.updateShader(pickingShader);

        // pickingShader.setMat4("model", model);
        // cube->Draw(pickingShader);
        cubeActor->updateShader(pickingShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // second pass: default framebuffer
        // --------------------------------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        shdr.use();

        // pass transformation matrices to the shader
        shdr.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

        // set light attributes
        light.updateShader(shdr);
        camera.updateShader(shdr);
        shdr.setFloat("material.shininess", 32);

        // shdr.setMat4("model", model);
        // cube->Draw(shdr);
        cubeActor->updateShader(shdr);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete all created objects
    // --------------------------
    delete cubeActor;

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

        // Read pixel color from picking buffer
        glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        unsigned char data[3];
        glReadPixels(xpos, SCR_HEIGHT - ypos, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Convert color to model ID
        int pickedID = -1;
        if (data[0] == 76 && data[2] == 255) {
            pickedID = 1;
        }

        if (pickedID != -1) {
            mousePressed = true;
            currentMode = EM_OBJECT_SELECTED;
            selectedModelID = pickedID;  // Store selected model ID
            glfwGetCursorPos(window, &lastX, &lastY);
        }
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mousePressed = false;
        currentMode = EM_IDLE;
        selectedModelID = -1;  // Reset selected model ID
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
        cubeActor->addRotation({xoffset, yoffset, 0.0f});
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
                cubeActor->addTranslation({0.0f, 0.0f, 0.2f});
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cubeActor->addTranslation({0.0f, 0.0f, -0.2f});
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cubeActor->addTranslation({0.2f, 0.0f, 0.0f});
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cubeActor->addTranslation({-0.2f, 0.0f, 0.0f});

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
