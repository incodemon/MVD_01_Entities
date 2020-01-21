#include "GraphicsSystem.h"
#include "Game.h"
#include "extern.h"


//destructor
GraphicsSystem::~GraphicsSystem() {
	//delete shader pointers
	for (auto shader_pair : shaders_) {
		if (shader_pair.second)
			delete shader_pair.second;
	}
}


void GraphicsSystem::init() {
	//TODO: 
	// - initialise state of openGL
	// - create camera variable in .h and initialise camera here
	cam_position_ = lm::vec3(0.0f, 0.0f, 3.0f);
	lm::vec3 cam_target(0.0f, 0.0f, 0.0f);
	lm::vec3 cam_up(0.0f, 1.0f, 0.0f);
	lm::mat4 view_matrix, projection_matrix;
	view_matrix.lookAt(cam_position_, cam_target, cam_up);
	projection_matrix.perspective(60.0f * DEG2RAD, 1, 0.01f, 100.0f);
	view_projection_ = projection_matrix * view_matrix;

}

void GraphicsSystem::update(float dt) {
	
	//TODO:
	// - clear buffers
	// - set shader program
	// render all mesh components in ECS
	// basic - keep rendering code here
	// advanced: abstract code to renderMeshComponent function below
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int temp_program = shaders_["phong"]->program;
	glUseProgram(temp_program);

	//glUseProgram(shaders_["phong"]->program);

	auto& all_meshes = ECS.getAllComponents<Mesh>();
	
	//this code to move to GraphicsSystem.init() and be called ONCE
	
	
	

	

	//to here
	for(auto& mesh : all_meshes){
		auto& all_trasnforms = ECS.getAllComponents<Transform>();
		//model matrix
		lm::mat4 model_matrix =
			ECS.getComponentFromEntity<Transform>(mesh.owner).getGlobalMatrix(all_trasnforms);
		


		//normal matrix
		lm::mat4 normal_matrix = model_matrix;
		normal_matrix.inverse();
		normal_matrix.transpose();

		//Model view projection matrix
		lm::mat4 mvp_matrix = view_projection_ * model_matrix;

		//ask shader for a reference to the uniforms 
		GLint u_mvp = glGetUniformLocation(temp_program, "u_mvp");
		GLint u_model = glGetUniformLocation(temp_program, "u_model");
		GLint u_normal_matrix = glGetUniformLocation(temp_program, "u_normal_matrix");
		GLint u_light_pos = glGetUniformLocation(temp_program, "u_light_pos");
		GLint u_cam_pos = glGetUniformLocation(temp_program, "u_cam_pos");
		GLint u_texture_diffuse = glGetUniformLocation(temp_program, "u_texture_diffuse");
		GLint u_glossiness = glGetUniformLocation(temp_program, "u_glossiness");

		//if the uniforms exist, send the data to the shader
		if (u_mvp != -1) glUniformMatrix4fv(u_mvp, 1, GL_FALSE, mvp_matrix.m);
		if (u_model != -1) glUniformMatrix4fv(u_model, 1, GL_FALSE, model_matrix.m);
		if (u_normal_matrix != -1) glUniformMatrix4fv(u_normal_matrix, 1, GL_FALSE, normal_matrix.m);
		if (u_light_pos != -1) glUniform3f(u_light_pos, 1000.0f, 0.0f, 1000.0f); //... 3f - is 3 floats
		if (u_cam_pos != -1) glUniform3fv(u_cam_pos, 1, cam_position_.value_); // ...3fv - is array of 3 floats
		if (u_texture_diffuse != -1) glUniform1i(u_texture_diffuse, 0); // ...1i - is integer
		if (u_glossiness != -1) glUniform1f(u_glossiness, 80.0f); //...1f - for float

		//activate texture unit 0, and bind our texture there
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, temp_texture);

		//tell OpenGL we want to the the vao_ container with our buffers
		glBindVertexArray(mesh.vao);

		//draw our geometry
		glDrawElements(GL_TRIANGLES, mesh.num_tris * 3, GL_UNSIGNED_INT, 0);

		//tell OpenGL we don't want to use our container anymore
		glBindVertexArray(0);



	}

	glUseProgram(0);
}

//renders a given mesh component
void GraphicsSystem::renderMeshComponent_(Mesh& comp) {
	//TODO:
	// - drawing code for a single mesh component

	// hint to get model matrix from a transform:
	// Transform& transform = ECS.getComponentFromEntity<Transform>(comp.owner);
	// to get world matrix using scene hierarchy: 
	// transform.getGlobalMatrix(ECS.getAllComponents<Transform>());


}

//********************************************
// OpenGL helper functions
//********************************************

//loads a shader and returns its program id
int GraphicsSystem::loadShader(std::string name, std::string vs_path, std::string fs_path) {
	shaders_[name] = new Shader(vs_path, fs_path);
	return shaders_[name]->program;
}

//returns a shader program id
int GraphicsSystem::getShaderProgram(std::string name) {
	return shaders_[name]->program;
}

//loads a texture and returns its id
GLuint GraphicsSystem::loadTexture(std::string path) {
	Texture t("data/assets/test.tga");
	return t.texture_id;
}

//creates a standard plane geometry
void GraphicsSystem::createPlaneGeometry(GLuint& vao, GLuint& num_tris) {

	//four vertices in a square
	const GLfloat position_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	//texture coordinates (uvs)
	const GLfloat texture_buffer_data[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	//normals point 'out of screen'
	const GLfloat normal_buffer_data[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};
	//index buffer
	const GLuint index_buffer_data[] = { 0, 1, 2, 0, 2, 3 };

	//set number of triangles (of passed variable)
	num_tris = 2;

	//create Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Now create Vertex Buffer Objects for each buffer: positions, uvs, normals, and indices
	GLuint vbo;
	//positions
	glGenBuffers(1, &vbo); //create buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //set OpenGL state to say we want to work with this buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(position_buffer_data), position_buffer_data, GL_STATIC_DRAW); //copy data
	glEnableVertexAttribArray(0); //enable attribute labelled as '0' in vertex shader: "layout(location = 0) in vec3 a_vertex;"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //each attribute '0' is composed of 3 floats (in this case, xyz)

	//texture coords
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_buffer_data), texture_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // (1 = a_uv)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); //only two coordinates for textures: uv
	//normals								   
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer_data), normal_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // (2 = a_normal)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//indices
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);
	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}
