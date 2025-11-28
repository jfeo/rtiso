# rtiso

A real-time isometric game engine, with a focus on pixel-perfect sprite-based
graphics.

The main idea is to project sprites onto shapes as textures, in order for the
rendered objects to mesh together correctly.

The project relies on `OpenGL`, and `GLEW`, for rendering graphics, `GLFW` for
creating the window, and interaction, and `SOIL` for loading images.

Furthermore it relies on `CMocka` for unit testing.

## Running

Simply use `make`:

```
make run
```

## On Nix

Start a nix-shell with the provided shell. This will install the required
development libraries, and headers.

```
nix-shell shell.nix
```

## Running tests

To run the unit tests:

```
make test
```
