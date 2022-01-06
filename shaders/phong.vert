// Based on lab3b
#version 150

in  vec3 inPosition;
in  vec3 inNormal;

in vec3 inColor;
in vec2 inTexCoord; // Currently not used

out vec3 exNormal; // Phong
out vec3 exSurface; // Phong (specular)
out vec2 exTexCoord;

out vec3 color;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

// instancing translation
in vec3 inTranslation;

// instancing rotation angle
in float inAngle;

void main(void)
{

	// Rotate around y
	mat4 r;
	r[0] = vec4(cos(inAngle), 0, sin(inAngle), 0);
	r[1] = vec4(0, 1, 0, 0);
	r[2] = vec4(-sin(inAngle), 0, cos(inAngle), 0);
	r[3] = vec4(0, 0, 0, 1);
	//vec4 originPos = vec4(inPosition, 1.0);

	vec4 pos = r * vec4(inPosition, 1.0);
	pos.x += inTranslation.x;
	pos.y += inTranslation.y;
	pos.z += inTranslation.z;

	exNormal = inverse(transpose(mat3(modelviewMatrix))) * inNormal; // Phong, normal transformation
	exSurface = vec3(modelviewMatrix * pos); // Don't include projection here - we only want to go to view coordinates
	exTexCoord = inTexCoord;

	color = inColor;

	//pos += vec4(inTranslation, 1.0);
	//vec4 pos = vec4(inTranslation + inPosition, 1.0) * r;
	gl_Position = projectionMatrix * modelviewMatrix * pos; // This should include projection
}
