#version 330 core		// OpenGL version 3.3
layout(location = 0) in vec3 aPos;	// aPos is a variable whose type is vec3
// layout(location = 1) in vec3 aColor;	
layout(location = 1) in vec2 aTexCoord;	

out vec3 ourColor;
out vec2 TexCoord;

// uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// vec.x, vec.y, vec.z, vec.w (vec.w is perspective division)
	// gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//	gl_Position = vec4(aPos, 1.0f);	// vectors merge

	gl_Position = projection * view * model * vec4(aPos, 1.0f);

//	ourColor = aColor;	// set the output variable to black red
	TexCoord = aTexCoord;
}