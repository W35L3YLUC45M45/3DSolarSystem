/*  by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) */

/*

Students:
· Gabriel Vallarta - gabriel.vallarta@students.salle.url.edu
· Wesley Lucas - wesley.lucas@students.salle.url.edu

*/

#define PI 3.14159 


//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>

//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//New includes for Projection
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>


#include "imageloader.h"
//include some custom code files
#include "glfunctions.h" //include all OpenGL stuff
#include "Shader.h" // class to compile shaders

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;
using namespace glm;




struct PLANET {

	int diameter; //(km)		//Volume = PI*((diameter/2)^3) /6;   ---> Scale
	int distance_from_sun;		//Radius Will affect only X or Y axis 	    
	int rotation_period;
	int orbital_period;

	float rotation_speed;   //2*PI*(diameter/2) / rotation_period =  rotation_speed;
	float orbital_speed;    //distance_from_sun /orbital_period =  orbital_speed;

} mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, moon, clouds;

struct SPACE_OBJ {

	vec3 origin;
	int distance_from_origin;
	int scale;

	int orbital_period;
	float orbital_speed;    //distance_from_origin /orbital_period =  orbital_speed;

	GLuint texture_id;
	vec3 position;

} x_wing, tie_fighter, tesla;



//Global variables to help us do things
//int g_ViewportWidth = 512; int g_ViewportHeight = 512; // Default window size, in pixels
float g_ViewportWidth = 1920.0f; float g_ViewportHeight = 1080.0f; // My resolution HP Pavilion //
double mouse_x, mouse_y; //variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f); // background colour - a GLM 3-component vector


string basepath = "assets/";
string inputfile = basepath + "sphere.obj";
vector< tinyobj::shape_t > shapes;
vector< tinyobj::material_t > materials;

//shader identifiers:
GLuint g_simpleShader_planet = 0;	
GLuint g_simpleShader_earth = 0;
GLuint g_simpleShader_sun = 0;
GLuint g_simpleShader_sky = 0;

//Global VAO's
GLuint g_Vao_sphere = 0;

//Global Number of triangles for different meshs
GLuint g_NumTriangles_sphere = 0;

//Normal map from the earth
GLuint texture_id_earth_normal;

//Global variable for the texture ID
GLuint texture_id_skybox;
GLuint texture_id_sun;
GLuint texture_id_mercury;
GLuint texture_id_venus;
GLuint texture_id_earth;
GLuint texture_id_mars;
GLuint texture_id_jupiter;
GLuint texture_id_saturn;
GLuint texture_id_uranus;
GLuint texture_id_neptune;


//Translation variables
float x_position = +3.0f;
float y_position = 0.0f;
float z_position = -20.0f;

//Rotation variables
float rotation_angle = 2.0f;
float rotation_angle_SUN = 2.0f;

//Scaling variables
float scale_factor = 1.0f;

//Rotation variables
float deltaRTime = 0.0f;
float lastRTime = 0.0f;
float ROTATE_SPEED = 0.015f;

float deltaTransTime = 0.0f;
float lastTransTime = 0.0f;
float TRANSLATE_SPEED = 0.015f;


//Moving Camera Variables
float deltaTime = 0.0f;
float lastTime = 0.0f;

float last_mouse_x = 0.0f;
float last_mouse_y = 0.0f;

bool is_down = false;
vec3 center = vec3(0.0f, 2.0f, 10.0f); //TODO z en -1 
vec3 eye = vec3(0.0f, 1.0f, 1.0f); //TODO todo en zeros
vec3 up = vec3(0.0f, 1.0f, 0.0f);

float cam_yaw = 0.0f;
float cam_pitch = 0.0f;

float MOVE_SPEED = 0.00015f;
float LOOK_SPEED = 0.00005f;

//global variables used for camera movement
int key_flags[] = { 0, 0, 0, 0 }; //w, a, s, d

//Global light variable for parallel light
vec3 g_light_dir(x_position -14, y_position, z_position);
//vec3 g_light_dir(10.0, 0.0, 10.0);

