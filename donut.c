#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linalg.h"

#define FOV 90
#define width 160
#define height 44
#define grayscale_map " .:-=+*#%@"
#define PI 3.1415926535897932384626433832795
#define dist width / (2 * tan(FOV * PI / 360))
#define clamp(x, min, max) ((x < min) ? min : ((x > max) ? max : x))

const struct vector light = {0, 20, 30};

int project(struct vector point) {
    int screenx = (int)(width / 2 + 2 * point.x * dist / point.z);
    int screeny = (int)(height / 2 - point.y * dist / point.z);
    return (screenx >= 0 && screenx < width && screeny >= 0 && screeny < height)
               ? screeny * width + screenx
               : -1;
}

int shade(struct vector point, struct vector normal) {
    float L = dot(normal, normalize(sub(light, point)));
    int grayscale_index = (int)(L * strlen(grayscale_map));
    return clamp(grayscale_index, 0, strlen(grayscale_map) - 1);
}

void donut(char* buffer, float R1, float R2, struct vector pos, struct matrix rotation_matrix) {
    float zbuffer[width * height];
    memset(zbuffer, 0, width * height * sizeof(float));
    for (float theta = 0; theta < 2 * PI; theta += 0.01) {
        float ct = cos(theta);
        float st = sin(theta);
        for (float phi = 0; phi < 2 * PI; phi += 0.03) {
            float cp = cos(phi);
            float sp = sin(phi);

            struct vector point = {(R2 + R1 * cp) * ct, (R2 + R1 * cp) * st, R1 * sp};
            point = add(mult(rotation_matrix, point), pos);

            float zinv = 1.0 / point.z;
            int buffer_index = project(point);
            if (buffer_index == -1 || zinv <= zbuffer[buffer_index]) continue;

            struct vector normal = {cp * ct, cp * st, sp};
            normal = mult(rotation_matrix, normal);

            zbuffer[buffer_index] = zinv;
            buffer[buffer_index] = grayscale_map[shade(point, normal)];
        }
    }
}

void renderframe(char* buffer) {
    printf("\x1b[H");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            putchar(buffer[y * width + x]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    printf("\x1b[2J");
    printf("\x1b[?25l");
    char buffer[width * height];

    struct vector donut_pos = {0, 0, 60};
    struct vector donut_rotation_angles = {0, 0, 0};
    struct vector donut_increment_angles = {0.06, 0.09, 0.04};

    while (1) {
        memset(buffer, ' ', width * height * sizeof(char));
        donut_rotation_angles = add(donut_rotation_angles, donut_increment_angles);
        struct matrix donut_rotation = get_rotation(donut_rotation_angles);
        donut(buffer, 5, 10, donut_pos, donut_rotation);
        renderframe(buffer);
    }
    return 0;
}