#include "emerald/fractal.h"

#include <float.h>
#include <math.h>
#include <stddef.h>

fractalView defaultFractalView(void) { return (fractalView){-0.75, 0.0, 1.5}; }

bool zoomFractal(fractalView *view, double factor) {
    if (view == NULL || !isfinite(factor) || factor <= 0.0)
        return false;
    double nextScale = view->scale * factor;
    if (!isfinite(nextScale) || nextScale < DBL_EPSILON || nextScale > DBL_MAX / 4.0)
        return false;
    view->scale = nextScale;
    return true;
}

bool panFractal(fractalView *view, double horizontal, double vertical) {
    if (view == NULL || !isfinite(horizontal) || !isfinite(vertical))
        return false;
    double nextX = view->centerX + horizontal * view->scale;
    double nextY = view->centerY + vertical * view->scale;
    if (!isfinite(nextX) || !isfinite(nextY))
        return false;
    view->centerX = nextX;
    view->centerY = nextY;
    return true;
}

int mandelbrotIterations(const fractalView *view, int pixelX, int pixelY, int width, int height,
                         int maximumIterations) {
    if (view == NULL || width <= 0 || height <= 0 || maximumIterations <= 0 || pixelX < 0 ||
        pixelX >= width || pixelY < 0 || pixelY >= height)
        return 0;

    double aspect = (double)width / (double)height;
    double real = view->centerX + ((2.0 * pixelX / width) - 1.0) * view->scale * aspect;
    double imaginary = view->centerY + ((2.0 * pixelY / height) - 1.0) * view->scale;
    double zx = 0.0;
    double zy = 0.0;
    int iteration = 0;
    while (iteration < maximumIterations && zx * zx + zy * zy <= 4.0) {
        double nextX = zx * zx - zy * zy + real;
        zy = 2.0 * zx * zy + imaginary;
        zx = nextX;
        ++iteration;
    }
    return iteration;
}
