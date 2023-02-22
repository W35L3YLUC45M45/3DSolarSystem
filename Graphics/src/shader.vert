#version 330

in vec3 a_vertex;
in vec3 a_color;
in vec3 a_normal;

in vec2 a_uv;

out vec3 v_normal;
out vec3 v_vertex;
out vec2 v_uv;
out vec3 v_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform mat3 u_normal;


void main()
{
	// position of the vertex
	gl_Position = u_projection * u_view * u_model * vec4( a_vertex , 1.0 );			//MVP 

	v_normal = u_normal * a_normal;


    	// STEP 2: pass the uv coordinates to the fragment shader
	v_uv = a_uv;

	
	v_vertex = (u_model * vec4( a_vertex , 1.0 )).xyz;

}

