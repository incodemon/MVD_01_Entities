#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"
#include "Texture.h"
#include <vector>
#include <unordered_map>

class GraphicsSystem {
public:
	//destructor frees Shader objects
	~GraphicsSystem();

	//init and update are mandatory for all systems
	void init();
	void update(float dt);

	//load and get shader into/from private unordered_map
	int loadShader(std::string name, std::string vs_path, std::string fs_path);
	int getShaderProgram(std::string name);

	//loads a texture and returns the OpenGL texture id
	GLuint loadTexture(std::string path);

	//create a simple plane geometry
	void createPlaneGeometry(GLuint& vao, GLuint& num_tris);

private:
	//dictionary of shaders
	std::unordered_map<std::string, Shader*> shaders_;
	lm::mat4 view_projection_;
	lm::vec3 cam_position_;
	//render a single mesh component
	void renderMeshComponent_(Mesh& comp);
};
