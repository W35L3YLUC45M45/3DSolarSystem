#version 330

in vec3 v_color;
in vec3 v_normal;
in vec3 v_vertex;

// Getting the uv coordinates form the vertex shader
in vec2 v_uv;


out vec4 fragColor;

uniform vec3 u_color;
uniform sampler2D u_texture;


void main(void)
{

	vec4 texture_color = texture(u_texture, v_uv);	//Creamos el vector texture_color a partir del U,V
	fragColor = vec4 (texture_color.xyz, 1.0);
	
}
