#version 460 core

layout (triangles, equal_spacing , ccw) in;

//in vec2 uvsCoord[];

bool notEqualsV3(vec4 v1, vec4 v2) {
    return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
}

void main()
{
    // barycentric coordinates
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;
    // barycentric interpolation
    //vec2 texCoord = u * uvsCoord[0] + v * uvsCoord[1] + w * uvsCoord[2];

    vec4 pos0 = gl_in[0].gl_Position;
    vec4 pos1 = gl_in[1].gl_Position;
    vec4 pos2 = gl_in[2].gl_Position;

    // calculate the normal
    vec3 normal = normalize(cross(vec3(pos1 - pos0), vec3(pos2 - pos0)));

    // barycentric interpolation
    vec4 pos = u * pos0 + v * pos1 + w * pos2;

    // displacement along the normal
    float displacement = 0.2; // adjust this value as needed

    // calculate the distance to the original vertices
    float dist0 = distance(pos.xyz, pos0.xyz);
    float dist1 = distance(pos.xyz, pos1.xyz);
    float dist2 = distance(pos.xyz, pos2.xyz);

    // find the minimum distance
    float minDist = min(min(dist0, dist1), dist2);

    // scale the displacement based on the minimum distance
    float scale = normalize(abs(1.0 - minDist)); // this will be in the range [0, 1]

    // apply the displacement
    if (notEqualsV3(pos, pos0) && notEqualsV3(pos, pos1) && notEqualsV3(pos, pos2))
        pos.xyz += normal * displacement * scale;

    gl_Position = pos;
    //uvs = texCoord;
}