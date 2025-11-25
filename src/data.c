#include "data.h"

/* 0.0f, 2 * ENTITY_HEIGHT, 0.0f, 0.5f, 2.0f / 6.0f, */
/* // East */
/* 0.0f, 2 * ENTITY_HEIGHT, 2.0f, 1.0f, 1.0f / 6.0f, */
/* // West */
/* 2.0f, 2 * ENTITY_HEIGHT, 0.0f, 0.0f, 1.0f / 6.0f, */
/* // North */
/* 2.0f, 2 * ENTITY_HEIGHT, 2.0f, 0.5f, 0.0f, */

/* // Bottom */
/* // South */
/* 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, */
/* // East */
/* 0.0f, 0.0f, 2.0f, 1.0f, 5.0f / 6.0f, */
/* // West */
/* 2.0f, 0.0f, 0.0f, 0.0f, 5.0f / 6.0f, */

GLfloat entity_vertices[] = {
    // Top
    // South
    0.0f, 2 * ENTITY_HEIGHT, 0.0f, 0.5f, 2.0f / 6.0f,
    // East
    0.0f, 2 * ENTITY_HEIGHT, 2.0f, 1.0f, 1.0f / 6.0f,
    // West
    2.0f, 2 * ENTITY_HEIGHT, 0.0f, 0.0f, 1.0f / 6.0f,
    // North
    2.0f, 2 * ENTITY_HEIGHT, 2.0f, 0.5f, 0.0f,

    // Bottom
    // South
    0.0f, 0.0f, 0.0f, 0.5f, 1.0f,
    // East
    0.0f, 0.0f, 2.0f, 1.0f, 5.0f / 6.0f,
    // West
    2.0f, 0.0f, 0.0f, 0.0f, 5.0f / 6.0f};

GLuint entity_indices[] = {
    // top face
    0, 2, 3, 3, 1, 0,

    // right face
    6, 2, 0, 0, 4, 6,

    // left face
    5, 1, 0, 0, 4, 5};

GLfloat tile_vertices[] = {
    // South
    0.0f, 0.0f, 0.0f, 0.5f / 4.0f, 1.0f / 4.0f,
    // East
    0.0f, 0.0f, 1.0f, 1.0f / 4.0f, 0.5f / 4.0f,
    // West
    1.0f, 0.0f, 0.0f, 0.0f, 0.5f / 4.0f,
    // North
    1.0f, 0.0f, 1.0f, 0.5f / 4.0f, 0.0f};

GLuint tile_indices[] = {0, 2, 3, 0, 3, 1};
