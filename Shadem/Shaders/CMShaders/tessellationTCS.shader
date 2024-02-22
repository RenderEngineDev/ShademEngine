#version 460 core

layout (vertices = 3) out;

//in vec2 uvs[];
//out vec2 uvsCoord[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    //uvsCoord[gl_InvocationID] = uvs[gl_InvocationID];

    //gl_TessLevelOuter[0] = 2; // left for triangles
    //gl_TessLevelOuter[1] = 2; // bot for triangles
    //gl_TessLevelOuter[2] = 2; // right for triangles
        
    //gl_TessLevelInner[0] = 4; // all inner sides for triangles

    if (gl_InvocationID == 0) {
        const int MIN_TESS_LEVEL = 1;
        const int MAX_TESS_LEVEL = 32;
        const float MIN_DISTANCE = 100;
        const float MAX_DISTANCE = 500;

        vec4 eyeSpacePos00 = view * model * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = view * model * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = view * model * gl_in[2].gl_Position;

        // "distance" from camera scaled between 0 and 1
        float distance00 = clamp((abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance01 = clamp((abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance10 = clamp((abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01));
        float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance10));
        float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00));

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;

        gl_TessLevelInner[0] = max(tessLevel0, tessLevel2);
    }
}