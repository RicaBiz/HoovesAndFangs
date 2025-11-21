#version 330 core

out vec4 FragColor;

uniform vec2 iResolution;
uniform float iTime;

// Funzioni per il random

// pseudo random function - source: https://thebookofshaders.com/11/
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Noise - source: https://thebookofshaders.com/11/
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(mix(random(i + vec2(0.0, 0.0)), 
                   random(i + vec2(1.0, 0.0)), u.x),
               mix(random(i + vec2(0.0, 1.0)), 
                   random(i + vec2(1.0, 1.0)), u.x), u.y);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec2 texUV = gl_FragCoord.xy / iResolution.y;


    // Wind oscillation
    float windX = sin(texUV.y * 5.0 + iTime * 2.0) * 0.02;
    float windY = cos(texUV.x * 5.0 + iTime * 1.5) * 0.02;
    vec2 grassUV = texUV + vec2(windX, windY);

    float grassGrain = random(grassUV * 250.0); 
    float grassPatches = noise(grassUV * 40.0); 

    vec3 grassGreen = vec3(0.1, 0.6, 0.1);
    vec3 darkGrass = vec3(0.05, 0.35, 0.05);
    vec3 bushColor = vec3(0.02, 0.25, 0.1);

    vec3 baseGrass = mix(darkGrass, grassGreen, grassGrain);
    
    vec3 grassAreaColor = (grassPatches < 0.15) ? bushColor : baseGrass;
    
    // Add subtle movement highlight to grass
    grassAreaColor += vec3(0.05) * sin(uv.x * 20.0 + iTime * 3.0);

    FragColor = vec4(grassAreaColor, 1.0);
}