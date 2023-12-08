#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include "object.h"
#include "graphics_headers.h"

class Cube : public Object // Extends Object class
{
public:
    Cube(); // Calls createVertices
    ~Cube();
    void InitializeCube(GLint posAttribLoc, GLint colAttribLoc); // Generates model matrix
    void createVertices(); // Sets up the verticies and index array for a cube (8 verticies and 12 triangles)
    void setupBuffers(GLint posAttribLoc, GLint colAttribLoc);

private:

};

#endif /* CUBE_H */