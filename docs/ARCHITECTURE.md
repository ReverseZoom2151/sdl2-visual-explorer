# Architecture

Emerald Maze has three deliberately separated layers.

- `include/emerald/` contains the public C interfaces shared by every target.
- `src/core/` owns the maze model: directions, grid storage, entities, state,
  and player actions. It has no SDL dependency and is covered by CTest and
  Valgrind.
- `src/platform/` adapts that model to SDL2 through the small `display` API.

`app/` contains the maintained runnable programs: the maze and Mandelbrot
explorer. `examples/` contains small SDL experiments; they are built for
reference but are not part of the core API. `tests/` exercises core behavior
without opening a window.

The CMake targets mirror the boundaries: `emerald_core`, `emerald_display`,
`emerald-maze`, `emerald-fractal`, and `emerald-core-tests`.
