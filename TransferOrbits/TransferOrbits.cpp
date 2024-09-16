#include <iostream>
#include <string>
#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "Buffer.h"
#include "grid.h"
#include "cube.h"
#include "conics.h"
#include "transformations.h"
#include "Item.h"
#include "Features.h"
#include "solver.h"
#include "venus.h"
#include "settings.h"
#include "planets.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


void graph_data(orbit& b1, orbit& b2, planet* planets) {
    COMPUTATION_MODE ? minimum_delta_v(planets, b1, b2) : void();
}

 GLFWwindow* createWindow() {
     glfwInit();
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
     GLFWwindow* win = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Orbits", NULL, NULL);
     win == NULL ? throw std::exception() : NULL;
     glfwMakeContextCurrent(win);
     !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ? throw std::exception() : NULL;
     return win;
 }

 void handle_trajectory(planet* planets, orbit* orbits,  int id) {
     orbit Earth = get(earth::inclination, earth::ecentricity, earth::s_major, earth::longitudeOfAscendingNode, earth::argumentOfPeriapsis);
     orbit destination = orbits[id];
     Earth.r = planets[1].pos;
     destination.r = planets[id].pos;
     Earth.v = planets[1].vel;
     destination.v = planets[id].vel;
     ob object = intercept_transfer_minimum(Earth, destination, true);
     planets[0].pos = Earth.r;
     planets[0].vel = object.v1 * 10.0f;
 }

int main()
{
    // glfw window creation
    window = createWindow();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    camera = Features::GetCamera();
    //shaders
    Shader vertex("vertex.glsl", "fragment.glsl");
    Shader vertex2("vertex2.glsl", "fragment.glsl");
    Cshader compute("compute.glsl");
    Grid gd = grid(2);
    Grid gds = grid(100);
    Buffer buff((void*)gd.vertices,   (void*)gd.indices, gd.v_size, gd.i_size);
    Buffer buff2((void*)gds.vertices, (void*)gds.indices, gds.v_size, gds.i_size);
    planet* planets = gen_planets();
    orbit* orbits = get_orbits();
    int size = 5; // planets + S/C
    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 inverse = glm::mat4(1);
        inverse[0] = camera.camera_basis.basis[0];
        inverse[1] = camera.camera_basis.basis[1];
        inverse[2] = camera.camera_basis.basis[2];
        inverse[3] = glm::vec4(0, 0, 0, 1);
        inverse = glm::transpose(inverse);
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(vertex.ID);
        glBindVertexArray(buff.VAO); 
        glUniformMatrix4fv(glGetUniformLocation(vertex.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniform3fv(glGetUniformLocation(vertex.ID, "camera_global"), 1, glm::value_ptr(glm::vec3(camera.camera_basis.basis[3].x, camera.camera_basis.basis[3].y, camera.camera_basis.basis[3].z)));
        glUniformMatrix4fv(glGetUniformLocation(vertex.ID, "view_direction"), 1, GL_FALSE, glm::value_ptr(inverse));
        for (int i = 0; i < size; i++) {
            sendPos(planets[i].pos, vertex.ID);
            glDrawElements(GL_TRIANGLES, gd.v_size, GL_UNSIGNED_INT, 0);
        }           
        if (launch != -1) {
            handle_trajectory(planets, orbits, launch);
            launch = -1;
            launched = true;
        }
        if (state) {
            for (int i = 0; i < warp_speed; i++) {
                float dt = .00001f;
                for (int j = 0; j < size; j++) {
                    planets[j].pos = planets[j].pos + (planets[j].vel * dt);
                    planets[j].vel = planets[j].vel + (planets[j].pos * (float)(-dt * 0.0295912208 / (glm::dot(planets[j].pos, planets[j].pos) * sqrt(glm::dot(planets[j].pos, planets[j].pos)))));
                }
                times += dt;
            }
        }         
        for (int i = 0; i < size; i++) {             
            glUseProgram(compute.ID);
            glUniform1f(glGetUniformLocation(compute.ID, "x"), planets[i].pos.x );
            glUniform1f(glGetUniformLocation(compute.ID, "y"), planets[i].pos.y );
            glUniform1f(glGetUniformLocation(compute.ID, "z"), planets[i].pos.z );
            glUniform1i(glGetUniformLocation(compute.ID, "id"),planets[i].id);
            glDispatchCompute(1, 1, 1);
            glUseProgram(vertex2.ID);
            glUniformMatrix4fv(glGetUniformLocation(vertex2.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform3fv(glGetUniformLocation(vertex2.ID, "camera_global"), 1, glm::value_ptr(glm::vec3(camera.camera_basis.basis[3].x, camera.camera_basis.basis[3].y, camera.camera_basis.basis[3].z)));
            glUniformMatrix4fv(glGetUniformLocation(vertex2.ID, "view_direction"), 1, GL_FALSE, glm::value_ptr(inverse));
            glBindVertexArray(buff2.VAO); 
            glUniform1f(glGetUniformLocation(vertex2.ID, "id"), planets[i].id);
            glDrawArrays(GL_LINE_STRIP, planets[i].id * 400, 400);
        }           
        glfwSwapBuffers(window);
        glfwPollEvents();
        HandleSettings(window, camera);
        PollControls(camera, window);
    }
    free(planets);
    free(orbits);
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


