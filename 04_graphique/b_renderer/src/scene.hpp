#pragma once


#include "cgp/cgp.hpp"

#include "environment.hpp"
#include "renderer.hpp"


using cgp::mesh_drawable;
struct gui_parameters {
	bool display_frame = false;
	bool display_grid = false;
	int image_size_x = 64;
	int image_size_y = 64;
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
	camera_projection_orthographic camera_projection;

	
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	mesh_drawable global_frame;          // The standard global frame

	mesh_drawable quad;
camera_controller_orbit_euler camera_renderer; 
	renderer_structure renderer;
	void initialize_renderer(int Nx = 64, int Ny = 64);
	curve_drawable grid_display;
	void initialize_grid_display();
	mesh_drawable border;
	void initialize_border();


	// ****************************** //
	// Callback functions
	// ****************************** //
	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

};





