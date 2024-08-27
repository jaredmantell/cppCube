#include "donut.h"

#include "common.h"

#include <cmath>

#include <iostream>



void drawDonut() {

    float i, j;

    int k;

    float z[160 * 44];

    char b[160 * 44];



    memset(b, 32, 160 * 44);

    memset(z, 0, 160 * 44 * sizeof(float));



    // Draw one large donut
    for (j = 0; j < 6.28f; j += 0.07f) {
        for (i = 0; i < 6.28f; i += 0.02f) {

            float c = sin(i);

            float d = cos(j);

            float e = sin(A);

            float f = sin(j);

            float g = cos(A);

            float h = d + 2;

            float D = 1 / (c * h * e + f * g + 5);

            float l = cos(i);

            float m = cos(B);

            float n = sin(B);



            float t = c * h * g - f * e;

            int x = static_cast<int>(80 + 30 * D * (l * h * m - t * n));
            int y = static_cast<int>(22 + 15 * D * (l * h * n + t * m));
            int N = static_cast<int>(8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n));

            int o = x + 160 * y;

            if (44 > y && y > 0 && x > 0 && 160 > x && D > z[o]) {

                z[o] = D;

                b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];

            }

        }

    }



    // Update buffer

    for (k = 0; k < width * height; k++) {

        buffer[k] = b[k];

    }

}