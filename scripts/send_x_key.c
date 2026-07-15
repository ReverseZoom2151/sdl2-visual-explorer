#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Window findWindow(Display *display, Window window, const char *title) {
    char *name = NULL;
    if (XFetchName(display, window, &name) != 0) {
        int matches = strcmp(name, title) == 0;
        XFree(name);
        if (matches)
            return window;
    }
    Window root = 0;
    Window parent = 0;
    Window *children = NULL;
    unsigned int count = 0;
    if (XQueryTree(display, window, &root, &parent, &children, &count) == 0)
        return 0;
    Window result = 0;
    for (unsigned int index = 0; index < count && result == 0; ++index)
        result = findWindow(display, children[index], title);
    if (children != NULL)
        XFree(children);
    return result;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: send_x_key WINDOW_TITLE KEY\n");
        return EXIT_FAILURE;
    }
    Display *display = XOpenDisplay(NULL);
    if (display == NULL)
        return EXIT_FAILURE;
    Window window = findWindow(display, DefaultRootWindow(display), argv[1]);
    KeySym symbol = XStringToKeysym(argv[2]);
    KeyCode code = XKeysymToKeycode(display, symbol);
    if (window == 0 || symbol == NoSymbol || code == 0) {
        XCloseDisplay(display);
        return EXIT_FAILURE;
    }
    XKeyEvent key = {0};
    key.display = display;
    key.window = window;
    key.root = DefaultRootWindow(display);
    key.same_screen = True;
    key.keycode = code;
    key.type = KeyPress;
    XSendEvent(display, window, True, KeyPressMask, (XEvent *)&key);
    key.type = KeyRelease;
    XSendEvent(display, window, True, KeyReleaseMask, (XEvent *)&key);
    XFlush(display);
    XCloseDisplay(display);
    return EXIT_SUCCESS;
}
