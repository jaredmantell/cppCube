#include "cube.h"

#include "common.h"



void drawCube() {

    for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {

        for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {

            calculateForSurface(cubeX, cubeY, -cubeWidth, '.');

            calculateForSurface(cubeWidth, cubeY, cubeX, '$');

            calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');

            calculateForSurface(-cubeX, cubeY, cubeWidth, '#');

            calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');

            calculateForSurface(cubeX, cubeWidth, cubeY, '+');

        }

    }

}