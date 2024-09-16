#pragma once
struct planet {
    glm::vec3 pos, vel;
    int id;
};

struct ob {
    glm::vec3 v1, v2;
};

float t = 0;

//function for generating conics
void init_planets(int size) {
    GLuint vxb, vyb, vzb;
    float* arr = (float*)calloc(size * CONIC_PATCH, sizeof(float));
    int n = size * CONIC_PATCH;
    glGenBuffers(1, &vxb);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vxb);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vxb);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * n, arr, GL_STATIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vxb);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind   
    glGenBuffers(1, &vyb);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vyb);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vyb);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * n, arr, GL_STATIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, vyb);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glGenBuffers(1, &vzb);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vzb);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, vzb);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * n, arr, GL_STATIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vzb);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    free(arr);
}

void sendPos(glm::vec3 pos, uint ID) {
    glUniform1f(glGetUniformLocation(ID, "x"), pos.x);
    glUniform1f(glGetUniformLocation(ID, "y"), pos.y);
    glUniform1f(glGetUniformLocation(ID, "z"), pos.z);
}

glm::vec3 minimum_dv_transfer(orbit& b1, orbit& b2, int time_upper = 300, int time_lower = 14) {
    float delta_v = 0xFFFFFFFF;
    int time = -1;
    glm::vec3 dv, min_dv;
    for (int i = time_lower; i < time_upper; i++) {
        dv = solve(b1.r * (1.0f / R_NORM), b2.r * (1.0F / R_NORM), b1.v * (1.0F / (float)V_NORM), i);
        if (glm::dot(dv * 10.0f - b1.v * (1.0F / (float)V_NORM), dv * 10.0f - b1.v * (1.0F / (float)V_NORM)) < delta_v) {
            min_dv = dv;
            delta_v = glm::dot(dv * 10.0f - b1.v * (1.0F / (float)V_NORM), dv * 10.0f - b1.v * (1.0F / (float)V_NORM));
            time = i;
        }
    }
    std::cout << "dV = " << sqrt(delta_v) * V_NORM << " " << time << "\n";
    return min_dv;
}


