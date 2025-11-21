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

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;

    // Coordinate ricentrate in -1 _ 1
    vec2 centered = abs(uv * 2 - 1);
    
    // Border of the field
    float edgeNoise = noise(uv * 15.0) * 0.02;       // Rough rounded shapes
    float sawToothX = abs(sin(uv.x * 80.0)) * 0.005; // Tiny sawtooth pattern
    float sawToothY = abs(sin(uv.y * 80.0)) * 0.005; // Tiny sawtooth pattern
    
    // The distance from the center, modified by noise
    float dist = max(centered.x, centered.y) + edgeNoise + sawToothX + sawToothY;
    
    // Smooth Border at distance 0.9 from center
    float fieldBorder = smoothstep(0.855, 0.845, dist);
    
    // Field colors
    float hayGrain = random(uv * 60.0); 
    float hayVariation = noise(uv * 10.0);

    vec3 yellowHay = vec3(0.95, 0.875, 0.3); 
    vec3 orangeHay = vec3(0.95, 0.75, 0.2);
    vec3 dirt = vec3(0.35, 0.25, 0.1);

    vec3 hayBase = mix(yellowHay, orangeHay, hayVariation * 0.6 + hayGrain * 0.4);
    vec3 fieldColor = (hayGrain < 0.15) ? dirt : hayBase;

    // Grassland colors
    // Wind oscillation
    float windX = sin(uv.y * 5.0 + iTime * 2.0) * 0.02;
    float windY = cos(uv.x * 5.0 + iTime * 1.5) * 0.02;
    vec2 grassUV = uv + vec2(windX, windY);

    float grassGrain = random(grassUV * 250.0); 
    float grassPatches = noise(grassUV * 40.0); 

    vec3 grassGreen = vec3(0.1, 0.6, 0.1);
    vec3 darkGrass = vec3(0.05, 0.35, 0.05);
    vec3 bushColor = vec3(0.02, 0.25, 0.1);

    vec3 baseGrass = mix(darkGrass, grassGreen, grassGrain);
    
    vec3 grassAreaColor = (grassPatches < 0.15) ? bushColor : baseGrass;
    
    // Add movement to grass
    grassAreaColor += vec3(0.05) * sin(uv.x * 20.0 + iTime * 3.0);

    // Mix the two zones based on the rough noisy border mask
    vec3 finalColor = mix(grassAreaColor, fieldColor, fieldBorder);

    FragColor = vec4(finalColor, 1.0);
}