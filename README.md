# Emerald Maze

Emerald Maze is a compact C23 and SDL2 project built around two small visual programs:

- **Emerald Maze** — collect every emerald in a fixed grid while navigating walls with the arrow keys.
- **Mandelbrot Flight** — explore a continuously-rendered Mandelbrot view with keyboard zoom and pan controls.

The repository is intentionally more than a pair of demos. Its maze model is a separate, tested C library with validated ASCII levels, explicit ownership, and no SDL dependency. SDL2 is kept at the presentation boundary.

## Highlights

- C23 project built with CMake and Ninja.
- A testable maze core: directions, entities, grid storage, game state, player actions, and level validation.
- SDL2 display adapter with checked startup and software-renderer fallback.
- CTest, Valgrind, AddressSanitizer, UndefinedBehaviorSanitizer, clang-format, and GitHub Actions checks.
- A clean source layout rather than a flat collection of coursework files.

## Quick start

On Ubuntu or Debian, install the prerequisites:

```bash
sudo apt-get install -y cmake ninja-build libsdl2-dev
```

Build and test:

```bash
cmake --preset dev
cmake --build --preset dev
ctest --test-dir build/dev --output-on-failure
```

Run the applications:

```bash
./build/dev/emerald-maze
./build/dev/emerald-fractal
```

The Makefile is a small convenience wrapper for the same workflow:

```bash
make build
make test
make sanitize
```

## Controls

| Program | Controls |
| --- | --- |
| Emerald Maze | Arrow keys move. Collect every emerald. `Esc` quits. |
| Mandelbrot Flight | `Z` zooms in, `X` zooms out, arrow keys pan, and `Esc` quits. |

## Project layout

```text
app/                 maintained SDL applications
examples/            small SDL reference programs
include/emerald/     public library headers
src/core/            tested, SDL-independent maze model
src/platform/        SDL2 display adapter
tests/               CTest regression coverage
docs/                architecture and implementation notes
```

The dependency direction is deliberate:

```text
app ──> platform (SDL2)
  └──> core (no graphics dependency)
```

See [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) for the component-level breakdown.

## Quality checks

The default development preset builds debug targets and runs the core tests. The sanitizer preset adds AddressSanitizer and UndefinedBehaviorSanitizer:

```bash
cmake --preset sanitize
cmake --build --preset sanitize
ctest --test-dir build/sanitize --output-on-failure
```

For a memory check of the non-graphical core:

```bash
valgrind --error-exitcode=1 --leak-check=full build/dev/emerald-core-tests
```

GitHub Actions runs formatting, CTest, Valgrind, and sanitizer checks on every push and pull request.

## Status

Emerald Maze is a focused learning and systems-programming project. The code is structured to make further work straightforward: additional validated levels, game states, rendering themes, and fractal palettes can be added without entangling the core model with SDL.
