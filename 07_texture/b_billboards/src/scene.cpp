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
		{ 3.0f, 2.0f, 2.0f  } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,1,0} /* direction of the "up" vector */);



	// General information
	display_info();

	// Create 3D coordinate frame (x, y, z axes) for visual reference
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the shapes seen in the 3D scene
	// ********************************************** //
	
	floor.initialize_data_on_gpu(mesh_primitive_quadrangle({ -1.5f,-1.5f,0.0f }, { 1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f }, { -1.5f,1.5f,0.0f }));
	floor.material.color = { 0.75f, 0.8f, 0.5f };

	mesh quad_mesh = mesh_primitive_quadrangle({ -0.5f,0,0 }, { 0.5f,0,0 }, { 0.5f,0,1 }, { -0.5f,0,1 });
	quad_1.initialize_data_on_gpu(quad_mesh);
	quad_2.initialize_data_on_gpu(quad_mesh);

	quad_1.texture.load_and_initialize_texture_2d_on_gpu(project::path+"assets/grass.png");
	quad_2.texture.load_and_initialize_texture_2d_on_gpu(project::path+"assets/window_red.png");

	quad_1.material.phong = { 0.4f, 0.6f,0,1 };
	quad_2.material.phong = { 0.4f, 0.6f,0,1 };

	quad_1.model.translation = { 0,-0.5f,0 };
	quad_2.model.translation = { 0,+0.5f,0 };


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


	draw(floor, environment);

	if (gui.display_wireframe) {
		draw_wireframe(quad_1, environment);
		draw_wireframe(quad_2, environment);
	}

	// We display the semi-transparent shapes after the non-transparent ones 
	//   (as semi-transparent shapes are not associated to depth buffer write)
	display_semiTransparent();

}

void scene_structure::display_semiTransparent()
{
	// Enable use of alpha component as color blending for transparent elements
	//  alpha = current_color.alpha
	//  new color = previous_color * alpha + current_color * (1-alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);

	auto const& camera = camera_control.camera_model;

	// Re-orient the grass shape to always face the camera direction
	vec3 const right = camera.right();
	// Rotation such that the grass follows the right-vector of the camera, while pointing toward the z-direction
	rotation_transform R = rotation_transform::from_frame_transform({ 1,0,0 }, { 0,0,1 }, right, { 0,0,1 });
	quad_1.model.rotation = R;


	// Sort transparent shapes by depth to camera
	//   This step can be skipped, but it will be associated to visual artifacts

	// Transform matrix (the same matrix which is applied in the vertices in the shader: T = Projection x View)
	mat4 T = camera_projection.matrix() * camera.matrix_view();
	// Projected vertices (center of quads) in homogeneous coordinates
	vec4 p1 = T * vec4{ 0, -0.5f, 0, 1 };
	vec4 p2 = T * vec4{ 0, +0.5f, 0, 1 };
	// Depth to camera
	float z1 = p1.z / p1.w;
	float z2 = p2.z / p2.w;

	// Display the quads relative to their depth
	if (z1 <= z2) {
		draw(quad_2, environment);
		draw(quad_1, environment);
	}
	else {
		draw(quad_1, environment);
		draw(quad_2, environment);
	}

	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
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
	std::cout << "Transparency." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}