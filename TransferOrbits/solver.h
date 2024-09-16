#pragma once

/*
Lambert solver using universal variables
algorithm by 
https://www.researchgate.net/publication/236012521_Lambert_Universal_Variable_Algorithm
*/
struct vec4 {
    float x, y, z, w;
    vec4 operator *(float s) {
        return { x * s, y * s, z * s, w * z };
    }
    vec4 operator /(float s) {
        return { x / s, y / s, z / s, w / z };
    }
    vec4 operator -(vec4 rhs) {
        return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
    }
    vec4 operator +(vec4 rhs) {
        return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
    }
    float len() {
        return x * x + y * y + z * z;
    }
};

vec4 cross(vec4 a, vec4 b) {
    return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 0 };
}
float dot(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float a1(float phi, float H0, float n, float ep, int M) {
    float H = H0;
    float C = H;
    float K = 2;
    float L = n;
    int N = 0;
    for (int i = 0; i < M; i++) {
        H = -1.0f * phi / (2 * K * L) * H;
        if (abs(H) < ep) {
            N = i;
            break;
        }
        C += H;
        K += 1;
        L += 2;
    }
    return C;
}

float c22(float psi) {
    return (1 - cos(sqrt(psi))) / psi;
}
float c33(float psi) {
    return (sqrt(psi) - sin(sqrt(psi))) / (psi * sqrt(psi));
}
struct c {
    float c2, c3;
};
c a2(float phi, float phi_max) {
    float K = 0;
    for (int i = 0; i < 400; i++) {
        if (abs(phi) < phi_max) break;
        K += 1;
        phi *= 0.25f;
    }
    float C2 = a1(phi, 0.5f, 3, 1e-7, 200);
    float C3 = a1(phi, 1.0f / 6.0f, 5, 1e-7, 200);
step:
    if (K == 0) return { C2, C3 };
    K -= 1;
    float c2 = 0.5f * (1 - phi * C3) * (1 - phi * C3);
    float c3 = (C2 + C3 - phi * C2 * C3) / 4;
    C2 = c2;
    C3 = c3;
    phi *= 4;
    goto step;
}
vec4 uv_lambert(vec4 r1, vec4 r2, float delt, float mu, int tm = +1, float tol = 1e-6, float psi = 0.8f) {
    float r0 = sqrt(r1.len());
    float r = sqrt(r2.len());
    float gamma = dot(r1, r2) / (r0 * r);
    float beta = tm * sqrt(1 - gamma * gamma);
    float A = tm * sqrt(r0 * r * (1 + gamma));
    float psi_l = -4 * 3.141592653 * 3.141592653;
    float psi_max = 4 * 3.141592653 * 3.141592653;
    float B = 0;
    if (A == 0) return { -1, -1, -1, -1 };
    for (int i = 0; i < 400; i++) {
        float c2 = a2(psi, psi_max).c2;
        float c3 = a2(psi, psi_max).c3;
        B = r0 + r + A * (psi * c3 - 1) / sqrt(c2);
        if (A > 0.0 && B < 0.0) {
            psi_l += 3.141592653462643;
            B *= -1;
        }
        float xi = sqrt(B / c2);
        float dt = 1.0f * (xi*xi*xi * c3 + A * sqrt(B));

        if (abs(delt - dt) < tol) {
            break;
        }
        else {
            if (dt <= delt) {
                psi_l = psi;
            }
            else {
                psi_max = psi;
            }
            psi = 0.5f * (psi_l + psi_max);

        }
    }
    float F = 1 - B / r0;
    float G = A * sqrt(B);
    float Gdot = 1 - B / r;
    vec4 dv = (r2 - (r1 * F)) / G;
    return dv;
}
glm::vec3 solve(glm::vec3 r11, glm::vec3 r22, glm::vec3 ve, float transfer_time)
{
    vec4 r1 = { r11.x, r11.y, r11.z, 0.0 };
    vec4 r2 = { r22.x, r22.y, r22.z, 0.0 };
    vec4 vearth = { ve.x, ve.y, ve.z, 0.0 };
    vec4 deltav = { 0, 0, 0, 0 };
    vec4 del;
    float min = 0xFFFFFFFF;
    int days_min = -1;   
    float dv = 0;
    del = uv_lambert(r1, r2, (float)transfer_time / 58.1324408f, 1) * 0.01720209895;
    dv = (vearth - del).len();
    if (dv < min) {
        min = dv;
        days_min = transfer_time;
        deltav = del;
    }  
    return glm::vec3(deltav.x, deltav.y, deltav.z );
}