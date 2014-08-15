#version 330

in vec3 fPosition;

layout (location = 0) out vec4 outWorldPos;
layout (location = 1) out vec4 outDiffuse;
layout (location = 2) out vec4 outNormal;

in vec4 fNormal;

void main()
{
    outWorldPos = vec4(fPosition,1.0);
    outDiffuse  = vec4(1.0);
    outNormal   = fNormal;
} 
