#version 330\

in vec3 theColor;
out vec4 outColor;

void main()
{
	outColor = vec4(theColor, 1.0);
}