// ------------------------------------------------------------------------------------------
// This function will be called in the main and will help us with planet rotations
// ------------------------------------------------------------------------------------------
void mannageRotations() {

	//Rotation over AXIS 
	deltaRTime = (glfwGetTime() - lastRTime) * 1000;
	lastRTime = glfwGetTime();

	rotation_angle = rotation_angle + deltaRTime * ROTATE_SPEED;
	//cout << "Rotation angle: " << rotation_angle << endl;

	rotation_angle_SUN = rotation_angle_SUN + deltaRTime * ROTATE_SPEED;
	//cout << "Rotation angle: " << rotation_angle << endl;


}

void mannageTranslations() {

	deltaTransTime = (glfwGetTime() - lastTransTime) * 1000;
	lastTransTime = glfwGetTime();

	//Change for translations
	//rotation_angle = rotation_angle + deltaRTime * ROTATE_SPEED;

	mercury.diameter = 323;



	//Translation arround the Sun

		//Translation arround the Earth
			//Satelite
			//Moon



	//EXTRA ---> Paint Trajectory

}


// ------------------------------------------------------------------------------------------
// This function will help us load all the desired textures to our CPU
// ------------------------------------------------------------------------------------------
void loadTextures() {
	
	
	Image* image_sun = loadBMP("assets/sun.bmp");
	glGenTextures(1, &texture_id_sun);
	glBindTexture(GL_TEXTURE_2D, texture_id_sun);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_sun->width, image_sun->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_sun->pixels);

	Image* image_mercury = loadBMP("assets/mercury.bmp");
	glGenTextures(1, &texture_id_mercury);
	glBindTexture(GL_TEXTURE_2D, texture_id_mercury);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_mercury->width, image_mercury->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_mercury->pixels);

	Image* image_venus = loadBMP("assets/venus.bmp");
	glGenTextures(1, &texture_id_venus);
	glBindTexture(GL_TEXTURE_2D, texture_id_venus);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_venus->width, image_venus->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_venus->pixels);
	
	Image* image_earth = loadBMP("assets/earthmap1k.bmp");
	glGenTextures(1, &texture_id_earth);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_earth->width, image_earth->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_earth->pixels);

	Image* image_mars = loadBMP("assets/mars.bmp");
	glGenTextures(1, &texture_id_mars);
	glBindTexture(GL_TEXTURE_2D, texture_id_mars);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_mars->width, image_mars->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_mars->pixels);

	Image* image_jupiter = loadBMP("assets/jupiter.bmp");
	glGenTextures(1, &texture_id_jupiter);
	glBindTexture(GL_TEXTURE_2D, texture_id_jupiter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_jupiter->width, image_jupiter->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_jupiter->pixels);

	Image* image_saturn = loadBMP("assets/saturn.bmp");
	glGenTextures(1, &texture_id_saturn);
	glBindTexture(GL_TEXTURE_2D, texture_id_saturn);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_saturn->width, image_saturn->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_saturn->pixels);

	Image* image_uranus = loadBMP("assets/uranus.bmp");
	glGenTextures(1, &texture_id_uranus);
	glBindTexture(GL_TEXTURE_2D, texture_id_uranus);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_uranus->width, image_uranus->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_uranus->pixels);

	Image* image_neptune = loadBMP("assets/neptune.bmp");
	glGenTextures(1, &texture_id_neptune);
	glBindTexture(GL_TEXTURE_2D, texture_id_neptune);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_neptune->width, image_neptune->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_neptune->pixels);

	//Loading Milkyway
	Image* image_milkyway = loadBMP("assets/milkyway.bmp");
	glGenTextures(1, &texture_id_skybox);
	glBindTexture(GL_TEXTURE_2D, texture_id_skybox);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_milkyway->width, image_milkyway->height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image_milkyway->pixels);

	//Load the normal map as texture:
	Image* image_normal = loadBMP("assets/earthnormal.bmp");
	glGenTextures(1, &texture_id_earth_normal);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth_normal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_normal->width, image_normal->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_normal->pixels);
	
}

