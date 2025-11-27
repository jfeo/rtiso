with import <nixpkgs> { };

stdenv.mkDerivation {
  name = "c";
  nativeBuildInputs = [ pkg-config ];
  buildInputs = [
    glfw
    glew
    libGL
    libGLU
    soil
    freetype
    gdb
  ];
  shellHook = "";
}
