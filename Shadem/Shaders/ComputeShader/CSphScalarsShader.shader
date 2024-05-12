#version 440

layout(local_size_x = 128) in;

layout(std430, binding = 0) buffer scalarFunctionBuffer {
    float scalarValues[];
};

void main() {
    uint index = gl_GlobalInvocationID.x;
    if (index >= scalarValues.length()) return;

    scalarValues[index] = 1.0f;
}