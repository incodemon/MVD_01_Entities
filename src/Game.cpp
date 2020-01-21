#include "Game.h"
#include "extern.h"
#include "Shader.h"
#include "Texture.h"

void Game::init() {

	//******** INITIALISATION CODE **********//
    
	//add shader
	temp_program = graphics_system_.loadShader("phong", "data/shaders/phong.vert", "data/shaders/phong.frag");
    
	//create a temporary texture as we don't have a material system yet
	temp_texture = graphics_system_.loadTexture("data/assets/test.tga");

	//create plane geometry for use with Mesh Component
	graphics_system_.createPlaneGeometry(temp_vao, temp_tris);

	//set 'background' colour of framebuffer
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);



	int plane2_ent = ECS.createEntity("plane2");
	Mesh& plane2_mesh = ECS.createComponentForEntity<Mesh>(plane2_ent);
	plane2_mesh.vao = temp_vao;
	plane2_mesh.num_tris = temp_tris;

	Transform& plane2_transform = ECS.getComponentFromEntity<Transform>(plane2_ent);
	plane2_transform.translate(-1.0, 0, 0);

	int plane_ent = ECS.createEntity("plane");
	Mesh& plane_mesh = ECS.createComponentForEntity<Mesh>(plane_ent);
	plane_mesh.vao = temp_vao;
	plane_mesh.num_tris = temp_tris;

	Transform& plane_transform = ECS.getComponentFromEntity<Transform>(plane_ent);
	plane_transform.translate(1.0, 0, 0);

	
	//******* TODO *******//

	//TODO in Components.h
	// - fill in MeshComponent properties

	//TODO in GraphicsSystem.cpp
	// - move all GL and rendering code from Game 

	//TODO in Game.cpp
	// - call init and update functions of GraphicSystem
	// - create entity for plane - use function defined in ECS
	// - create mesh component for plane - use function in ECS
	// - assign mesh component vao and num tris from created plane geometry
	// - delete temp_camera (move it GraphicsSystem)

	// - change position of plane using transform component:
	//   ECS.getComponentFromEntity<Transform>(entity_id).translate(0.0f, 1.0f, 3.0f);

	// - Advanced: create a second entity and new Mesh Component
	//   which uses the same geometry
	// - draw both entities in different positions
	
}

//Entry point for game update code
void Game::update(float dt) {
	graphics_system_.update(dt);
	//TODO
	// - update graphics system
	// - move this drawing code to graphics system


	//tell OpenGL which shader to use
	glUseProgram(temp_program);

	

}