void simulate_forward(float days, planet& planet) {
    float dt = .001f;
    float epoch = (float)days / dt * 0.1f;
    for (int i = 0; i < epoch; i++) {
        planet.pos = planet.pos + (planet.vel * dt);
        planet.vel = planet.vel + (planet.pos * (float)(-dt * 0.0295912208 / (glm::dot(planet.pos, planet.pos) * sqrt(glm::dot(planet.pos, planet.pos)))));
        t += dt;
    }
}
ob intercept_transfer_minimum(orbit& b1, orbit& b2, int time_upper = 300, int time_lower = 50) {
    //for each tranfer time, the end location must match the final location of the body. 
    planet venus = { b2.r * (float)(1.0f / R_NORM), b2.v * (float)(1.0f / V_NORM) , 0 };
    t = 0;
    float delta_v = 0xFFFFFFFFFF;
    int time = -1;
    glm::vec3 dv, min_dv;
    glm::vec3 intercept = glm::vec3(0);
    simulate_forward(time_lower, venus);
    for (int i = time_lower; i < time_upper; i++) {
        simulate_forward(1, venus);
        dv = solve(b1.r * (float)(1.0f / R_NORM), venus.pos, b1.v * (float)(1.0f / V_NORM), (float)i);
        if (glm::dot(dv * 10.0f - b1.v * (float)(1.0f / V_NORM), dv * 10.0f - b1.v * (float)(1.0f / V_NORM)) < delta_v) {
            min_dv = dv;
            delta_v = glm::dot(dv * 10.0f - b1.v * (float)(1.0f / V_NORM), dv * 10.0f - b1.v * (float)(1.0f / V_NORM));
            time = i;
            intercept = venus.pos;
        }
    }
    //std::cout << sqrt(glm::dot(min_dv * 10.0f - b1.v * (float)(1.0f / V_NORM), min_dv * 10.0f - b1.v * (float)(1.0f / V_NORM))) * V_NORM << "|" << time << "\n";
    return { min_dv * 10.0f - b1.v * (float)(1.0f / V_NORM), glm::vec3(time, 0, 0) };
}
ob intercept_transfer_minimum(orbit& b1, orbit& b2, bool normalize, int time_upper = 400, int time_lower = 50) {
    //for each tranfer time, the end location must match the final location of the body. 
    planet venus = { b2.r, b2.v , 0 };
    t = 0;
    float delta_v = 0xFFFFFFFFFF;
    int time = -1;
    glm::vec3 dv, min_dv;
    glm::vec3 intercept = glm::vec3(0);
    simulate_forward(time_lower, venus);
    for (int i = time_lower; i < time_upper; i++) {
        simulate_forward(1, venus);
        dv = solve(b1.r, venus.pos, b1.v, (float)i);
        if (glm::dot(dv * 10.0f - b1.v, dv * 10.0f - b1.v) < delta_v) {
            min_dv = dv;
            delta_v = glm::dot(dv * 10.0f - b1.v, dv * 10.0f - b1.v);
            time = i;
            intercept = venus.pos;
        }
    }
    std::cout << "dV = " << sqrt(delta_v) * V_NORM << " " << time << "\n";
    return { min_dv, intercept };
}
void minimum_delta_v(planet* planets, orbit& b1, orbit& b2, int time_upper = 200, int time_lower = 50) {
    float minimum = 0xFFFFFFFFFF;
    ofstream output("data.txt");
    ofstream output2("annomally.txt");
    ofstream output3("time.txt");
    for (int i = 0; i < 360; i++) {
        set_annomaly(b2, i);
        ob object = intercept_transfer_minimum(b1, b2);
        glm::vec3 vel = object.v1;
        output << sqrt(glm::dot(vel, vel)) * V_NORM << "\n";
        output2 << i << "\n";
        output3 << object.v2.x << "\n";
        if (glm::dot(vel, vel) < minimum) minimum = glm::dot(vel, vel);
    }
    std::cout << "minimum transfer dv =>" << sqrt(minimum) * V_NORM << "\n";
}
planet* gen_planets() {
    planet* planets = (planet*)malloc(sizeof(planet) * 6);
    orbit Earth = get(earth::inclination, earth::ecentricity, earth::s_major, earth::longitudeOfAscendingNode, earth::argumentOfPeriapsis);
    orbit Mars = get(mars::inclination, mars::ecentricity, mars::s_major, mars::longitudeOfAscendingNode, mars::argumentOfPeriapsis);
    orbit Venus = get(venus::inclination, venus::ecentricity, venus::s_major, venus::longitudeOfAscendingNode, venus::argumentOfPeriapsis);
    orbit Mercury = get(mercury::inclination, mercury::ecentricity, mercury::s_major, mercury::longitudeOfAscendingNode, mercury::argumentOfPeriapsis);
    orbit Jupiter = get(jupiter::inclination, jupiter::ecentricity, jupiter::s_major, jupiter::longitudeOfAscendingNode, jupiter::argumentOfPeriapsis);
    set_annomaly(Venus, 0.0f);
    set_annomaly(Earth, 0.0f);
    //ob object = intercept_transfer_minimum(Earth, Venus);
    glm::vec3 v = Earth.v * 0.1f * (float)(1.0f / V_NORM);
    planets[1] = { Earth.r * (1.0f / R_NORM) , Earth.v * (float)(1.0f / V_NORM) , 0 };
    planets[0] = { { Earth.r * (1.0f / R_NORM)}, {v.x * 10 , v.y * 10, v.z * 10 } , 1 };
    planets[2] = { Venus.r * (float)(1.0f / R_NORM),  Venus.v * (float)(1.0f / V_NORM) , 2 };
    planets[3] = { Mars.r * (float)(1.0f / R_NORM),  Mars.v * (float)(1.0f / V_NORM) , 3 };
    planets[4] = { Mercury.r * (float)(1.0f / R_NORM),  Mercury.v * (float)(1.0f / V_NORM) , 4 };
    planets[5] = { Jupiter.r * (float)(1.0f / R_NORM),  Jupiter.v * (float)(1.0f / V_NORM) , 5 };
    //change this to any planet of interest
    COMPUTATION_MODE ? minimum_delta_v(planets, Earth, Mars) : void();
    init_planets(12);
    return planets;
}
orbit* get_orbits() {
    orbit* orbits = (orbit*)malloc(sizeof(orbit) * 5);
    orbits == NULL ? throw exception() : void();
    orbits[0] = get(earth::inclination, earth::ecentricity, earth::s_major, earth::longitudeOfAscendingNode, earth::argumentOfPeriapsis);
    orbits[1] = get(mars::inclination, mars::ecentricity, mars::s_major, mars::longitudeOfAscendingNode, mars::argumentOfPeriapsis);
    orbits[2] = get(venus::inclination, venus::ecentricity, venus::s_major, venus::longitudeOfAscendingNode, venus::argumentOfPeriapsis);
    orbits[3] = get(mercury::inclination, mercury::ecentricity, mercury::s_major, mercury::longitudeOfAscendingNode, mercury::argumentOfPeriapsis);
    orbits[3] = get(jupiter::inclination, jupiter::ecentricity, jupiter::s_major, jupiter::longitudeOfAscendingNode, jupiter::argumentOfPeriapsis);
    return orbits;
}