#version 460 core

#define MAX_TRIANGLE_AMOUNT 12

layout(points) in;
layout(triangle_strip, max_vertices = MAX_TRIANGLE_AMOUNT) out;


in vec3[] FragPos;
in vec2[] TextCoords;

uniform float isoValue;
uniform float frequency;
uniform float amplitude;
uniform float persistence;
uniform float lacunarity;
uniform float noiseScale;
uniform vec3 offset;

uniform vec3 gridSize;

uniform mat4 model;
uniform mat4 MVP;

out fData
{
    vec3 position;
    vec3 normal;
    vec4 color;
}frag;

#pragma include "../Shadem/Shaders/CMShaders/CubeMarchingTables.shader"
#pragma include "../Shadem/Shaders/CMShaders/Noise.shader"
#pragma include "../Shadem/Shaders/CMShaders/CubeMarching.shader"

void main() {

    GridCell cell = calculateCellValuesForNoise3D(offset, noiseScale, frequency, amplitude, persistence, lacunarity);
    int cubeIndex = calculateCubeIndex(cell);
     
    if ((cubeIndex != 0) && (cubeIndex != 255)) {
        const int[16] edgeIndex = triangleTable[cubeIndex];
        vec3 v1, v2, v3;
        vec3[MAX_TRIANGLE_AMOUNT] intersections;
#pragma FOR_LOOP(4) 
        intersections = getIntersectionCoordinates(cell, cubeIndex);
        if (edgeIndex[i * 3] == -1) return;
        v1 = intersections[edgeIndex[i * 3 + 0]];
        v2 = intersections[edgeIndex[i * 3 + 1]];
        v3 = intersections[edgeIndex[i * 3 + 2]];

        frag.normal = abs(normalize(cross(v1 - v2, v3 - v2)));

        gl_Position = MVP * vec4(v1, 1.0f);
        frag.position = (model * vec4(v1, 1.0f)).xyz;
        frag.color = model * vec4(v1, 1.0f);
        EmitVertex();
        gl_Position = MVP * vec4(v2, 1.0f);
        frag.position = (model * vec4(v2, 1.0f)).xyz;
        frag.color = model * vec4(v2, 1.0f);
        EmitVertex();
        gl_Position = MVP * vec4(v3, 1.0f);
        frag.position = (model * vec4(v3, 1.0f)).xyz;
        frag.color = model * vec4(v3, 1.0f);
        EmitVertex();
        EndPrimitive();
#pragma END_FOR_LOOP
    }
    /*
    if ((cubeIndex != 0) && (cubeIndex != 255)) {
        vec3[MAX_TRIANGLE_AMOUNT] intersections = getIntersectionCoordinates(cell, cubeIndex);
        for (int i = 0; triangleTable[cubeIndex][i] != -1; i += 3) {
            vec3 v1 = intersections[triangleTable[cubeIndex][i + 0]];
            vec3 v2 = intersections[triangleTable[cubeIndex][i + 1]];
            vec3 v3 = intersections[triangleTable[cubeIndex][i + 2]];

            frag.normal = abs(normalize(cross(v1 - v2, v3 - v2)));

            gl_Position = MVP * vec4(v1, 1.0f);
            frag.position = (model * vec4(v1, 1.0f)).xyz;
            frag.color = model * vec4(v1, 1.0f);
            EmitVertex();
            gl_Position = MVP * vec4(v2, 1.0f);
            frag.position = (model * vec4(v2, 1.0f)).xyz;
            frag.color = model * vec4(v2, 1.0f);
            EmitVertex();
            gl_Position = MVP * vec4(v3, 1.0f);
            frag.position = (model * vec4(v3, 1.0f)).xyz;
            frag.color = model * vec4(v3, 1.0f);
            EmitVertex();
            EndPrimitive();
        }
    }
    */
}
