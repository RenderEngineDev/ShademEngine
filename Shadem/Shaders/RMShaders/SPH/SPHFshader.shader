#version 460 core

out vec4 FragColor;

in vec3 FragPos;

#pragma include "../Shadem/Shaders/RMShaders/BasicUniforms.shader"

uniform vec3 Scale;
uniform vec3 SPHPos;
uniform float ballScale;

uniform int N;
layout(binding = 0) buffer fparticle { vec4 p []; };
layout(binding = 12) buffer fprojected_particles { vec4 projectedP []; };

float total_distance_traveled = 0.0;
const int NUMBER_OF_STEPS = 128;
const float MINIMUM_HIT_DISTANCE = 0.01;

int validSpheres[256];
int validSpheresQuantity = 0;

float sMin(float a, float b, float k) {
  float h = clamp(0.5 + 0.5 * (a - b) / k, 0.0, 1.0);
  return mix(a, b, h) - k * h * (1.0 - h);
}

float distance_from_sphere(in vec3 point, in vec3 center, float radius)
{
    return length(point - (center + SPHPos) ) - length(radius * Scale * ballScale);
}

float map_the_world(in vec3 point)
{
    if(validSpheresQuantity < 1)
        return distance_from_sphere(point, vec3(p[0] - 0.5)*Scale, 1.0f);

    float sphere = distance_from_sphere(point, vec3(p[validSpheres[0]] - 0.5)*Scale, 1.0f);
    float tmp = 0.0f;
    for(int i=1; i<validSpheresQuantity; i++)
    {
        tmp = distance_from_sphere(point, vec3(p[validSpheres[i]] - 0.5)*Scale, 1.0f);
        //if(sphere > tmp)
        //{
        //    sphere = tmp;
        //}
        sphere = sMin(sphere, tmp, 0.1f);
    }

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
    vec3 current_position;

    for(int i=0; i<N; ++i)
    {
        if( distance(vec2(projectedP[i]), uv/(WindowSize*0.5)) < projectedP[i].w)
        {
            validSpheres[validSpheresQuantity++] = i;
        }         
    }


    if(sampleDepth)
    {
        total_distance_traveled = CameraRange.y * texture(depthSampler, vec2(TexCoords.x, 1-TexCoords.y)).x; 
        return texture(colorSampler, vec2(TexCoords.x, 1-TexCoords.y));
    }

    for (int i = 0; i < NUMBER_OF_STEPS; ++i)
    {
        current_position = rayOrigin + total_distance_traveled * rayDirection;

        float distance_to_closest = map_the_world(current_position);

        if (distance_to_closest < MINIMUM_HIT_DISTANCE)
        {
            vec3 normal = calculate_normal(current_position);
            vec3 light_position = SPHPos + vec3(2.0, -5.0, 3.0);
            vec3 direction_to_light = normalize(current_position - light_position);

            float diffuse_intensity = max(0.0, dot(normal, direction_to_light));
            
            return vec4(vec3(1, 1, 1) * diffuse_intensity, 1.0);
        }

        if (total_distance_traveled > CameraRange.y )
        {
            total_distance_traveled = 2*CameraRange.y;
            return vec4(0);
        }
        total_distance_traveled += distance_to_closest;
    }

    vec3 normal = calculate_normal(current_position);
    vec3 light_position = SPHPos + vec3(2.0, -5.0, 3.0);
    vec3 direction_to_light = normalize(current_position - light_position);

    float diffuse_intensity = max(0.0, dot(normal, direction_to_light));
            
    return vec4(vec3(1, 1, 1) * diffuse_intensity, 1.0);
}

#pragma include "../Shadem/Shaders/RMShaders/Main.shader"