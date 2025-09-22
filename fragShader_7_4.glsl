 #version 430
 layout (binding=0) uniform sampler2D samp;
 in vec2 tc;
 in vec4 varyingColor;
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

 uniform vec4 globalAmbient;
 uniform PositionalLight light;
 uniform mat4 m_matrix;
 uniform mat4 v_matrix;
 uniform mat4 p_matrix;
 uniform mat4 norm_matrix;

 void main(void)
 { 
	fragColor = varyingColor * texture(samp,tc);
 }