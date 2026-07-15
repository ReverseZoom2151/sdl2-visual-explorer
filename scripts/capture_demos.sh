#!/usr/bin/env bash
set -euo pipefail

cmake --preset dev
cmake --build --preset dev
mkdir -p assets
helper="$(mktemp)"
cc -std=c17 -Wall -Wextra -Werror scripts/send_x_key.c -lX11 -o "$helper"
trap 'rm -f "$helper" /tmp/maze-demo.mp4 /tmp/fractal-demo.mp4 /tmp/maze-palette.png /tmp/fractal-palette.png' EXIT

xvfb-run -a -s '-screen 0 1024x768x24' bash -c '
  ./build/dev/maze-explorer & game=$!
  sleep 0.6
  ffmpeg -y -loglevel error -f x11grab -draw_mouse 0 -video_size 1024x768 -i "$DISPLAY" -t 4 -vf "crop=576:576:224:96,fps=10" /tmp/maze-demo.mp4 & recorder=$!
  for key in Up Right Right Down Up Right Up Right Right Right; do
    sleep 0.25
    "$0" "Emerald Maze" "$key"
  done
  wait "$recorder"
  kill "$game"
  wait "$game" 2>/dev/null || true
' "$helper"
ffmpeg -y -loglevel error -i /tmp/maze-demo.mp4 -vf 'fps=10,scale=576:-2:flags=lanczos,palettegen' /tmp/maze-palette.png
ffmpeg -y -loglevel error -i /tmp/maze-demo.mp4 -i /tmp/maze-palette.png -lavfi 'fps=10,scale=576:-2:flags=lanczos[x];[x][1:v]paletteuse' assets/maze-demo.gif

xvfb-run -a -s '-screen 0 1024x768x24' bash -c '
  ./build/dev/fractal-explorer & explorer=$!
  sleep 0.8
  ffmpeg -y -loglevel error -f x11grab -draw_mouse 0 -video_size 1024x768 -i "$DISPLAY" -t 5 -vf "crop=640:640:192:64,fps=10" /tmp/fractal-demo.mp4 & recorder=$!
  for key in z z Right Up z Left; do
    sleep 0.5
    "$0" "Emerald Fractal Explorer" "$key"
  done
  wait "$recorder"
  kill "$explorer"
  wait "$explorer" 2>/dev/null || true
' "$helper"
ffmpeg -y -loglevel error -i /tmp/fractal-demo.mp4 -vf 'fps=10,scale=640:-2:flags=lanczos,palettegen=max_colors=128' /tmp/fractal-palette.png
ffmpeg -y -loglevel error -i /tmp/fractal-demo.mp4 -i /tmp/fractal-palette.png -lavfi 'fps=10,scale=640:-2:flags=lanczos[x];[x][1:v]paletteuse=dither=sierra2_4a' assets/fractal-demo.gif
