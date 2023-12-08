#include "cube.h"

// Note: When I downloaded PA2 for this assignment, the cube was already implemented in the Object class. I of course seperated it out but was that suppose to already be implemented?
// Sets up the verticies for a cube. Buffers are setup in base Object class

Cube::Cube()
{
	Cube::createVertices();
}

void Cube::createVertices() {
    // Vertices
    Vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f))); // 0
    Vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f))); // 1
    Vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f))); // 2
    Vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f))); // 3
    Vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f))); // 4
    Vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f,  1.0f),glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f))); // 5
    Vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f,  1.0f),glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f))); // 6
    Vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f))); // 7




	Indices = { // Format is Face Orientation and then upper/lower which refer to where the base of the traingle sits (in reference to the top and bottom face)
			  1, 2, 3, // Bottom Face, Front 
			  7, 6, 5, // Top Face, Front
			  0, 4, 5, // East Face, Upper
			  1, 5, 6, // Front Face, Upper 
			  6, 7, 3, // West Face, Upper
			  0, 3, 7, // North Face, Lower 
			  0, 1, 3, // Bottom Face, Back 
			  4, 7, 5, // Top Face, Back 
			  1, 0, 5, // East Face, Lower
			  2, 1, 6, // South Face, Lower
			  2, 6, 3, // West Face, Lower
			  4, 0, 7  // North Face, Upper
	};

}

void Cube::setupBuffers(GLint posAttribLoc, GLint colAttribLoc) {
	// Set up your VOA
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// setting the Vertex VBO
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Setting the Index VBO
	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

void Cube::InitializeCube(GLint posAttribLoc, GLint colAttribLoc) {

	setupBuffers(posAttribLoc, colAttribLoc);

	// Computing the model matrix
	angle = 1 * 3.1415f; 	// Model orientation

	//float tvec1 = glm::linearRand(-1.f, 1.f);
	//float tvec2 = glm::linearRand(-1.f, 1.f);
	//float tvec3 = glm::linearRand(-2.f, 2.f);

	//model = glm::translate(glm::mat4(1.0f), glm::vec3(tvec1, tvec2, tvec3));
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Remember, y is up in this world
	model *= glm::rotate(glm::mat4(1.0f), 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model *= glm::scale(glm::vec3(1, 1, 1));
}

Cube::~Cube()
{
	Vertices.clear();
	Indices.clear();
}