void myCreateAndBind(GLuint * shader) {

	vector<GLfloat> colors(shapes[0].mesh.positions.size(), 1.0);

	gl_createAndBindAttribute(&(shapes[0].mesh.positions[0]), shapes[0].mesh.positions.size() * sizeof(float), *shader, "a_vertex", 3);
	gl_createAndBindAttribute(&(colors[0]), colors.size() * sizeof(GLfloat), *shader, "a_color", 3);
	gl_createIndexBuffer(&(shapes[0].mesh.indices[0]), shapes[0].mesh.indices.size() * sizeof(unsigned int));

	//Creating the VBO to VAO with my Normals
	gl_createAndBindAttribute(&(shapes[0].mesh.normals[0]),
		shapes[0].mesh.normals.size() * sizeof(float), *shader, "a_normal", 3);

	// STEP 1: create a VBO for the texture coordinates
	gl_createAndBindAttribute(
		&(shapes[0].mesh.texcoords[0]),
		shapes[0].mesh.texcoords.size() * sizeof(GLfloat),
		*shader,
		"a_uv", 2);
}

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load()
{
	//Load all to memory
	string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), basepath.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	else
		std::cout << "Loaded " << inputfile << " with shapes: " << shapes.size() << std::endl;


	//We now load all Textures
	loadTextures();

	//Loading of each Planet & Star
	g_NumTriangles_sphere = shapes[0].mesh.indices.size() / 3;
	g_Vao_sphere = gl_createAndBindVAO();

	//load the EARTH shader
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_simpleShader_earth = simpleShader.program;
	myCreateAndBind(&g_simpleShader_earth);
	gl_unbindVAO();
	
	//load the SUN shader
	Shader simpleShader_sun("src/shader_sun.vert", "src/shader_sun.frag");
	g_simpleShader_sun = simpleShader_sun.program;
	myCreateAndBind(&g_simpleShader_sun);
	gl_unbindVAO();

	//Load the PLANETS Shader 
	Shader simpleShader_planet("src/shader_planet.vert.txt", "src/shader_planet.frag.txt");
	g_simpleShader_planet = simpleShader_planet.program;
	myCreateAndBind(&g_simpleShader_planet);
	gl_unbindVAO();

	//Load the SKYBOX Shader 
	Shader simpleShader_sky("src/shader_sky.vert.txt", "src/shader_sky.frag.txt");
	g_simpleShader_sky = simpleShader_sky.program;
	myCreateAndBind(&g_simpleShader_sky);
	gl_unbindVAO();
	

	std::cout << "Loading DONE" << std::endl;
}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	/////////////////////	SKYBOX	  //////////////////////////////

	glUseProgram(g_simpleShader_sky);

	GLuint u_sky_model = glGetUniformLocation(g_simpleShader_sky, "u_model");
	GLuint u_sky_view = glGetUniformLocation(g_simpleShader_sky, "u_view");
	GLuint u_sky_projection = glGetUniformLocation(g_simpleShader_sky, "u_projection");

	float my_Pavilion_ratio = g_ViewportWidth / g_ViewportHeight;

	//set MVP
	mat4 sky_model_matrix = translate(mat4(1.0f), eye);
	mat4 sky_view_matrix = lookAt(eye, center, vec3(0, 1, 0));
	mat4 sky_projection_matrix = perspective(90.0f, my_Pavilion_ratio, 0.1f, 50.0f);


	//send all values to shader
	glUniformMatrix4fv(u_sky_model, 1, GL_FALSE, glm::value_ptr(sky_model_matrix));
	glUniformMatrix4fv(u_sky_view, 1, GL_FALSE, glm::value_ptr(sky_view_matrix));
	glUniformMatrix4fv(u_sky_projection, 1, GL_FALSE, glm::value_ptr(sky_projection_matrix));


	GLuint u_sky_texture = glGetUniformLocation(g_simpleShader_sky, "u_texture");
	glUniform1i(u_sky_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_skybox);

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);
	

	/////////////////////	PLANETS	  //////////////////////////////

	// Now we mannage the planets
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader_planet);

	//Those from the SKY
	GLuint projection_loc_planets = glGetUniformLocation(g_simpleShader_planet, "u_projection");
	glUniformMatrix4fv(projection_loc_planets, 1, GL_FALSE, glm::value_ptr(sky_projection_matrix));
	GLuint view_loc_planets = glGetUniformLocation(g_simpleShader_planet, "u_view");
	glUniformMatrix4fv(view_loc_planets, 1, GL_FALSE, glm::value_ptr(sky_view_matrix));

	//Still using same shader as above "g_simpleShader_planet"

	//Pass light to shader
	GLuint light_loc_planets = glGetUniformLocation(g_simpleShader_planet, "u_light_dir");
	glUniform3f(light_loc_planets, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint camp_pos_loc2 = glGetUniformLocation(g_simpleShader_planet, "u_camp_pos");
	glUniform3f(camp_pos_loc2, eye.x, eye.y, eye.z);
	GLuint ambient_loc2 = glGetUniformLocation(g_simpleShader_planet, "u_ambient");
	glUniform3f(ambient_loc2, 0.2, 0.2, 0.2);
	GLuint diffuse_loc2 = glGetUniformLocation(g_simpleShader_planet, "u_diffuse");
	glUniform3f(diffuse_loc2, 1.0, 1.0, 1.0);
	GLuint specular_loc2 = glGetUniformLocation(g_simpleShader_planet, "u_specular");
	glUniform3f(specular_loc2, 1.0, 1.0, 1.0);
	GLuint shininess_loc2 = glGetUniformLocation(g_simpleShader_planet, "u_shininess");
	glUniform1f(shininess_loc2, 80.0);


	////////MERCURY////////
	GLuint u_mercury_texture = glGetUniformLocation(g_simpleShader_planet, "u_texture");
	glUniform1i(u_mercury_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id_mercury);

	GLuint model_loc_mercury = glGetUniformLocation(g_simpleShader_planet, "u_model");

	//Translate
	mat4 T_Mercury = translate(mat4(1.0f), vec3(x_position - 3, y_position -4, z_position)); //x, y, z
	//Rotate
	mat4 R_Mercury = rotate(mat4(1.0f), rotation_angle_SUN, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S_Mercury = scale(mat4(1.0f), vec3(scale_factor, scale_factor, scale_factor)); //x, y, z

	mat4 model_mercury = T_Mercury * R_Mercury * S_Mercury;
	glUniformMatrix4fv(model_loc_mercury, 1, GL_FALSE, glm::value_ptr(model_mercury));

	//Fix the light normals rotation
	mat3 normal_matrix_mercury = mat3(transpose(inverse(model_mercury)));
	GLuint normal_loc_mercury = glGetUniformLocation(g_simpleShader_planet, "u_normal");
	glUniformMatrix3fv(normal_loc_mercury, 1, GL_FALSE, glm::value_ptr(normal_matrix_mercury));

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);

	////////VENUS////////
	GLuint u_venus_texture = glGetUniformLocation(g_simpleShader_planet, "u_texture");
	glUniform1i(u_venus_texture, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_id_venus);

	GLuint model_loc_venus = glGetUniformLocation(g_simpleShader_planet, "u_model");

	//Translate
	mat4 T_Venus = translate(mat4(1.0f), vec3(x_position - 1, y_position + 2, z_position)); //x, y, z
	//Rotate
	mat4 R_Venus = rotate(mat4(1.0f), rotation_angle_SUN, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S_Venus = scale(mat4(1.0f), vec3(1.25*scale_factor, 1.25*scale_factor, 1.25*scale_factor)); //x, y, z

	mat4 model_venus = T_Venus * R_Venus * S_Venus;
	glUniformMatrix4fv(model_loc_venus, 1, GL_FALSE, glm::value_ptr(model_venus));

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);

	////////MARS////////
	GLuint u_mars_texture = glGetUniformLocation(g_simpleShader_planet, "u_texture");
	glUniform1i(u_mars_texture, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture_id_mars);

	GLuint model_loc_mars = glGetUniformLocation(g_simpleShader_planet, "u_model");

	//Translate
	mat4 T_Mars = translate(mat4(1.0f), vec3(x_position + 4.5, y_position + 6, z_position)); //x, y, z
	//Rotate
	mat4 R_Mars = rotate(mat4(1.0f), rotation_angle_SUN, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S_Mars = scale(mat4(1.0f), vec3(scale_factor, scale_factor, scale_factor)); //x, y, z

	mat4 model_mars = T_Mars * R_Mars * S_Mars;
	glUniformMatrix4fv(model_loc_mars, 1, GL_FALSE, glm::value_ptr(model_mars));

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);

	////////JUPITER////////
	GLuint u_jupiter_texture = glGetUniformLocation(g_simpleShader_planet, "u_texture");
	glUniform1i(u_jupiter_texture, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture_id_jupiter);

	GLuint model_loc_jupiter = glGetUniformLocation(g_simpleShader_planet, "u_model");

	//Translate
	mat4 T_J = translate(mat4(1.0f), vec3(x_position + 8, y_position - 5, z_position)); //x, y, z
	//Rotate
	mat4 R_J = rotate(mat4(1.0f), rotation_angle_SUN, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S_J = scale(mat4(1.0f), vec3(2*scale_factor, 2*scale_factor, 2*scale_factor)); //x, y, z

	mat4 model_jupiter = T_J * R_J * S_J;
	glUniformMatrix4fv(model_loc_jupiter, 1, GL_FALSE, glm::value_ptr(model_jupiter));

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);

	////////SATURN////////
	GLuint u_saturn_texture = glGetUniformLocation(g_simpleShader_planet, "u_texture");
	glUniform1i(u_saturn_texture, 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture_id_saturn);

	GLuint model_loc_saturn = glGetUniformLocation(g_simpleShader_planet, "u_model");

	//Translate
	mat4 T_S = translate(mat4(1.0f), vec3(x_position + 10, y_position + 4, z_position)); //x, y, z
	//Rotate
	mat4 R_S = rotate(mat4(1.0f), rotation_angle_SUN, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S_S = scale(mat4(1.0f), vec3(1.6*scale_factor, 1.6*scale_factor, 1.6*scale_factor)); //x, y, z

	mat4 model_saturn = T_S * R_S * S_S;
	glUniformMatrix4fv(model_loc_saturn, 1, GL_FALSE, glm::value_ptr(model_saturn));

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);

	////////URANUS////////
	GLuint u_uranus_texture = glGetUniformLocation(g_simpleShader_planet, "u_texture");
	glUniform1i(u_uranus_texture, 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, texture_id_uranus);

	GLuint model_loc_uranus = glGetUniformLocation(g_simpleShader_planet, "u_model");

	//Translate
	mat4 T_U = translate(mat4(1.0f), vec3(x_position + 13, y_position - 2, z_position)); //x, y, z
	//Rotate
	mat4 R_U = rotate(mat4(1.0f), rotation_angle_SUN, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S_U = scale(mat4(1.0f), vec3(scale_factor, scale_factor, scale_factor)); //x, y, z

	mat4 model_uranus = T_U * R_U * S_U;
	glUniformMatrix4fv(model_loc_uranus, 1, GL_FALSE, glm::value_ptr(model_uranus));

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);

	////////NEPTUNE////////
	GLuint u_neptune_texture = glGetUniformLocation(g_simpleShader_planet, "u_texture");
	glUniform1i(u_neptune_texture, 7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, texture_id_neptune);

	GLuint model_loc_neptune = glGetUniformLocation(g_simpleShader_planet, "u_model");

	//Translate
	mat4 T_N = translate(mat4(1.0f), vec3(x_position + 18, y_position, z_position)); //x, y, z
	//Rotate
	mat4 R_N = rotate(mat4(1.0f), rotation_angle_SUN, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S_N = scale(mat4(1.0f), vec3(1.3*scale_factor, 1.3*scale_factor, 1.3*scale_factor)); //x, y, z

	mat4 model_neptune = T_N * R_N * S_N;
	glUniformMatrix4fv(model_loc_neptune, 1, GL_FALSE, glm::value_ptr(model_neptune));

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);


	/////////////////////	EARTH	  //////////////////////////////

	//We change to the Earth Shader
	glUseProgram(g_simpleShader_earth);

	GLuint u_texture = glGetUniformLocation(g_simpleShader_earth, "u_texture");
	//Bind the sampler to the texture unit 0
	glUniform1i(u_texture, 0);
	//Activate texture unit 0 and bind texture to object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth);

	GLuint u_texture_normal = glGetUniformLocation(g_simpleShader_earth, "u_texture_normal");
	//Bind the sampler to the texture unit 1
	glUniform1i(u_texture_normal, 1);
	//Activate texture unit 1 and bind texture to object
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth_normal);

	//Pass light to shader
	GLuint light_loc = glGetUniformLocation(g_simpleShader_earth, "u_light_dir");
	glUniform3f(light_loc, g_light_dir.x, g_light_dir.y, g_light_dir.z);
	GLuint camp_pos_loc = glGetUniformLocation(g_simpleShader_earth, "u_camp_pos");
	glUniform3f(camp_pos_loc, eye.x, eye.y, eye.z);
	GLuint ambient_loc = glGetUniformLocation(g_simpleShader_earth, "u_ambient");
	glUniform3f(ambient_loc, 0.2, 0.2, 0.2);
	GLuint diffuse_loc = glGetUniformLocation(g_simpleShader_earth, "u_diffuse");
	glUniform3f(diffuse_loc, 1.0, 1.0, 1.0);
	GLuint specular_loc = glGetUniformLocation(g_simpleShader_earth, "u_specular");
	glUniform3f(specular_loc, 1.0, 1.0, 1.0);
	GLuint shininess_loc = glGetUniformLocation(g_simpleShader_earth, "u_shininess");
	glUniform1f(shininess_loc, 80.0);


	//Those from the SKY
	GLuint projection_loc = glGetUniformLocation(g_simpleShader_earth, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(sky_projection_matrix));
	GLuint view_loc = glGetUniformLocation(g_simpleShader_earth, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(sky_view_matrix));


	GLuint model_loc = glGetUniformLocation(g_simpleShader_earth, "u_model");

	//Translate
	mat4 T = translate(mat4(1.0f), vec3(x_position + 3, y_position - 1, z_position)); //x, y, z
	//Rotate
	mat4 R = rotate(mat4(1.0f), rotation_angle, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S = scale(mat4(1.0f), vec3(1.3*scale_factor, 1.3*scale_factor, 1.3*scale_factor)); //x, y, z

	mat4 model = T * R * S;
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

	//Fix the light normals rotation
	mat3 normal_matrix = mat3(transpose(inverse(model)));
	GLuint normal_loc = glGetUniformLocation(g_simpleShader_earth, "u_normal");
	glUniformMatrix3fv(normal_loc, 1, GL_FALSE, glm::value_ptr(normal_matrix));


	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);



	/////////////////////	SUN	  //////////////////////////////

	//We change to the SUN Shader
	glUseProgram(g_simpleShader_sun);
	

	//Find the sample uniform in the shader 
	GLuint u_texture_sun = glGetUniformLocation(g_simpleShader_sun, "u_texture");

	//Bind the sampler to the texture unit 
	glUniform1i(u_texture_sun, 8);

	//Activate texture unit 0 and bind texture to object
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, texture_id_sun);

	//Those from the SKY
	GLuint projection_loc_sun = glGetUniformLocation(g_simpleShader_sun, "u_projection");
	glUniformMatrix4fv(projection_loc_sun, 1, GL_FALSE, glm::value_ptr(sky_projection_matrix));
	GLuint view_loc_sun = glGetUniformLocation(g_simpleShader_sun, "u_view");
	glUniformMatrix4fv(view_loc_sun, 1, GL_FALSE, glm::value_ptr(sky_view_matrix));

	
	GLuint model_loc_sun = glGetUniformLocation(g_simpleShader_sun, "u_model");
	
	//Translate
	mat4 T_Sun = translate(mat4(1.0f), vec3(x_position - 14, y_position, z_position)); //x, y, z
	//Rotate
	mat4 R_Sun = rotate(mat4(1.0f), rotation_angle_SUN, vec3(0.0f, 1.0f, 0.0f)); //x, y, z
	//Scale
	mat4 S_Sun = scale(mat4(1.0f), vec3(7*scale_factor, 7*scale_factor, 7*scale_factor)); //x, y, z

	mat4 model_sun = T_Sun * R_Sun * S_Sun;
	glUniformMatrix4fv(model_loc_sun, 1, GL_FALSE, glm::value_ptr(model_sun));

	gl_bindVAO(g_Vao_sphere);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);
}


