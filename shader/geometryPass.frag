#version 330

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

layout (location = 0) out vec4 outWorldPos;
layout (location = 1) out vec4 outDiffuse;
layout (location = 2) out vec4 outNormal;

uniform sampler2D texture0;

void main()
{
    outWorldPos = vec4(fPosition,1.0);
    //outDiffuse  = vec4(1.0,1.0,1.0,1.0);
    outDiffuse  = texture(texture0,fTexCoord) + 0.3*vec4(1.0,1.0,1.0,1.0);
    outNormal = vec4(normalize(fNormal),1.0);

    //outDiffuse = outDiffuse*(0.3+0.7*outNormal.g);
} 
