#version 330 core		// OpenGL version 3.3
out vec4 FragColor;		// output variable named FragColor

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform float visible;
uniform int type;

void main()
{
	// vec.x, vec.y, vec.z, vec.w (vec.w is perspective division)
	// FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
//	FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0f);
	if (type == 0)
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1 - TexCoord.x, TexCoord.y)), visible);
	else
		FragColor = texture(texture3, TexCoord);
}