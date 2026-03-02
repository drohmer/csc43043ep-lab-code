#pragma once


#include "cgp/cgp.hpp"

#include "environment.hpp"



using cgp::mesh_drawable;

// Variables associated to the GUI (buttons, etc)
struct gui_parameters {
	bool display_frame = false;
	bool display_wireframe = false;

	vec3 light_color = { 1,1,1 };
	float ambiant = 0.3f;
	float diffuse = 0.55f;
	float specular = 0.6f;
	float specular_exp = 64.0f;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
	
	// ****************************** //
	// Standard Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display_frame();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop

	// ****************************** //
	// Context
	// ****************************** //

	// Environment controler (background color, )
	environment_structure environment; 
	// Window where the scene is displayed
	window_structure window; 
	// Storage for inputs status (mouse, keyboard, window dimension)
	input_devices inputs; 
	// Standard GUI element storage
	gui_parameters gui;     
	
	// Display information at the start of the program
	void display_info();

	// ****************************** //
	// Camera controler
	// ****************************** //

	// Controller of the camera (extrinsic parameters: position/orientation) -- to be adapted to the desired model and behavior
	camera_controller_orbit_euler camera_control; 


	// The model of camera projection (intrinsic parameters)
	camera_projection_perspective camera_projection;

	
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	mesh_drawable global_frame;          // The standard global frame

	mesh_drawable ground;
	mesh_drawable cube;
	mesh_drawable sphere;
	mesh_drawable camel;

	mesh_drawable sphere_light;





	// ****************************** //
	// Callback functions
	// ****************************** //
	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

};





