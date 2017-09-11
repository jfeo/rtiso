# Libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(SOIL REQUIRED)


include_directories(${FREETYPE_INCLUDE_DIRS})

file(GLOB rtiso_sources ${CMAKE_CURRENT_SOURCE_DIR}
	"*.c"
)

file(GLOB rtiso_headers ${CMAKE_CURRENT_SOURCE_DIR}
	"*.h"
)

add_executable(rtiso ${rtiso_sources})
target_link_libraries(rtiso rt ${OPENGL_gl_LIBRARY} glfw GLEW m ${SOIL_LIBRARIES} ${FREETYPE_LIBRARY})
