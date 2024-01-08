#version 460 core

out vec4 FragColor;

in vec3 FragPos;

//layout (depth_greater) out float gl_FragDepth;
//layout (depth_less) out float gl_FragDepth;

uniform mat4 View;
uniform vec2 CameraRange;
uniform vec3 CameraPos;
uniform vec3 SpherePos;
uniform vec2 WindowSize;
uniform vec3 Scale;
uniform float Zoom;

float total_distance_traveled = 0.0;
const int NUMBER_OF_STEPS = 64;
int i = 0;

float distance_from_sphere(in vec3 point, in vec3 center, float radius)
{
    return length(point - center ) - length(radius * Scale);
}

float map_the_world(in vec3 point)
{
    float sphere = float(distance_from_sphere(point, SpherePos, 1.0) );

    return sphere;
}

vec3 calculate_normal(in vec3 point)
{
    const vec3 small_step = vec3(0.001, 0.0, 0.0);

    float gradient_x = map_the_world(point + small_step.xyy) - map_the_world(point - small_step.xyy);
    float gradient_y = map_the_world(point + small_step.yxy) - map_the_world(point - small_step.yxy);
    float gradient_z = map_the_world(point + small_step.yyx) - map_the_world(point - small_step.yyx);

    vec3 normal = vec3(gradient_x, gradient_y, gradient_z);

    return normalize(normal);
}

vec4 ray_march(in vec3 rayOrigin, in vec3 rayDirection)
{
    const float MINIMUM_HIT_DISTANCE = 0.0001;

    for (i = 0; i < NUMBER_OF_STEPS; ++i)
    {
        vec3 current_position = rayOrigin + total_distance_traveled * rayDirection;

        float distance_to_closest = map_the_world(current_position);

        if (distance_to_closest < MINIMUM_HIT_DISTANCE)
        {
            vec3 normal = calculate_normal(current_position);
            vec3 light_position = SpherePos + vec3(2.0, -5.0, 3.0);
            vec3 direction_to_light = normalize(current_position - light_position);

            float diffuse_intensity = max(0.0, dot(normal, direction_to_light));
            
            return vec4(vec3(1, 0, 0) * diffuse_intensity, 1.0);
        }

        if (total_distance_traveled > CameraRange.y)
        {
            break;
        }
        total_distance_traveled += distance_to_closest;
    }
    
    total_distance_traveled = CameraRange.y;
}

void main()
{
    vec2 resolution = vec2(WindowSize);

    vec2 uv = (vec2(gl_FragCoord) - 0.5 * resolution);
    vec3 rayOrigin = CameraPos;

    vec3 rayDirectionBeforeRotation = vec3(uv.x, uv.y, (resolution.y * 0.5) / tan(radians(Zoom * 0.5)));
    vec3 rayDirection = normalize(vec3(View * vec4(rayDirectionBeforeRotation, 0.0)));
    
    vec4 color = ray_march(rayOrigin, rayDirection);
    
    vec3 dist = total_distance_traveled * normalize(rayDirectionBeforeRotation);
    
    float depth = (1.0 / dist.z - 1.0 / CameraRange.x) / (1.0 / CameraRange.y - 1.0 / CameraRange.x);

    gl_FragDepth = depth;
    
    FragColor = color;
        
}