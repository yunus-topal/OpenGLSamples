#version 430

 layout (location = 0) in vec3 position;

 uniform mat4 v_matrix;
 uniform mat4 p_matrix;
 uniform float tf;

  out vec4 varyingColor;

  mat4 buildTranslate(float x, float y, float z);
  mat4 buildRotateX(float rad); 
  mat4 buildRotateY(float rad); 
  mat4 buildRotateZ(float rad);

 void main(void)
 { 
	float i = gl_InstanceID + tf; 
	float a = sin(2.0 * i) * 8.0; 
	float b = sin(3.0 * i) * 8.0;
	float c = sin(4.0 * i) * 8.0;
	
	mat4 localRotX = buildRotateX(10*i);
	mat4 localRotY = buildRotateY(10*i);
	mat4 localRotZ = buildRotateZ(10*i);
	mat4 localTrans = buildTranslate(a,b,c);

	 //  build the model matrix and then the model-view matrix
	 mat4 newM_matrix = localTrans * localRotX * localRotY * localRotZ;
	 mat4 mv_matrix = v_matrix * newM_matrix;

    vec4 initialPos = vec4(position, 1.0);
	gl_Position = p_matrix * mv_matrix * initialPos;

	varyingColor = initialPos * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
 }
     mat4 buildTranslate(float x, float y, float z)
    {
        mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            x, y, z, 1.0);
        return trans;
    }
    // builds and returns a matrix that performs a rotation around the X axis
    mat4 buildRotateX(float rad)
    {
        mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
            0.0, cos(rad), -sin(rad), 0.0,
            0.0, sin(rad), cos(rad), 0.0,
            0.0, 0.0, 0.0, 1.0);
        return xrot;
    }
    // builds and returns a matrix that performs a rotation around the Y axis
    mat4 buildRotateY(float rad)
    {
        mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
            0.0, 1.0, 0.0, 0.0,
            -sin(rad), 0.0, cos(rad), 0.0,
            0.0, 0.0, 0.0, 1.0);
        return yrot;
    }
    // builds and returns a matrix that performs a rotation around the Z axis
    mat4 buildRotateZ(float rad)
    {
        mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
            sin(rad), cos(rad), 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);
        return zrot;
    }