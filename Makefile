BUILD_DIR ?= build/dev

.PHONY: all configure build test sanitize format format-check clean

all: build

configure:
	cmake --preset dev

build: configure
	cmake --build --preset dev

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

sanitize:
	cmake --preset sanitize
	cmake --build --preset sanitize
	ctest --test-dir build/sanitize --output-on-failure

format:
	cmake --build $(BUILD_DIR) --target format

format-check: configure
	cmake --build $(BUILD_DIR) --target format-check

clean:
	cmake -E rm -rf build
