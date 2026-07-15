#pragma once

#include <stdbool.h>

typedef struct fractalView {
    double centerX;
    double centerY;
    double scale;
} fractalView;

fractalView defaultFractalView(void);
bool zoomFractal(fractalView *view, double factor);
bool panFractal(fractalView *view, double horizontal, double vertical);
int mandelbrotIterations(const fractalView *view, int pixelX, int pixelY, int width, int height,
                         int maximumIterations);
