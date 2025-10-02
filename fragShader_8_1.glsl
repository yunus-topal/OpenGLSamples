 #version 430

 in vec3 varyingNormal;
 in vec3 varyingLightDir;
 in vec3 varyingVertPos;
 in vec3 varyingHalfVector;
 in vec4 shadowCoord;
 out vec4 fragColor;

 // uniforms match those in the vertex shader,
 // but are not used directly in this fragment shader

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
 uniform mat4 norm_matrix;

 uniform mat4 shadowMVP;
 uniform mat4 shadowMVP2;
 layout (binding = 0) uniform sampler2DShadow shTex;

 void main(void)
 { 
	 vec3 L = normalize(varyingLightDir);
	 vec3 N = normalize(varyingNormal);
	 vec3 V = normalize(-v_matrix[3].xyz - varyingVertPos);
	 vec3 H = normalize(varyingHalfVector);

	 float inShadow = textureProj(shTex, shadowCoord);

	 // compute light reflection vector with respect to N:
	 vec3 R = normalize(reflect(-L, N));
	 // get the angle between the light and surface normal:
	 float cosTheta = dot(L,N);
	 // angle between the view vector and reflected light:
	 float cosPhi = dot(H,N);


	 // compute ADS contributions (per pixel), and combine to build output color:

	 // Ambient is always active, others are modulated by shadow term
	 fragColor = ((globalAmbient * material.ambient) + (light.ambient * material.ambient));

	 if (inShadow == 1.0){
	 	 vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0);
		 vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess * 3.0);
		fragColor += vec4((diffuse + specular), 1.0);
	 }
 }