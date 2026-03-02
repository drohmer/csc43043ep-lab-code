#include "scene.hpp"


using namespace cgp;


// Main initialization function called once at program startup
// Sets up the camera, 3D scene elements, and the image animation system
void scene_structure::initialize()
{
	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(inputs, window); 
	camera_control.set_rotation_axis_y(); // camera rotates around y-axis
	//   look_at(camera_position, targeted_point, up_direction)
	camera_control.look_at(
		{ 6.0f, 3.0f, 1.0f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,1,0} /* direction of the "up" vector */);



	// General information
	display_info();

	// Create 3D coordinate frame (x, y, z axes) for visual reference
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the shapes seen in the 3D scene
	// ********************************************** //
	

	sphere.initialize_data_on_gpu(mesh_primitive_sphere(1.0f));
	sphere.material.color = { 0.5f,0.8f,1.0f };

	float const L = 0.35f; //size of the quad
	billboard.initialize_data_on_gpu(mesh_primitive_quadrangle({ -L,-L,0 }, { L,-L,0 }, { L,L,0 }, { -L,L,0 }));
	billboard.texture.load_and_initialize_texture_2d_on_gpu(project::path+"assets/smoke.png");

	// Load 3D model of pot
	cooking_pot.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/cauldron.obj"));
	cooking_pot.material.color = { 0.9f, 0.8f, 0.6f };
	cooking_pot.model.translation = { -0.1f, -0.3f, 0.0f };
	cooking_pot.model.scaling = 0.43f;

	// Load 3D model of spoon
	spoon.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/spoon.obj"));
	spoon.material.color = { 0.9f, 0.8f, 0.6f };
	spoon.model.translation = { -0.1f, -0.3f, 0 };
	spoon.model.scaling = 0.43f;

	// Create the flat surface representing the liquid in the pot
	int N_liquid_sample = 20;
	liquid_surface.initialize_data_on_gpu(mesh_primitive_grid({-1,0,-1},{-1,0,1},{1,0,1},{1,0,-1},N_liquid_sample, N_liquid_sample));
	liquid_surface.shader.load(project::path+"shaders/water/water.vert.glsl",project::path+"shaders/water/water.frag.glsl"); // special shader for the liquid surface
liquid_surface.material.color = { 0.5f,0.6f,0.8f };
	liquid_surface.material.phong = { 0.7f, 0.3f, 0.0f, 128 };


	// timer_bubble/billboard.event_period is the time between two spawns for the bubbles/billboards
	timer_bubble.event_period = 0.2f;
	timer_billboard.event_period = 0.05f;

	// Remove warnings for unset uniforms
	cgp_warning::max_warning = 0;

	std::cout << "End function scene_structure::initialize()" << std::endl;

}




void scene_structure::display_bubble()
{
	// Evaluate the positions and display the bubbles
	int const N = particle_system.bubbles.size();
	for (int k = 0; k < N; ++k)
	{
		// Current particle
		particle_bubble& particle = particle_system.bubbles[k];
		// Evaluate the current position of the particle
		vec3 const p = particle.evaluate_position(timer_bubble.t);

		sphere.model.translation = p;
		sphere.model.scaling = particle.radius;
		sphere.material.color = particle.color;

		draw(sphere, environment);
	}
}

void scene_structure::display_billboard()
{
	// Enable transparency using alpha blending (if display_transparent_billboard is true)	
	if (gui.display_transparent_billboard) {
		glEnable(GL_BLEND);
		glDepthMask(false);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glDisable(GL_BLEND);


	// Evaluate the positions and display the billboards
	int const N = particle_system.billboards.size();
	for (int k = 0; k < N; ++k)
	{
		particle_billboard& particle = particle_system.billboards[k];
		vec3 const p = particle.evaluate_position(timer_bubble.t);
		billboard.model.translation = p;
		// Display the sprites here
		//  Note : to make the sprite constantly facing the camera set
		//    billboard.model.rotation = camera_control.camera_model.orientation();
		//    ...
		//  The transparency of the sprite can be set using
		//    billboard.material.alpha = value in [0,1] (1=opaque, 0=transparent)
		//  The current life time of the billboard (in sec) can be obtained via: timer_billboard.t - particle.t0
		//
draw(billboard, environment);
		if(gui.display_wireframe) {
			draw_wireframe(billboard, environment);
		}
	}

	glDepthMask(true);
}

void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
    camera_projection.aspect_ratio = window.aspect_ratio();
	environment.camera_projection = camera_projection.matrix();
	environment.camera_view = camera_control.camera_model.matrix_view();
	environment.light = camera_control.camera_model.position();

	
	if (gui.display_frame)
		draw(global_frame, environment);

	// Static elements
	draw(spoon, environment);
	draw(cooking_pot, environment);
	draw(liquid_surface, environment);

	if(gui.display_wireframe) {
		draw_wireframe(liquid_surface, environment);
	}

	// Timers
	timer_bubble.update();
	timer_billboard.update();
	timer.update();

	environment.uniform_generic.uniform_float["time"] = timer.t;

	// Handling creation/remove of particles
	if (timer_bubble.event)
		particle_system.bubbles.push_back(particle_bubble(timer_bubble.t));
	if (timer_billboard.event)
		particle_system.billboards.push_back(particle_billboard(timer_billboard.t));

	particle_system.remove_old_particles(timer_bubble.t);


	if(gui.display_sphere) {
		display_bubble();
	}
	if(gui.display_billboard){
		display_billboard();
	}

}



void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	ImGui::Checkbox("Transparent billboard", &gui.display_transparent_billboard);
	ImGui::SliderFloat("Bubble spawn time", &timer_bubble.event_period, 0.05f, 2.0f);
	ImGui::SliderFloat("Smoke spawn time", &timer_billboard.event_period, 0.01f, 0.5f);

	ImGui::Checkbox("Display sphere", &gui.display_sphere);
	ImGui::Checkbox("Display billboard", &gui.display_billboard);
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
	std::cout << "Procedural scene." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}