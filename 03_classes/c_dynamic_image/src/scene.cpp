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
		{0.0f,0.0f, 3.0f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,1,0} /* direction of the "up" vector */);

	// Configure orthographic projection for 2D-style viewing
	// Range [-1.1, 1.1] in both x and y to frame the quad with some margin
	camera_projection = camera_projection_orthographic{ -1.1f, 1.1f, -1.1f, 1.1f, -10, 10, window.aspect_ratio()};
	// Optional: could be a perspective projection instead (to be changed in the header file: camera_projection_orthographic -> camera_projection_perspective)

	// General information
	display_info();

	// Create 3D coordinate frame (x, y, z axes) for visual reference
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// Initialize the shapes of the scene
	// ***************************************** //

	// Create a quad (2x2 square) centered at origin in the xy-plane
	quad.initialize_data_on_gpu(mesh_primitive_quadrangle({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }));
	quad.material.color = { 1.0f,1.0f,1.0f }; // White base color for texture display
	quad.material.phong = { 1.0f, 0.0f, 0.0f, 128.0f }; // Ambient only, no diffuse/specular lighting

	// Set up the animated texture and pixel grid overlay
	initialize_image_animation();
	
	// Create black border frame around the quad
	initialize_border();

}


// Initialize the image animation system
// - Clears any existing texture data from the quad
// - Initializes the image data structure (sets size and initial content)
// - Creates a 2D GPU texture from the image data with:
//   * GL_CLAMP_TO_EDGE: prevents texture wrapping at edges
//   * GL_NEAREST: uses nearest-neighbor filtering for sharp pixel display (no blurring)
//     Optional: GL_LINEAR for linear interpolation
// - Sets up the grid overlay for visualizing individual pixels
void scene_structure::initialize_image_animation()
{
	image_animation_initialization(img);

	if(quad.texture.id != mesh_drawable::default_texture.id)
		quad.texture.clear();
	quad.texture.initialize_texture_2d_on_gpu(img.data, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, false, GL_NEAREST, GL_NEAREST);
	quad.material.texture_settings.inverse_v = false;
	initialize_grid_display();

}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
    camera_projection.aspect_ratio = window.aspect_ratio();
	environment.camera_projection = camera_projection.matrix();
	environment.camera_view = camera_control.camera_model.matrix_view();
	environment.light = camera_control.camera_model.position();

	// Draw the 3D reference frame axes if enabled
	if (gui.display_frame)
		draw(global_frame, environment);

	// Draw the main quad displaying the animated texture
	draw(quad, environment);
	
	// Draw the black border around the quad
	draw(border, environment);
	
	// Draw the pixel grid overlay if enabled in GUI
	if( gui.display_grid )
		draw(grid_display, environment);
	
	// Update the image animation based on current time
	image_animation_display_frame(img, timer.t);
	
	// Upload the modified image data to the GPU texture
	quad.texture.update(img.data);
	
	// Update the timer for next frame
	timer.update();


}


void scene_structure::display_gui()
{

	ImGui::Checkbox("Display grid", &gui.display_grid);

	// Commented for 2D view
	//ImGui::Checkbox("Frame", &gui.display_frame);
}



void scene_structure::initialize_grid_display()
{
	float eps = 0.001f; // Small z-offset to render grid in front of quad
	float L = 0.002f;   // Half-width of each grid line

	mesh mesh_grid;

	// Create vertical lines: one thin quad for each column boundary
	for(int k=0; k<img.data.dimension.x+1; ++k) {
		float u = -1.0f + 2.0f * float(k) / float(img.data.dimension.x);
		mesh_grid.push_back( mesh_primitive_quadrangle({ u-L,-1.0f,eps }, { u+L,-1.0f,eps }, { u+L,1.0f,eps }, { u-L,1.0f,eps }) );
	}

	// Create horizontal lines: one thin quad for each row boundary
	for(int k=0; k<img.data.dimension.y+1; ++k) {
		float v = -1.0f + 2.0f * float(k) / float(img.data.dimension.y);
		mesh_grid.push_back( mesh_primitive_quadrangle({ -1.0f,v-L,eps }, { 1.0f,v-L,eps }, { 1.0f,v+L,eps }, { -1.0f,v+L,eps }) );
	}

	// Upload grid geometry to GPU and configure display properties
	grid_display.clear();
	grid_display.initialize_data_on_gpu(mesh_grid);
	grid_display.material.color = { 0.0f, 0.0f, 0.0f }; // Black grid lines
	grid_display.material.phong = { 1.0f, 0.0f, 0.0f, 128.0f }; // Ambient only
}

void scene_structure::initialize_border()
{
	// Border thickness and depth offset
	float eps = 0.002f; // Small z-offset to render border in front of quad
	float L = 0.01f;    // Border width extending from the quad edges

	mesh mesh_border;
	
	// Create four rectangular strips forming a frame around the [-1, 1] x [-1, 1] quad
	// Each quadrangle extends L units beyond the quad edges to create the border thickness
	
	// Bottom edge: horizontal strip along y = -1
	mesh_border.push_back( mesh_primitive_quadrangle({ -1.0f-L,-1.0f-L,eps }, { 1.0f+L,-1.0f-L,eps }, { 1.0f+L,-1.0f+L,eps }, { -1.0f-L,-1.0f+L,eps } ) );
	
	// Top edge: horizontal strip along y = 1
	mesh_border.push_back( mesh_primitive_quadrangle({ -1.0f-L,1.0f-L,eps }, { 1.0f+L,1.0f-L,eps }, { 1.0f+L,1.0f+L,eps }, { -1.0f-L,1.0f+L,eps } ) );
	
	// Right edge: vertical strip along x = 1
	mesh_border.push_back( mesh_primitive_quadrangle({ 1.0f-L,-1.0f-L,eps }, { 1.0f+L,-1.0f-L,eps }, { 1.0f+L,1.0f+L,eps }, { 1.0f-L,1.0f+L,eps } ) );
	
	// Left edge: vertical strip along x = -1
	mesh_border.push_back( mesh_primitive_quadrangle({ -1.0f-L,-1.0f-L,eps }, { -1.0f+L,-1.0f-L,eps }, { -1.0f+L,1.0f+L,eps }, { -1.0f-L,1.0f+L,eps } ) );

	// Upload border mesh to GPU and configure material properties
	border.initialize_data_on_gpu(mesh_border);
	border.material.color = {0.0f, 0.0f, 0.0f}; // Black border
	border.material.phong = {1.0f, 0.0f, 0.0f, 128.0f}; // Ambient lighting only (no specular/diffuse)
}

void scene_structure::mouse_move_event()
{
	// Remove this comment to have rotating/interactive camera
	/*
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
	*/
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

	std::cout << "\nSCENE INFO:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Display of an image as a texture." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}