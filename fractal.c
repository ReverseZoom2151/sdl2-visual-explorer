#include "display.h"
#include <float.h>
#include <time.h>
#include <assert.h>

#define ZOOM 1.03
#define ACC 64
#define STEPS 2

typedef struct params {
    double s, x, y;
} params;

void *newFractalState() {
    srand(time(0));
    params *p = malloc(sizeof(params));
    *p = (params){2.0, -3.0E-3 * ((rand()) % 1000 - 5.0E2) - 1.5, -1.0};
    return (void *)p;
}

void freeFractalState(void *data) {
    if (data)
        free(data);
}

int calculateMandelbrot(const int accuracy, const int x, const int y, const int w, const int h,
                        const params *p) {
    int i;
    double a[4] = {0.0};
    for (i = 0; (i < accuracy) && ((a[2] + a[3]) < accuracy / 4); i++) {
        a[0] = 2.0 * a[0] * a[1] + y * p->s / h + p->y;
        a[1] = a[2] - a[3] + x * p->s / w + p->x;
        a[2] = a[1] * a[1];
        a[3] = a[0] * a[0];
    }
    return i;
}

bool navigateMandelbrot(display *d, void *data, const char pressedKey) {

    params *p = (params *)data;

    if ((d == NULL) || (p == NULL))
        return true;
    int w = getWidth(d), h = getHeight(d), tx = w / 4, ty = h / 4;
    params c = (params){DBL_MAX, 0.0, 0.0};

    for (int y = 0; y < h; y += STEPS) {
        bool last = false;
        for (int x = 0; x < w; x += STEPS) {
            int i = calculateMandelbrot(ACC, x, y, w, h, p);
            if (i == ACC) {
                if (!last) {
                    int target = (x - tx) * (x - tx) + (y - ty) * (y - ty);
                    if (target < c.s)
                        c = (params){target, x - tx, y - ty};
                }
                colour(d, 0xFF);
                last = true;
            } else {
                colour(d, 0xFFFF * (1 + 16 * (i % 16)));
                last = false;
            }
            block(d, x, y, STEPS, STEPS);
        }
    }
    show(d);
    if (p->s < 1.0E-6)
        p->s = 1.0E-3;
    *p = (params){(pressedKey == 120)
                      ? (p->s / (ZOOM / 1.15))
                      : ((pressedKey == 122) ? (p->s / (ZOOM * 1.15)) : (p->s / ZOOM)),
                  ((pressedKey == 79) || (pressedKey == 80))
                      ? (p->x + p->s * (w / 4 - ((pressedKey - 78) * 2 - 3) * (w / 20) - tx) / w)
                      : (p->x + p->s * c.x / w),
                  ((pressedKey == 81) || (pressedKey == 82))
                      ? (p->y + p->s * (h / 4 - ((pressedKey - 80) * 2 - 3) * (h / 20) - ty) / h)
                      : (p->y + p->s * c.y / h)};
    return (pressedKey == 27);
}

int main() {
    display *d = newDisplay("Mandelbrot Flight", 256, 256);
    void *data = newFractalState();
    run(d, data, navigateMandelbrot);
    freeFractalState(data);
    freeDisplay(d);
    return 0;
}
