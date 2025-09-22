 #version 430

 layout (location=0) in vec3 vertPos;
 layout (location=1) in vec3 vertNormal;
 layout (location=2) in vec2 texCoord;

 out vec4 varyingColor;
 out vec2 tc; 

 struct PositionalLight
 { 
	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	vec3 position;
 };

 uniform vec4 globalAmbient;
 uniform PositionalLight light;
 uniform mat4 m_matrix;
 uniform mat4 v_matrix;
 uniform mat4 p_matrix;
 uniform mat4 norm_matrix; // for transforming normals

 void main(void)
 { 
	 vec4 color;
	 // convert vertex position to world space,
	 // convert normal to world space, and
	 // calculate world space light vector (from vertex to light)
	 vec4 P = m_matrix * vec4(vertPos, 1.0);
	 vec3 N = normalize((norm_matrix * vec4(vertNormal,1.0)).xyz);
	 vec3 L = normalize(light.position - P.xyz);
	 // view vector is from vertex to camera, camera loc is extracted from view matrix
	 vec3 V = normalize(-v_matrix[3].xyz - P.xyz);
	 // R is reflection of -L with respect to surface normal N
	 vec3 R = reflect(-L,N);
	 // ambient, diffuse, and specular contributions
	 vec3 ambient = (globalAmbient + light.ambient).xyz;
	 vec3 diffuse = light.diffuse.xyz * max(dot(N,L), 0.0);
	 vec3 specular = light.specular.xyz * max(dot(R,V), 0.0f);
	 // send the color output to the fragment shader
	 varyingColor = vec4((ambient + diffuse + specular), 1.0);
	 // send the position to the fragment shader, as before
	 gl_Position = p_matrix * v_matrix * m_matrix * vec4(vertPos,1.0);

	 tc = texCoord;
 }