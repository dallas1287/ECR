#version 440

in vec3 posAttr;
in vec3 colAttr;
in vec3 texCoordAttr;

out vec3 color;
out vec2 TexCoords;

void main()
{
	gl_Position = vec4(posAttr, 1.0);
	color = colAttr;
	TexCoords = texCoordAttr.xy;
}