#version 330
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 transform;
uniform vec3 lightDir;

out float intensity;
out vec2 texUV;

void main()
{
	intensity = dot(lightDir, normal);
	texUV = texCoord;
	gl_Position = transform*vec4(position, 1.0);
}
