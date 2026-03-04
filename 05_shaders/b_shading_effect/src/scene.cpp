#include "scene.hpp"


using namespace cgp;


// Main initialization function called once at program startup
// Sets up the camera, 3D scene elements, and the image animation system
void scene_structure::initialize()
{
	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(inputs, window); 
	camera_control.set_rotation_axis_z(); // camera rotates around z-axis
	//   look_at(camera_position, targeted_point, up_direction)
	camera_control.look_at(
		{3.0f,-2.0f, 1.5f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,1,0} /* direction of the "up" vector */);



	// General information
	display_info();

	// Create 3D coordinate frame (x, y, z axes) for visual reference
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the shapes seen in the 3D scene
	// ********************************************** //
	
	// Create a mesh structure (here a cube)
	mesh cube_mesh = mesh_primitive_cube(/*center*/{ 0,0,0 }, /*edge length*/ 1.0f);
	// a mesh is simply a container of vertex data (position,normal,color,uv) and triangle index
	// the mesh data are stored on the CPU memory - they will need to be sent to the GPU memory before being drawn

	// Initialize a mesh drawable from a mesh structure
	//   - mesh : store buffer of data (vertices, indices, etc) on the CPU. The mesh structure is convenient to manipulate in the C++ code but cannot be displayed (data is not on GPU).
	//   - mesh_drawable : store VBO associated to elements on the GPU + associated uniform parameters. A mesh_drawable can be displayed using the function draw(mesh_drawable, environment). It only stores the indices of the buffers on the GPU - the buffer of data cannot be directly accessed in the C++ code through a mesh_drawable.
	//   Note: a mesh_drawable can be created from a mesh structure in calling [mesh_drawable_name].initialize_data_on_gpu([mesh_name])
	cube.initialize_data_on_gpu(cube_mesh);
	cube.material.color = { 1,1,0 };  // set the color of the cube (R,G,B) - sent as uniform parameter to the shader when display is called.
	cube.model.translation = { 1,1,0 }; // set the position of the cube - translation applied as uniform parameter to the "model matrix" in the shader when display is called.

	// Same process for the ground which is a plane 
	//  A quadrangle is defined a plane with 4-extremal corners.
	float L_ground = 20.0f;
	float z_ground = -0.51f;
	mesh ground_mesh = mesh_primitive_quadrangle(
		{ -L_ground, -L_ground, z_ground },
		{  L_ground, -L_ground, z_ground },
		{  L_ground,  L_ground, z_ground },
		{ -L_ground,  L_ground, z_ground });
	ground.initialize_data_on_gpu(ground_mesh);
	//ground.texture.load_and_initialize_texture_2d_on_gpu(project::path+"assets/checkerboard.png");



	mesh sphere_mesh = mesh_primitive_sphere(1.0f);
	sphere.initialize_data_on_gpu(sphere_mesh);
	sphere.model.scaling = 0.2f; // coordinates are multiplied by 0.2 in the shader
	sphere.model.translation = { 1,2,0 }; // coordinates are offset by {1,2,0} in the shader
	sphere.material.color = { 1,0.5f,0.5f }; // sphere will appear red (r,g,b components in [0,1])

	
	// mesh_load_file_obj: lit un fichier .obj et renvoie une structure mesh lui correspondant
	mesh camel_mesh = mesh_load_file_obj(project::path + "assets/camel.obj");

	// Initialisation classique de la structure mesh_drawable
	camel.initialize_data_on_gpu(camel_mesh);
	camel.model.scaling = 0.5f;
	camel.model.translation = { -1,1,0.5f };

	// Sphere used to display the position of a light
	sphere_light.initialize_data_on_gpu(mesh_primitive_sphere(0.2f));

	environment.light = { 3, 3, 3 };

	// Remove warnings for unset uniforms
	cgp_warning::max_warning = 1;

	// Load a shader from a file
	opengl_shader_structure shader_custom;
	shader_custom.load(
		project::path + "shaders/mesh_custom/mesh_custom.vert.glsl", 
		project::path + "shaders/mesh_custom/mesh_custom.frag.glsl");

	// Affect the loaded shader to the mesh_drawable
	camel.shader = shader_custom;
	cube.shader = shader_custom;
	sphere.shader = shader_custom;
	ground.shader = shader_custom;


	std::cout << "End function scene_structure::initialize()" << std::endl;

}




void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
    camera_projection.aspect_ratio = window.aspect_ratio();
	environment.camera_projection = camera_projection.matrix();
	environment.camera_view = camera_control.camera_model.matrix_view();

	

	// Set additional uniform parameters to the shader
	environment.uniform_generic.uniform_float["coeff_ambient"] = gui.ambient;
	environment.uniform_generic.uniform_float["coeff_diffuse"] = gui.diffuse;
	environment.uniform_generic.uniform_float["coeff_specular"] = gui.specular;
	environment.uniform_generic.uniform_float["coeff_specular_exp"] = gui.specular_exp;

	environment.uniform_generic.uniform_vec3["light_color"] = gui.light_color;

	sphere_light.model.translation = environment.light;
	sphere_light.material.color = gui.light_color * 0.8f;
	sphere_light.material.phong.ambient = 1;
	sphere_light.material.phong.diffuse = 0;
	sphere_light.material.phong.specular = 0;
	draw(sphere_light, environment);
	if (gui.display_frame)
		draw(global_frame, environment);

	// the general syntax to display a mesh is:
	//   draw(mesh_drawableName, environment);
	// Note: scene is used to set the uniform parameters associated to the camera, light, etc. to the shader
	draw(ground, environment);
	draw(cube, environment);	
	draw(sphere, environment);
	draw(camel, environment);

	if (gui.display_wireframe) {
		draw_wireframe(ground, environment);
		draw_wireframe(sphere, environment);
		draw_wireframe(cube, environment);
		draw_wireframe(camel, environment);
	}

	


}



void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	ImGui::ColorEdit3("Light color", &gui.light_color[0]);
	ImGui::SliderFloat3("Light position", &environment.light[0], -3.0f, 3.0f);

	ImGui::SliderFloat("Ambient", &gui.ambient, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse", &gui.diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular", &gui.specular, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular Exp.", &gui.specular_exp, 1.0f, 256.0f, "%.3f", 2);
}




void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift) 
		camera_control.action_mouse_move();
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click();
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard();
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame();
	
}


void scene_structure::display_info()
{
	std::cout << "\nCAMERA CONTROL:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << camera_control.doc_usage() << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;


	std::cout << "\nSCENE INFO:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Custom shading effects: ambient, diffuse, specular lighting in GLSL." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}