#version 330 core

in vec2 tCoord;
in vec3 fragpos;
in vec3 normal;

const vec3 skycolor = vec3(0.86f, 0.82f, 0.78f);
// const vec3 lightPos = vec3(0.0f, 20.0f, 0.0f);
const float maxDist = 10.0f;
uniform vec3 sunDirection = normalize(vec3(0.0f, 1.0f, -2.0f));

uniform sampler2D texture_diffuse1;


out vec4 fragcol;

vec3 applyFog(vec3 originalColor, float distance) {
    // float fogDensity = 0.00001; // Adjust this value to increase or decrease fog intensity
    // float fogFactor = exp(-fogDensity * distance);
    
    // vec3 fogColor = skycolor; // The color of the fog
    // return mix(originalColor, fogColor, 1.0 - fogFactor);

    float fogAmt = 1.0 - exp(-distance*.004);
    vec3 fogColor = vec3(0.529,0.708,0.922);//vec3(0.5, 0.6, 0.7);
    return mix(originalColor, fogColor, fogAmt);
}

in float dist;
in vec3 col;

const float minHeight = 0.0;
// const float maxHeight = 65.0;
uniform float maxHeight = 65.0;
const vec3 snowColor  = vec3(0.9, 0.9, 0.95);  // Light bluish-white
const vec3 grassColor = vec3(0.1, 0.6, 0.1);   // Vibrant green
const vec3 rockColor  = vec3(0.4, 0.3, 0.2);   // Earthy brown-gray


void main(){

    /*
    IMPL 1
    */
    // vec3 lightPos = fragpos + sunDirection;
    // vec3 vecToLight = normalize(lightPos - fragpos);
    // //vec3(0.7f, 0.9f, 0.8f)
    // //vec3(0.58823, 0.43529, 0.2)
    // // vec3 tCol = vec3(0.60784, 0.43529, 0.31372)*max(dot(sunDirection, normal), 0.0f) + 
    // //             vec3(0.1f);
    // vec3 co = texture(texture_diffuse1, tCoord).rgb;
    // vec3 tCol = co*max(dot(sunDirection, normal), 0.0f) + co*0.3;
    // float d = min(dist, maxDist);
    // float alpha = d/maxDist;
    // //vec3 tempcol = (1-alpha)*tCol + (alpha)*skycolor;
    // vec3 tempcol = applyFog(tCol, d);

    // fragcol = vec4(tempcol, 1.0f);
    // fragcol = vec4(col, 1.0f);

    /* IMPL 2.0*/
    float s = smoothstep(0.5, 0.9, dot(normal, vec3(.3, 1, 0.05)));
    vec3 color = mix(vec3(0.1), vec3(0.7, 0.72, 0.7), smoothstep(.1, .7, s));

    float shadow = .5 + clamp(-8. + 16.*dot(sunDirection, normal), 0.0, .5);
    shadow *= smoothstep(20.0, 80.0, fragpos.y);
    float amb = max(0.5+0.5*normal.y, 0.0);
    float diff = max(dot(sunDirection, normal), 0.0);
    float backlight = max(0.5 + 0.5*dot( normalize( vec3(sunDirection.x, 0., sunDirection.z)), normal), 0.0);

    vec3 lin = (diff*shadow*3.) * vec3(1.0);
    lin += amb*vec3(0.4, 0.6, 1.0);
    lin += backlight*vec3(0.4, .5, .6);
    color *= lin;
    color *= (.6 + .4*smoothstep(400., 100., abs(fragpos.z)));
    color = applyFog(color, dist);
    fragcol =  vec4(color, 1.0f);

    /* IMPL 3.0 */
    // float height = fragpos.y; // Altitude
    // // float noise = getNoise(FragPos); // Randomization
    // float heightFactor = smoothstep(minHeight, maxHeight, height); 

    // // Slope detection: using normal's y component
    // float slopeFactor = clamp(normal.y, 0.0, 1.0);
    
    // // Snow blending
    // float snowBlend = smoothstep(0.6, 1.0, heightFactor) * slopeFactor;
    
    // // Grass blending
    // float grassBlend = smoothstep(0.3, 0.6, heightFactor) * slopeFactor;
    
    // // Rock blending (default if neither snow nor grass)
    // float rockBlend = 1.0 - (snowBlend + grassBlend);

    // vec3 finalColor = snowBlend * snowColor + 
    //                   grassBlend * grassColor + 
    //                   rockBlend * rockColor;

    // fragcol = vec4(vec3(dot(normal, vec3(0.0, 1.0, 0.0))), 1.0);


    // fragcol = vec4(col, 1.0f);
}
