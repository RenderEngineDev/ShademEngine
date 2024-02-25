#version 460 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer VertexBuffer {
    vec3 vertices[]; // Dane wierzcho³ków
};

void main() {
    // Przypisz wspó³rzêdne trójk¹ta
    vertices[0] = vec3(10.0, 10.0, 10.0);
    vertices[1] = vec3(11.0, 10.0, 10.0);
    vertices[2] = vec3(10.5, 11.0, 10.0);
}
