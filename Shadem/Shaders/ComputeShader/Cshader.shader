#version 460 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;

layout(std430, binding = 0) buffer VertexBuffer {
    vec3 vertices[]; // Dane wierzcho³ków
};

void main() {
    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    value.x = float(texelCoord.x) / (gl_NumWorkGroups.x);
    value.y = float(texelCoord.y) / (gl_NumWorkGroups.y);

    imageStore(imgOutput, texelCoord, value);
}

//#version 460 core
//
//layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
//
//layout(std430, binding = 1) buffer GridValues {
//    float values[];
//};
//
//uniform float isoValue;
//uniform float frequency;
//uniform float amplitude;
//uniform float persistence;
//uniform float lacunarity;
//uniform float noiseScale;
//uniform vec3 offset;
//
//uniform vec3 gridSize;
//
//#pssssragma include "../Shadem/Shaders/CMShaders/Noise.shader"
//
//void main() {
//    uint index = gl_GlobalInvocationID.x;
//    if (index >= values.length()) return;
//
//    float x = index / (gridSize.z * gridSize.y);
//    float y = (index % int(gridSize.z * gridSize.y)) / gridSize.z;
//    float z = index % int(gridSize.z);
//    x = x / gridSize.x - 0.5f;
//    y = y / gridSize.y - 0.5f;
//    z = z / gridSize.z - 0.5f;
//    //x = int((x + offset.x * noiseScale));
//    //y = int((y + offset.y * noiseScale));
//    //z = int((z + offset.z * noiseScale));
//
//    values[index] = cnoise(vec3(x, y, z), frequency, amplitude, persistence, lacunarity);
//}