// ------------------------------------------------------------------------------------------
// This function is called every time you press a screen
// ------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  
	if (action == GLFW_PRESS)
		switch (key)
		{
		case GLFW_KEY_W:
			key_flags[0] = 1;
			break;
		case GLFW_KEY_A:
			key_flags[1] = 1;
			break;
		case GLFW_KEY_S:
			key_flags[2] = 1;
			break;
		case GLFW_KEY_D:
			key_flags[3] = 1;
			break;
		case GLFW_KEY_R:
			eye = vec3(0.0f, 1.0f, 1.0f);
			cam_yaw = 0.0f;
			cam_pitch = 0.0f;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, 1);
			break;
		}
	else if (action == GLFW_RELEASE)
		switch (key)
		{
		case GLFW_KEY_W:
			key_flags[0] = 0;
			break;
		case GLFW_KEY_A:
			key_flags[1] = 0;
			break;
		case GLFW_KEY_S:
			key_flags[2] = 0;
			break;
		case GLFW_KEY_D:
			key_flags[3] = 0;
			break;
		}
}

// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		is_down = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		is_down = false;
	}
}

void onMouseMove(GLFWwindow* window, double x, double y) {
	mat4 tr = translate(mat4(1.0), eye);

	if (is_down) {
		cam_yaw += (last_mouse_x - x) * LOOK_SPEED * deltaTime;
		cam_pitch += (last_mouse_y - y) * LOOK_SPEED * deltaTime;                                // no z
	}

	last_mouse_x = x;
	last_mouse_y = y;

}

