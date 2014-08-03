#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection = mat4(1);
uniform mat4 model = mat4(1);
uniform mat4 view = mat4(1);

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;

void main()
{
    vec4 worldPos = view * model * vec4(position,1.0);

    fPosition = worldPos.xyz;
    fTexCoord = texCoord;
    fNormal   = ( view * model * vec4(normal,0.0)).xyz;

    gl_Position = projection * worldPos;
}
