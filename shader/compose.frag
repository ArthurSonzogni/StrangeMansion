#version 330


uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

in vec2 gTexCoord;

uniform float blur = 1.0;
uniform mat4 projection = mat4(1.0);
uniform vec3 lightDir = vec3(0.0,-1.0,0.0);

void main()
{
    vec3 normal = texture(normalMap,gTexCoord).xyz;
    vec3 position = texture(positionMap,gTexCoord).xyz;
    vec4 color = texture(colorMap,gTexCoord);

    //vec3 verticalTangent = normalize( cross(normal,vec3(1.0,0.0,0.0)));
    //vec3 horizontalTangent = cross( verticalTangent, normal);

    //float dx = blur/640;
    //float dy = blur/480;

    //float l=0.0;
    //float depth = texture(positionMap,gTexCoord).z;
    //float total = 0.0;
    //int r=1;
    //for(int x = -r; x<=r ; ++x)
    //for(int y = -r; y<=r ; ++y)
    //{
        //vec3 pos = position +
            //blur*(
                //normal + 
                //verticalTangent*float(x)/float(r) + 
                //horizontalTangent*blur*float(y)/float(r)
            //);
        //vec4 p = (projection * vec4(pos,1.0));
        //vec2 tPos = p.xy/p.w*0.5+vec2(0.5);
        //float t = texture(positionMap,tPos).z;
        //if (depth<t && depth>t-0.4)
            //l+=(depth-t);
    //}
    //l/=(5*5);

    float light = -dot(normal,lightDir);
    light = clamp(light,0.0,1.0) * 0.9 + 0.1;
    ////light = int(light*10)/10.0;

    //float lll = -l*1000.0;
    //lll = 100.0*lll/((1+lll*lll)*(1+lll*lll));
    //light += l;
    //light = clamp(light,0.0,1.0);
    gl_FragColor = color * light;
} 
