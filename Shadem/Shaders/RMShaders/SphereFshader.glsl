#version 460 core

out vec4 FragColor;

in vec3 FragPos;

uniform mat4 Model;
uniform mat4 View;
uniform vec3 CameraPos;
uniform vec3 SpherePos;
uniform float Zoom;

float distance_from_sphere(in vec3 p, in vec3 c, float r)
{
	return length(p - c) - r;
}

float map_the_world(in vec3 p)
{
    float sphere = float(distance_from_sphere(p, SpherePos, 1.0) );

    return sphere;
}

vec3 calculate_normal(in vec3 p)
{
    const vec3 small_step = vec3(0.001, 0.0, 0.0);

    float gradient_x = map_the_world(p + small_step.xyy) - map_the_world(p - small_step.xyy);
    float gradient_y = map_the_world(p + small_step.yxy) - map_the_world(p - small_step.yxy);
    float gradient_z = map_the_world(p + small_step.yyx) - map_the_world(p - small_step.yyx);

    vec3 normal = vec3(gradient_x, gradient_y, gradient_z);

    return normalize(normal);
}

vec4 ray_march(in vec3 ro, in vec3 rd)
{
    float total_distance_traveled = 0.0;
    const int NUMBER_OF_STEPS = 32;
    const float MINIMUM_HIT_DISTANCE = 0.001;
    const float MAXIMUM_TRACE_DISTANCE = 250.0;

    for (int i = 0; i < NUMBER_OF_STEPS; ++i)
    {
        vec3 current_position = ro + total_distance_traveled * rd;

        float distance_to_closest = map_the_world(current_position);

        if (distance_to_closest < MINIMUM_HIT_DISTANCE)
        {
            vec3 normal = calculate_normal(current_position);
            vec3 light_position = SpherePos + vec3(2.0, -5.0, 3.0);
            vec3 direction_to_light = normalize(current_position - light_position);

            float diffuse_intensity = max(0.0, dot(normal, direction_to_light));

            return vec4(vec3(1,0,0) * diffuse_intensity, 1.0);
        }

        if (total_distance_traveled > MAXIMUM_TRACE_DISTANCE)
        {
            break;
        }
        total_distance_traveled += distance_to_closest;
    }
    return vec4(0.0);
}

void main()
{
    vec2 res = vec2(1920.0f, 1080.0f);

    vec2 uv = (vec2(gl_FragCoord) - 0.5*res);
    vec3 ro = CameraPos;

    vec3 rdBR = vec3(uv.x, uv.y,  (res.y*0.5) / tan(radians(Zoom * 0.5)) );          // works?
    vec3 rd = normalize(vec3((View) * vec4(rdBR, 0.0) ));
 
	FragColor = ray_march(ro, rd);
}