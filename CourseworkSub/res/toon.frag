#version 330

in vec2 texUV;
in float intensity;
uniform sampler2D diffuse;
out vec4 color;
void main()
{



	if (intensity > 0.95)
		color = vec4(1.0,1.0,1.0,1.0);
	else if (intensity > 0.5)
		color = vec4(0.75,0.75,0.75,1.0);
	else if (intensity > 0.25)
		color = vec4(0.5,0.5,0.5,1.0);
	else
		color = vec4(0.25,0.25,0.25,1.0);
		
	 color *= texture2D(diffuse, texUV);
}
