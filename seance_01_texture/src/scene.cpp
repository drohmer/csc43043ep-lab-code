#include "scene.hpp"


using namespace cgp;


// Main initialization function called once at program startup
// Sets up the camera, 3D scene elements, and the image animation system
void scene_structure::initialize()
{
	// Configure orthographic projection for 2D-style viewing
	// Range [-1.1, 1.1] in both x and y to frame the quad with some margin
	camera_projection = camera_projection_orthographic{ -1.1f, 1.1f, -1.1f, 1.1f, -10, 10, window.aspect_ratio()};
	// Optionnal: could be a perspective projection instead (to be changed in the header file: camera_projection_orthographic -> camera_projection_perspective)
	
	// Position camera looking at origin from positive z-axis
	// Parameters: camera_position, look_at_target, up_direction
	camera_control.look_at({ 0.0f, 0.0f, 3.0f }, {0,0,0}, {0,1,0});

	// Connect camera controller to input system and window
	camera_control.initialize(inputs, window);
	
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
	quad.texture.clear();
	image_animation_initialization(img);
	quad.texture.initialize_texture_2d_on_gpu(img.data, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, false, GL_NEAREST, GL_NEAREST);
	initialize_grid_display();

}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
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
	cgp::numarray<vec3> grid_lines;
	
	// Create vertical lines: one line for each column boundary in the image grid
	for(int k=0; k<img.data.dimension.x+1; ++k) {
		// Map pixel index k to normalized coordinates [-1, 1]
		float u = -1.0f + 2.0f * float(k) / float(img.data.dimension.x );
		// Add a vertical line from bottom to top at position u
		grid_lines.push_back({ u, -1.0f, 0.001f });
		grid_lines.push_back({ u,  1.0f, 0.001f });
	}
	
	// Create horizontal lines: one line for each row boundary in the image grid
	for(int k=0; k<img.data.dimension.y+1; ++k) {
		// Map pixel index k to normalized coordinates [-1, 1]
		float v = -1.0f + 2.0f * float(k) / float(img.data.dimension.y );
		// Add a horizontal line from left to right at position v
		grid_lines.push_back({ -1.0f, v, 0.001f });
		grid_lines.push_back({  1.0f, v, 0.001f });
	}
	
	// Upload grid geometry to GPU and configure display properties
	grid_display.clear();
	grid_display.initialize_data_on_gpu(grid_lines);
	grid_display.display_type = curve_drawable_display_type::Segments; // Draw as line segments
	grid_display.color = { 0.0f, 0.0f, 0.0f }; // Black color for grid lines
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
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
	
}