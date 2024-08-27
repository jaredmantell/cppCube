#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <windows.h>

// Function declarations
void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch);
float calculateX(int i, int j, int k);
float calculateY(int i, int j, int k);
float calculateZ(int i, int j, int k);

// External variable declarations
extern float A, B, C;
extern float cubeWidth;
extern int width, height;
extern float zBuffer[];
extern char buffer[];
extern int distanceFromCam;
extern float K1;
extern float incrementSpeed;
extern int backgroundASCIICode;

// If you're using these functions in multiple files, declare them here
void drawCube();
void drawPyramid();
void drawDonut();

#endif // COMMON_H