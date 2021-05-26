#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
void display( const char *b, float &A, float &B) {
    printf("\x1b[H");
    for (int k = 0; k < 1761; k++) {
        //1760 corresponds to buffer sizer
        putchar(k % 80 ? b[k] : 10);
        A += 0.00004;
        B += 0.00002;
    }
    usleep(30000);
}

void render(float A, float B, int buffer_size, float *zBuffer, char *output, int height, int width) {
    int outer_size = width / 40;
    //radius of outer circle
    char * lum_chars = ".,-~:;=!*#$@";
    //characters corresponding to different amounts of brightness;
    memset(output, ' ',buffer_size);
    memset(zBuffer, 0, buffer_size * sizeof(float));
    for (float theta = 0; theta < 6.28; theta += 0.07) {
        for (float phi = 0; phi < 6.28; phi += 0.02) {
            /**Sin and cos of A and B and represent starting points,
             * Phi corresponds to rotation of center of revolution
             * Theta corresponds to cross sectional of the donut*/
            float sinPhi = sin(phi);
            float cosTheta = cos(theta);
            float sinA = sin(A);
            float sinTheta = sin(theta);
            float cosA = cos(A);
            float h = cosTheta + outer_size;
            //radius of outer circle
            float depth = 1 / (sinPhi * h * sinA + sinTheta * cosA + 5);
            //depth or normalized z
            float cosPhi = cos(phi);
            float cosB = cos(B);
            float sinB = sin(B);
            float donut = sinPhi * h * cosA - sinTheta * sinA;
            int x = (width / 2) + 30 * depth * (cosPhi * h * cosB - donut * sinB);
            int y = (height / 2 + 1) + 15 * depth * (cosPhi * h * sinB + donut * cosB);
            int circle = x + width * y;
            //rotation
            int lum = 8 * ((sinTheta * sinA - sinPhi * cosTheta * cosA) * cosB - sinPhi * cosTheta * sinA - sinTheta * cosA - cosPhi * cosTheta * sinB);
            //luminance

            if (height > y && y > 0 && x > 0 && width > x && depth > zBuffer[circle]) {
                zBuffer[circle] = depth;
                output[circle] = lum_chars[lum > 0 ? lum : 0];
            }
        }
    }
}
int main() {
    float	x = 0, y = 0;
    int buffer_size = 1760;
    float zBuffer[buffer_size];
    char output[buffer_size];
    int height = 22;
    int width = 80;

    printf("\x1b[2J");
    for (;;) {
        render(x, y, buffer_size, zBuffer, output, height, width);
        display(output, x, y);
    }
    return 0;
}
