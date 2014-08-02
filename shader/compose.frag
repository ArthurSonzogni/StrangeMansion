#version 330


uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

in vec2 gTexCoord;

uniform float blur = 1.0;

void main()
{
    float dx = blur/640;
    float dy = blur/480;
    vec4 color = texture(colorMap,gTexCoord);
    float blurLength = 0.005;
    float l = texture(positionMap,gTexCoord+vec2(+dx,+dy)).z;
         l += texture(positionMap,gTexCoord+vec2(-dx,+dy)).z;
         l += texture(positionMap,gTexCoord+vec2(-dx,-dy)).z;
         l += texture(positionMap,gTexCoord+vec2(+dx,-dy)).z;
         l += texture(positionMap,gTexCoord+vec2(+dx,  0)).z;
         l += texture(positionMap,gTexCoord+vec2(-dx,  0)).z;
         l += texture(positionMap,gTexCoord+vec2(  0,+dy)).z;
         l += texture(positionMap,gTexCoord+vec2(  0,-dy)).z;
         l*=1.0/8.0;
    float ll = texture(positionMap,gTexCoord).z;

    vec3 normal = texture(normalMap,gTexCoord).xyz;
    vec3 position = texture(positionMap,gTexCoord).xyz;

    float light = (0.3+0.7*dot(normal,vec3(0.0,1.0,0.0)));
    //light = int(light*10)/10.0;
    gl_FragColor = color * light;

    gl_FragColor.rgb += vec3((ll-l));
} 
