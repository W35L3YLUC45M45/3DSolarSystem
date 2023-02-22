#version 330

in vec3 v_color;
in vec3 v_normal;
in vec3 v_vertex;

// Getting the uv coordinates form the vertex shader
in vec2 v_uv;


out vec4 fragColor;

uniform vec3 u_light_dir;
uniform vec3 u_camp_pos;

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;

uniform float u_shininess;

uniform vec3 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_texture_normal;



mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{ // obtener vectores de borde del triángulo de píxeles
vec3 dp1 = dFdx(p);
vec3 dp2 = dFdy(p);
vec2 duv1 = dFdx(uv);
vec2 duv2 = dFdy(uv);
// resolver el sistema lineal
vec3 dp2perp = cross(dp2, N);
vec3 dp1perp = cross(N, dp1);
vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
// construir un scale-invariant frame
float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
return mat3(T * invmax, B * invmax, N);
}
// supongo N, el vértice interpolado normal y
// V, el view vector (vertex to eye)
vec3 perturbNormal(vec3 N, vec3 V, vec2 texcoord, vec3 normal_pixel)
{
normal_pixel = normal_pixel * 2.0 - 1.0;
mat3 TBN = cotangent_frame(N, V, texcoord);
return normalize(TBN * normal_pixel);
}



void main(void)
{
	//USING the uv coordinates as colors
		//fragColor =  vec4(v_uv, 0.0, 1.0);    
		//fragColor =  vec4(v_color,1.0);  

	
	// STEP 8: get the color from the texture
	vec4 texture_color = texture(u_texture, v_uv);	//Creamos el vector texture_color a partir del U,V
	//fragColor = vec4(texture_color.xyz, 1.0);
	

	vec3 texture_normal = texture(u_texture_normal, v_uv).xyz;


	vec3 N = normalize(v_normal);

	vec3 N_orig = N;
	N = perturbNormal(N, v_vertex, v_uv, texture_normal);
	N = mix(N_orig, N, 1.0f);

	vec3 L = normalize(u_light_dir - v_vertex);

	vec3 R = normalize(-reflect(L, N) );
	vec3 E = normalize(u_camp_pos - v_vertex);
	
	

	//Set the light											19/11/2020
	//float NdotL = max ( dot (N,L) , 0.0 ) ;
		//vec3 final_color = v_color * NdotL;				//Si quieres aplicar la luz al color del objeto
	//vec3 final_color = texture_color.xyz * NdotL;		//Si quieres aplicar la luz a la textura


	//Phong Equation:										26/11/2020
	
		//Scalar Products
		float NdotL = max ( dot (N,L) , 0.0 ) ;
		float RdotE = max ( dot (R,E) , 0.0 ) ;

	//3 main sections:
	vec3 color_light = (texture_color.xyz * u_ambient);
	vec3 diffuse_section = (texture_color.xyz * u_diffuse) * NdotL;
	vec3 specular_section = (texture_color.xyz * u_specular) * pow(RdotE, u_shininess);
	

	vec3 final_color = color_light + diffuse_section + specular_section;


	fragColor = vec4 (final_color, 1.0);
	
}
