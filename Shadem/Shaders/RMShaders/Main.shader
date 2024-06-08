#ifndef MAIN_SHADER
#define MAIN_SHADER

void main()
{
    vec2 uv = (vec2(gl_FragCoord) - 0.5 * vec2(WindowSize));

    vec3 rayDirectionBeforeRotation = vec3(uv.x, uv.y, (WindowSize.y * 0.5) / tan(radians(Zoom * 0.5)));
    vec3 rayDirection = normalize(vec3(View * vec4(rayDirectionBeforeRotation, 0.0)));
    
    vec4 color = ray_march(CameraPos, rayDirection);
    
    vec3 dist = total_distance_traveled * normalize(rayDirectionBeforeRotation);
    
    float depth = (1.0 / dist.z - 1.0 / CameraRange.x) / (1.0 / CameraRange.y - 1.0 / CameraRange.x);

    gl_FragDepth = depth;
    
    FragColor = color;
}

#endif