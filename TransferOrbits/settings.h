#pragma once
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;

#define CONIC_PATCH 400
#define COMPUTATION_MODE 0 // set 1 to write dv data to file

//globals
GLFWwindow* window;
Camera camera;
glm::mat4 projection = glm::mat4(1);
glm::mat4 view = glm::mat4(1);
glm::mat4 model = glm::mat4(1);


float rot = 0.001f;
float times = 0;
int warp_speed = 200;
bool state = false;
bool launched = false;
int launch = -1;

void HandleSettings(GLFWwindow* window, Camera& camera) {
    float tr = 0.001f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.Translate(camera.camera_basis.basis[2] * tr);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.Translate(camera.camera_basis.basis[2] * (-tr));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.Translate(camera.camera_basis.basis[0] * -tr);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.Translate(camera.camera_basis.basis[0] * (tr));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glm::vec4 vec = glm::vec4(camera.camera_basis.basis[2]);
        vec.w = 0.001;
        camera.Rotate(vec);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        glm::vec4 vec = glm::vec4(camera.camera_basis.basis[2]);
        vec.w = -0.001;
        camera.Rotate(vec);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.Translate(camera.camera_basis.basis[1] * (-tr));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.Translate(camera.camera_basis.basis[1] * (tr));
    }
}

void PollControls(Camera& cam, GLFWwindow* window) {
    float sensitivity = .5;
    double xpos, ypos;
    float angle;
    glfwGetCursorPos(window, &xpos, &ypos);
    angle = (ypos - 500) / 1940 * sensitivity;
    glm::vec4 axisT = cam.camera_basis.basis * glm::vec4(1, 0, 0, 0);
    axisT.w = angle;
    cam.Rotate(glm::vec4(axisT));
    angle = (xpos - 500) / 1940 * sensitivity;
    glm::vec4 axisB = cam.camera_basis.basis * glm::vec4(0, 1, 0, 0);
    axisB.w = angle;
    cam.Rotate(axisB);
    glfwSetCursorPos(window, 500, 500);
}

void processInput(GLFWwindow* window)
{
    float tr = 0.01f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        state = 1;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        state = 0;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && launch != true)
        launch = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && launch != true)
        launch = 2;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && launch != true)
        launch = 3;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && launch != true)
        launch = 4;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && launch != true)
        launch = 5;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && launch != true)
        warp_speed += 10;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && launch != true)
        warp_speed -= 10;
}