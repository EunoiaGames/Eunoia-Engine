#vertex
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

out vec2 texCoord0;

void main()
{
	gl_Position = vec4(pos, 1.0);
	texCoord0 = texCoord;
}

#fragment
#version 330

in vec2 texCoord0;
out vec4 outColor;

uniform sampler2D Albedo;

void main()
{
	outColor = texture(Albedo, texCoord0);
}
