 #version 430

 layout (binding=0) uniform sampler2D samp;

 out vec4 color;
 in vec2 tc;

 uniform bool useTex;


 void main(void)
 { 
	color = useTex ? texture(samp,tc) : vec4(1.0, 1.0, 0.0, 1.0);
 }