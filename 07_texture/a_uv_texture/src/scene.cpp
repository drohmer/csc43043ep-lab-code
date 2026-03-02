#include "scene.hpp"

#include "models_textures.hpp"

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
		{ 3.0f, 2.0f, 2.0f  } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,0,1} /* direction of the "up" vector */);



	// General information
	display_info();

	// Create 3D coordinate frame (x, y, z axes) for visual reference
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the shapes seen in the 3D scene
	// ********************************************** //
	
// Create a quadrangle as a mesh
	mesh quadrangle_mesh;
	quadrangle_mesh.position = { {-1,-1,0}, { 1,-1,0}, { 1, 1,0}, {-1, 1,0} };
	quadrangle_mesh.uv = { {0,0}, {1,0}, {1,1}, {0,1} }; // Associate Texture-Coordinates to the vertices of the quadrangle
	quadrangle_mesh.connectivity = { {0,1,2}, {0,2,3} };

	quadrangle_mesh.fill_empty_field(); // (fill with some default values the other buffers (colors, normals) that we didn't filled before)
	// Convert the mesh structure into a mesh_drawable structure
	shape.initialize_data_on_gpu(quadrangle_mesh);


	// Texture Image load and association
	//-----------------------------------	

	// Load an image from a file, and send the result to the GPU
	shape.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/squirrel.jpg",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
std::cout << "End function scene_structure::initialize()" << std::endl;

}




void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
    camera_projection.aspect_ratio = window.aspect_ratio();
	environment.camera_projection = camera_projection.matrix();
	environment.camera_view = camera_control.camera_model.matrix_view();
	environment.light = camera_control.camera_model.position();

	// conditional display of the global frame (set via the GUI)
	if (gui.display_frame)
		draw(global_frame, environment);



	draw(shape, environment);
	if (gui.display_wireframe)
		draw_wireframe(shape, environment, { 1,0,0 });
}




void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
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
	std::cout << "Texture UV." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}