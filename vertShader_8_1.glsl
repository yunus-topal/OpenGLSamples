 #version 430

 layout (location=0) in vec3 vertPos;
 layout (location=1) in vec3 vertNormal;

  out vec3 varyingNormal; 
  out vec3 varyingLightDir; 
  out vec3 varyingVertPos; 
  out vec3 varyingHalfVector;
  out vec4 shadowCoord;

 struct PositionalLight
 { 
	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	vec3 position;
 };

 struct Material
 { 
	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	float shininess;
 };

 uniform vec4 globalAmbient;
 uniform PositionalLight light;
 uniform Material material;
 uniform mat4 m_matrix;
 uniform mat4 v_matrix;
 uniform mat4 p_matrix;
 uniform mat4 norm_matrix; // for transforming normals

 uniform mat4 shadowMVP;
 uniform mat4 shadowMVP2;
 layout (binding = 0) uniform sampler2DShadow shTex;

 void main(void)
 { 
	varyingVertPos=(m_matrix * vec4(vertPos,1.0)).xyz;
	varyingLightDir = light.position - varyingVertPos;
	varyingNormal=(norm_matrix * vec4(vertNormal,1.0)).xyz;
	varyingHalfVector = (varyingLightDir + (-varyingVertPos)).xyz;
	shadowCoord = shadowMVP2 * vec4(vertPos,1.0);
	 gl_Position = p_matrix * v_matrix * m_matrix * vec4(vertPos,1.0);
 }