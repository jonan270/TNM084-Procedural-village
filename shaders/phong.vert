#version 150

in  vec3 inPosition;
in  vec3 inNormal;

in vec3 inColor;
in vec3 inTranslation;

in vec2 inTexCoord; // Currently not used


out vec3 exNormal; // Phong
out vec3 exSurface; // Phong (specular)
out vec2 exTexCoord;

out vec3 color;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	exNormal = inverse(transpose(mat3(modelviewMatrix))) * inNormal; // Phong, normal transformation

	exSurface = vec3(modelviewMatrix * vec4(inPosition, 1.0)); // Don't include projection here - we only want to go to view coordinates

	exTexCoord = inTexCoord;

	color = inColor;
	//vec4 pos = vec4(inTranslation + inPosition, 1.0);
	vec4 pos = vec4(inPosition, 1.0);
	if(gl_InstanceID != 0) {
		pos.y += 5;
		color = inTranslation;
	}
	gl_Position = projectionMatrix * modelviewMatrix * pos; // This should include projection
}
