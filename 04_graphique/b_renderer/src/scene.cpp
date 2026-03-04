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
	initialize_renderer();
	
	// Create black border frame around the quad
	initialize_border();

	gui.display_frame = false;

	camera_projection_perspective camera_perspective;
	renderer.perspective_matrix = camera_perspective.matrix();
	camera_renderer.set_rotation_axis_y();
	camera_renderer.look_at({ 0.0f, 0.0f, 3.0f }, {0,0,0}, {0,1,0});
	camera_renderer.initialize(inputs, window);
}


// Initialize the image animation system
// - Clears any existing texture data from the quad
// - Initializes the image data structure (sets size and initial content)
// - Creates a 2D GPU texture from the image data with:
//   * GL_CLAMP_TO_EDGE: prevents texture wrapping at edges
//   * GL_NEAREST: uses nearest-neighbor filtering for sharp pixel display (no blurring)
//     Optional: GL_LINEAR for linear interpolation
// - Sets up the grid overlay for visualizing individual pixels
void scene_structure::initialize_renderer(int Nx, int Ny)
{
	renderer.image_data.resize(Nx, Ny);
	renderer.depth_buffer.resize(Nx, Ny);
	renderer.clear();

	if(quad.texture.id != mesh_drawable::default_texture.id)
		quad.texture.clear();
	quad.texture.initialize_texture_2d_on_gpu(renderer.image_data, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, false, GL_NEAREST, GL_NEAREST);
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
	
	// Clear image and depth buffer before drawing shapes
	renderer.clear();
	renderer.view_matrix = camera_renderer.camera_model.matrix_view();
	vec3 p1 = {-0.5f, -0.5f, 0.0f};
	vec3 p2 = {0.5f, -0.5f, 0.0f};
	vec3 p3 = {0.0f, 0.5f, 0.0f};
	vec3 n = {0.0f, 0.0f, 1.0f};
	renderer.draw_triangle(
			p1,p2,p3,
			n, n, n,
			{1.0f, 0.0f, 0.0f});
	quad.texture.update(renderer.image_data);
	
}


void scene_structure::display_gui()
{
	bool grid_too_large = renderer.image_data.dimension.x > 100 || renderer.image_data.dimension.y > 100;
	if (grid_too_large) {
		gui.display_grid = false;
		ImGui::BeginDisabled();
	}
	ImGui::Checkbox("Display grid", &gui.display_grid);
	if (grid_too_large)
		ImGui::EndDisabled();

	ImGui::Text("Image Size");
	ImGui::PushItemWidth(100);
	ImGui::InputInt("##Width", &gui.image_size_x, 0, 0);
	ImGui::SameLine();
	ImGui::InputInt("##Height", &gui.image_size_y, 0, 0);
	ImGui::PopItemWidth();
	gui.image_size_x = std::max(gui.image_size_x, 1);
	gui.image_size_y = std::max(gui.image_size_y, 1);
	ImGui::SameLine();
	if (ImGui::Button("Resize")) {
		initialize_renderer(gui.image_size_x, gui.image_size_y);
	}

	ImGui::Separator();

	ImGui::Text("Background Color");
	ImGui::ColorEdit3("##Background", &renderer.background_color.x);

	ImGui::Separator();

	if (ImGui::TreeNode("Light Position")) {
		ImGui::SliderFloat("Light X", &renderer.light_position.x, -10.0f, 10.0f);
		ImGui::SliderFloat("Light Y", &renderer.light_position.y, -10.0f, 10.0f);
		ImGui::SliderFloat("Light Z", &renderer.light_position.z, -10.0f, 10.0f);
		ImGui::TreePop();
	}
}



void scene_structure::initialize_grid_display()
{
	cgp::numarray<vec3> grid_lines;

	if(renderer.image_data.dimension.x <= 100 && renderer.image_data.dimension.y <= 100) 
	{

		// Create vertical lines: one line for each column boundary in the image grid
		for(int k=0; k<renderer.image_data.dimension.x+1; ++k) {
			// Map pixel index k to normalized coordinates [-1, 1]
			float u = -1.0f + 2.0f * float(k) / float(renderer.image_data.dimension.x);
			// Add a vertical line from bottom to top at position u
			grid_lines.push_back({ u, -1.0f, 0.001f });
			grid_lines.push_back({ u,  1.0f, 0.001f });
		}

		// Create horizontal lines: one line for each row boundary in the image grid
		for(int k=0; k<renderer.image_data.dimension.y+1; ++k) {
			// Map pixel index k to normalized coordinates [-1, 1]
			float v = -1.0f + 2.0f * float(k) / float(renderer.image_data.dimension.y);
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

	if (!inputs.keyboard.shift) {
		//camera_control.action_mouse_move();
		camera_renderer.action_mouse_move();
	}
	
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
	std::cout << "Software rendering." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}