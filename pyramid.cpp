#include "pyramid.h"
#include "common.h"
#include <cmath>
#include <array>
#include <algorithm>
#include <vector>
#include <limits>
#undef min
#undef max

const std::array<std::array<float, 4>, 5> vertices = {{
    {-cubeWidth, -cubeWidth, -cubeWidth, 1},
    {cubeWidth, -cubeWidth, -cubeWidth, 1},
    {cubeWidth, -cubeWidth, cubeWidth, 1},
    {-cubeWidth, -cubeWidth, cubeWidth, 1},
    {0, cubeWidth * 1.5f, 0, 1}
}};

std::array<std::array<float, 4>, 4> rotationMatrix() {
    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);
    float cosC = cos(C), sinC = sin(C);

    return {{
        {cosB*cosC, sinA*sinB*cosC - cosA*sinC, cosA*sinB*cosC + sinA*sinC, 0},
        {cosB*sinC, sinA*sinB*sinC + cosA*cosC, cosA*sinB*sinC - sinA*cosC, 0},
        {-sinB, sinA*cosB, cosA*cosB, 0},
        {0, 0, 0, 1}
    }};
}

std::array<float, 4> multiplyMatrixVector(const std::array<std::array<float, 4>, 4>& m, const std::array<float, 4>& v) {
    std::array<float, 4> result = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i] += m[i][j] * v[j];
        }
    }
    return result;
}

std::array<float, 3> calculateNormal(const std::array<float, 4>& v1, const std::array<float, 4>& v2, const std::array<float, 4>& v3) {
    float ax = v2[0] - v1[0], ay = v2[1] - v1[1], az = v2[2] - v1[2];
    float bx = v3[0] - v1[0], by = v3[1] - v1[1], bz = v3[2] - v1[2];
    float nx = ay * bz - az * by;
    float ny = az * bx - ax * bz;
    float nz = ax * by - ay * bx;
    float length = std::sqrt(nx*nx + ny*ny + nz*nz);
    return {nx/length, ny/length, nz/length};
}

bool isFaceVisible(const std::array<float, 3>& normal, const std::array<float, 4>& v) {
    return normal[2] < 0; // Simplified visibility check
}

char selectChar(float x, float y, float z, const std::array<float, 3>& normal, bool isEdge, bool isBase) {
    if (isBase) return '@';
    if (isEdge) return '/';
    return '.';
}

void drawPyramid() {
    auto rotation = rotationMatrix();

    // Rotate vertices
    std::array<std::array<float, 4>, 5> transformedVertices;
    for (int i = 0; i < 5; i++) {
        transformedVertices[i] = multiplyMatrixVector(rotation, vertices[i]);
    }

    // Define faces (base and 4 triangular faces)
    std::vector<std::array<int, 3>> faces = {
            {0, 1, 2}, {0, 2, 3},  // base
            {0, 1, 4}, {1, 2, 4}, {2, 3, 4}, {3, 0, 4}  // sides
    };

    // Draw faces
    for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++) {
        const auto& face = faces[faceIndex];
        auto normal = calculateNormal(transformedVertices[face[0]], transformedVertices[face[1]], transformedVertices[face[2]]);

        // Always draw all faces
        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();

        for (int i = 0; i < 3; ++i) {
            minX = (std::min)(minX, transformedVertices[face[i]][0]);
            maxX = (std::max)(maxX, transformedVertices[face[i]][0]);
            minY = (std::min)(minY, transformedVertices[face[i]][1]);
            maxY = (std::max)(maxY, transformedVertices[face[i]][1]);
        }

        // Rasterize the face
        for (float x = minX; x <= maxX; x += incrementSpeed) {
            for (float y = minY; y <= maxY; y += incrementSpeed) {
                // Barycentric coordinates check
                float w0 = ((transformedVertices[face[1]][1] - transformedVertices[face[2]][1]) * (x - transformedVertices[face[2]][0]) +
                            (transformedVertices[face[2]][0] - transformedVertices[face[1]][0]) * (y - transformedVertices[face[2]][1])) /
                           ((transformedVertices[face[1]][1] - transformedVertices[face[2]][1]) * (transformedVertices[face[0]][0] - transformedVertices[face[2]][0]) +
                            (transformedVertices[face[2]][0] - transformedVertices[face[1]][0]) * (transformedVertices[face[0]][1] - transformedVertices[face[2]][1]));
                float w1 = ((transformedVertices[face[2]][1] - transformedVertices[face[0]][1]) * (x - transformedVertices[face[2]][0]) +
                            (transformedVertices[face[0]][0] - transformedVertices[face[2]][0]) * (y - transformedVertices[face[2]][1])) /
                           ((transformedVertices[face[1]][1] - transformedVertices[face[2]][1]) * (transformedVertices[face[0]][0] - transformedVertices[face[2]][0]) +
                            (transformedVertices[face[2]][0] - transformedVertices[face[1]][0]) * (transformedVertices[face[0]][1] - transformedVertices[face[2]][1]));
                float w2 = 1 - w0 - w1;

                if (w0 >= -0.01 && w1 >= -0.01 && w2 >= -0.01) {  // Slight tolerance for edge cases
                    float z = w0 * transformedVertices[face[0]][2] + w1 * transformedVertices[face[1]][2] + w2 * transformedVertices[face[2]][2];

                    // Check if point is on edge
                    bool isEdge = (w0 < 0.05 || w1 < 0.05 || w2 < 0.05);
                    bool isBase = (faceIndex < 2);  // First two faces are the base

                    char ch = selectChar(x, y, z, normal, isEdge, isBase);
                    calculateForSurface(x, y, z, ch);
                }
            }
        }
    }
}