// --------------------------------------------------------------
// rotate camera according to pitch and yaw,
// move camera according to key flags
// --------------------------------------------------------------
void update() {

	//reset camera to look down z-axis
	vec3 initial_look_vector = vec3(0, 0, -1);
	vec3 aux_vector = vec3(0, 1, 0);
	float cam_pitch_aux = cam_pitch * 57.19f;
	float cam_yaw_aux = cam_yaw * 57.19f;

	//rotate look vector around y_axis (yaw)
	vec3 first_rotation = rotate(initial_look_vector, cam_yaw_aux, aux_vector);
	//rotate x-axis by same amount - this avoids gimbal lock
	vec3 intermediate_axis = rotate(vec3(1, 0, 0), cam_yaw_aux, aux_vector);
	//now rotate pitch around intermediate axis
	vec3 final_look_vector = rotate(first_rotation, cam_pitch_aux, intermediate_axis);


	//set camera target to be position + our new look vector
	center = eye + final_look_vector;

	// use time to control the speed consistently across machines
	deltaTime = (glfwGetTime() - lastTime) * 10000;
	lastTime = glfwGetTime();

	//get forward and side vectors for movement
	vec3 forward = normalize(center - eye);
	vec3 side = cross(vec3(0, 1, 0), forward);

	// WASD = forward / back / strafe left / strafe right
	if (key_flags[0]) { // W
		eye = eye + forward * MOVE_SPEED * deltaTime;
		center = center + forward * MOVE_SPEED * deltaTime;
	}
	if (key_flags[1]) { // A
		eye = eye + side * MOVE_SPEED * deltaTime;
		center = center + side * MOVE_SPEED * deltaTime;
	}
	if (key_flags[2]) { // S
		eye = eye - forward * MOVE_SPEED * deltaTime;
		center = center - forward * MOVE_SPEED * deltaTime;
	}
	if (key_flags[3]) { // D
		eye = eye - side * MOVE_SPEED * deltaTime;
		center = center - side * MOVE_SPEED * deltaTime;
	}
}


int main(void)
{
	//setup window and boring stuff, defined in glfunctions.cpp
	GLFWwindow* window;
	if (!glfwInit())return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "Welcome to Space", NULL, NULL);
	if (!window) {glfwTerminate();	return -1;}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();


	//input callbacks
	glfwSetCursorPosCallback(window, onMouseMove);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	//load all the resources
	load();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
		draw();
		update();
		mannageRotations();
		
        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
        
        //mouse position must be tracked constantly (callbacks do not give accurate delta)
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
    }

    //terminate glfw and exit
    glfwTerminate();
    return 0;
}


