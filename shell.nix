with import <nixpkgs> { };

stdenv.mkDerivation {
  name = "c";
  nativeBuildInputs = [
    pkg-config
    gdb
  ];
  buildInputs = [
    glfw
    glew
    libGL
    libGLU
    soil
    freetype
    cmocka
  ];
  shellHook = "";
}
