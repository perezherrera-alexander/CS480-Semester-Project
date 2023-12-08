#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif



	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");

	// The Sun
	m_sphere = new Sphere(64, "assets\\2k_sun.jpg");

	// The Earth
	m_sphere2 = new Sphere(48, "assets\\2k_earth_daymap.jpg");
	
	// The moon
	m_sphere3 = new Sphere(48, "assets\\2k_moon.jpg");

	m_cube = new Cube();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {
	// Update your animation for the solar system here.
	glm::mat4 tmat, rmat, smat;
	std::vector<float> speed, dist, rotSpeed, scale;
	glm::vec3 rotVector;

	modelStack.push(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0))); // World Origin
	

	// Sun
	modelStack.push(modelStack.top()); // Sun origin
	speed = { 0., 0., 0. };
	dist = { 0., 0., 0. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 0.2, 0., 0. };
	scale = { 1.,1.,1. };
	ComputeTransforms(dt, speed, dist, rotSpeed, rotVector, scale, tmat, rmat, smat);
	if (m_sphere != NULL)
		m_sphere->Update(tmat * rmat * smat);
	modelStack.top()* tmat;

	// Starship
	modelStack.push(modelStack.top()); // Position based off the sun
	speed = { 0.5, 0.5, 0.5 };
	dist = { 0.0, 3., 3. };
	rotVector = { 1.0, 0.0 ,0.0 };
	rotSpeed = { 0.2, 0.0, 0.0 };
	scale = { .01,.01,.01 };
	ComputeTransforms(dt, speed, dist, rotSpeed, rotVector, scale, tmat, rmat, smat);
	glm::vec3 upVec = glm::vec3(0, 1, 0);
	glm::vec3 targetPoint = glm::vec3(0, 0, 0);
	glm::vec3 position = glm::vec3(tmat[3][0], tmat[3][1], tmat[3][2]); // Manually get position out of the tmat
	glm::vec3 z_axis = glm::normalize(targetPoint - position);
	glm::vec3 x_axis = glm::normalize(glm::cross(upVec, z_axis));
	glm::vec3 y_axis = glm::normalize(glm::cross(z_axis, x_axis));
	rmat = glm::mat4(glm::vec4(x_axis, 0), glm::vec4(y_axis, 0), glm::vec4(z_axis, 0), glm::vec4(0, 0, 0, 1));
	if (m_mesh != NULL)
		m_mesh->Update(modelStack.top() * tmat * rmat * smat);
	modelStack.top() *= tmat;
	modelStack.pop(); // back to the sun

	
	// Earth
	modelStack.push(modelStack.top()); // Position based off the sun
	speed = { 0.3, 0.0, 0.3 };
	dist = { 6., 0, 6. };
	rotVector = { 0.0,1.0,0.0 };
	rotSpeed = { 0.4, 0.0, 0.0 };
	scale = { .5,.5,.5 };
	ComputeTransforms(dt, speed, dist, rotSpeed, rotVector, scale, tmat, rmat, smat);
	if (m_sphere2 != NULL)
		m_sphere2->Update(modelStack.top() * tmat * rmat * smat);
	modelStack.top() *= tmat;
	//modelStack.pop();
	// Don't pop, moon will need earth position


	// Moon
	modelStack.push(modelStack.top()); // Position based off the earth
	speed = { 1, 1, 1 };
	dist = { 1.25, 0.3, 1.25 };
	rotVector = { 0.0 ,1.0, 0.0};
	rotSpeed = { 0.25, 0, 0 };
	scale = { .27f, .27f, .27f };
	ComputeTransforms(dt, speed, dist, rotSpeed, rotVector, scale, tmat, rmat, smat);
	if (m_sphere3 != NULL)
		m_sphere3->Update(modelStack.top() * tmat * rmat * smat);
	modelStack.top() *= tmat;
	modelStack.pop(); // back to the earth

	modelStack.pop(); // back to the sun

	modelStack.pop(); 	// back to the world origin
	
}


void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist, 
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], cos(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	// Render the objects
	/*
	if (m_cube != NULL){
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
		m_cube->Render(m_positionAttrib,m_colorAttrib);
	}
	*/

	
	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	/*if (m_pyramid != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_pyramid->GetModel()));
		m_pyramid->Render(m_positionAttrib, m_colorAttrib);
	}*/

	
	if (m_sphere != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere->GetModel()));
		if (m_sphere->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
				
			}
			glUniform1i(sampler, 0);
			m_sphere->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
		
	}
	

	
	if (m_sphere2 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere2->GetModel()));
		if (m_sphere2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	// Render Moon
	if (m_sphere3 != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sphere3->GetModel()));
		if (m_sphere3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sphere3->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sphere3->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

