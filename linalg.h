#ifndef LINALG_H
#define LINALG_H

#include <math.h>

struct vector {
    float x, y, z;
};

struct matrix {
    float m[3][3];
};

/**
 * @brief Add two vectors together
 * @param a first vector
 * @param b second vector
 * @return vector containing the sum of the components of the two input vectors
 */
static inline struct vector add(struct vector a, struct vector b) {
    return (struct vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

/**
 * @brief Subtract vector b from vector a
 * @param a first vector
 * @param b second vector
 * @return vector containing the difference of the components of the two input vectors
 */
static inline struct vector sub(struct vector a, struct vector b) {
    return (struct vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

/**
 * @brief Normalize a vector to length 1
 * @param a vector to be normalized
 * @return vector with the same direction as the input vector, but length 1
 */
static inline struct vector normalize(struct vector a) {
    float len = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    return (struct vector){a.x / len, a.y / len, a.z / len};
}

/**
 * @brief Calculate the dot product of two vectors
 * @param a first vector
 * @param b second vector
 * @return dot product of the two input vectors
 */
static inline float dot(struct vector a, struct vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * @brief Multiply a matrix and a vector
 * @param a matrix to be multiplied
 * @param b vector to be multiplied
 * @return vector containing the result of the multiplication
 */
static inline struct vector mult(struct matrix a, struct vector b) {
    return (struct vector){
        a.m[0][0] * b.x + a.m[0][1] * b.y + a.m[0][2] * b.z,
        a.m[1][0] * b.x + a.m[1][1] * b.y + a.m[1][2] * b.z,
        a.m[2][0] * b.x + a.m[2][1] * b.y + a.m[2][2] * b.z};
}

/**
 * @brief Generate a rotation matrix given a vector of angles
 * @param angles vector containing rotation angles about the x, y, and z axes
 * @return 3x3 matrix representing the rotation specified by the input vector
 * @note The rotation matrix is generated using the x -> y -> z convention
 */
static inline struct matrix get_rotation(struct vector angles) {
    float sx = sin(angles.x), sy = sin(angles.y), sz = sin(angles.z);
    float cx = cos(angles.x), cy = cos(angles.y), cz = cos(angles.z);
    return (struct matrix){{{cy * cz, cz * sx * sy - cx * sz, cx * cz * sy + sx * sz},
                            {cy * sz, cx * cz + sx * sy * sz, -cz * sx + cx * sy * sz},
                            {-sy, cy * sx, cx * cy}}};
}

#endif
