#ifndef MAIN_SHADER
#define MAIN_SHADER

void main()
{
    uv = (vec2(gl_FragCoord) - 0.5 * WindowSize);

    rayDirectionBeforeRotation = vec3(uv.x, uv.y, (WindowSize.y * 0.5) / tan(radians(Zoom * 0.5)));
    vec3 rayDirection = normalize(vec3(View * vec4(rayDirectionBeforeRotation, 0.0)));
    rayDirectionBeforeRotation = normalize(rayDirectionBeforeRotation);
    
    vec4 color = ray_march(CameraPos, rayDirection);
    
    vec3 dist = total_distance_traveled * rayDirectionBeforeRotation;
    
    if(!sampleDepth)
    {
        gl_FragDepth = calculate_depth(CameraRange.x, CameraRange.y, dist.z);
        FragColor = color;
    }
    else
    {
        gl_FragDepth = calculate_linear_depth(dist.z);
        FragColor = color;
    }
}

#endif