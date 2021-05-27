#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
void display( const char *output, float &a, float &b) {
    printf("\x1b[H");
    for (int i = 0; i < 1761; i++) {
        //1760 corresponds to buffer size
        putchar(i % 80 ? output[i] : 10);
        a += 0.00004;
        b += 0.00002;
    }
}

void render(float a, float b, int buffer_size, float *zBuffer, char *output, int height, int width) {
    int outer_size = width / 40;
    //radius of outer circle
    char * lum_chars = ".,-~:;=!*#$@";
    //characters corresponding to different amounts of brightness;
    memset(output, ' ',buffer_size);
    memset(zBuffer, 0, buffer_size * sizeof(float));
    for (float theta = 0; theta < 6.28; theta += 0.07) {
        for (float phi = 0; phi < 6.28; phi += 0.02) {
            /**Sin and cos of a and b and represent starting points,
             * Phi corresponds to rotation of center of revolution
             * Theta corresponds to cross sectional of the circleY*/
            float sinPhi = sin(phi);
            float cosTheta = cos(theta);
            float sinA = sin(a);
            float sinTheta = sin(theta);
            float cosA = cos(a);
            float circleX = cosTheta + outer_size;
            float circleY = sinPhi * circleX * cosA - sinTheta * sinA;
            //x, y coord before rotating
            float norm_z = 1 / (sinPhi * circleX * sinA + sinTheta * cosA + 5);
            //normalized z, in this respect, z is a line from the monitor straight to your face, normalized for consistent output
            float cosPhi = cos(phi);
            float cosB = cos(b);
            float sinB = sin(b);
            int x = (width / 2) + 30 * norm_z * (cosPhi * circleX * cosB + circleY * sinB);
            int y = (height / 2 + 1) + 15 * norm_z * (cosPhi * circleX * sinB - circleY * cosB);
            //y is negative, pixel 1920 x 760 is in the top right corner of your screen, as you move to the bottom of your screen, y decreases
            int o = x + width * y;
            //gives cross sections of donut(level curves if you're familiar with multivariable calculus)
            int lum = 8 * ((sinTheta * sinA - sinPhi * cosTheta * cosA) * cosB - sinPhi * cosTheta * sinA - sinTheta * cosA - cosPhi * cosTheta * sinB);
            //luminance
            if (height > y && y > 0 && x > 0 && width > x && norm_z > zBuffer[o]) {
                zBuffer[o] = norm_z;
                output[o] = lum_chars[lum > 0 ? lum : 0];
            }
        }
    }
}
int main() {
    float	a = 0, b = 0;
    int height = 22;
    int width = 80;
    int buffer_size = height * width;
    float zBuffer[buffer_size];
    char output[buffer_size];

    printf("\x1b[2J");
    for (;;) {
        render(a, b, buffer_size, zBuffer, output, height, width);
        display(output, a, b);
    }
    return 0;
}
