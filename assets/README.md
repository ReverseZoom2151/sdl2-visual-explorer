# Captured application media

`maze-demo.gif` and `fractal-demo.gif` are committed README assets captured
from the compiled SDL programs running inside an Xvfb display. They are not
illustrations or manually redrawn substitutes.

Regenerate both files with:

```bash
./scripts/capture_demos.sh
```

The script requires SDL2 development files, CMake, Ninja, FFmpeg, Xvfb, and
the X11 development library used by its small keyboard-event helper.
