#version 440

layout(local_size_x = 128) in;

uniform float h;   // smoothing length
uniform float particleRadius;   // smoothing length
uniform vec3 gridSize;

layout(std430, binding = 0) buffer fparticle { vec4 p[]; };
layout(std430, binding = 1) buffer scalarFunctionBuffer {
    float scalarValues[];
};

bool isWithinSphere(float px, float py, float pz, int x, int y, int z, float radius) {
	float dx = px - float(x);
	float dy = py - float(y);
	float dz = pz - float(z);
	return (dx * dx + dy * dy + dz * dz) <= (radius * radius);
}

void main() {
    uint index = gl_GlobalInvocationID.x;
    //if (index >= scalarValues.length()) return;
    //scalarValues[index] = 1.0f;
	
	vec3 posNew = vec3(p[index]) * gridSize;
	//float r = h * int((gridSize.x + gridSize.y + gridSize.z) / 3);
	float r = particleRadius * int((gridSize.x + gridSize.y + gridSize.z) / 3);
	int startX = int(floor(posNew.x - r));
	int endX = int(ceil(posNew.x + r));
	int startY = int(floor(posNew.y - r));
	int endY = int(ceil(posNew.y + r));
	int startZ = int(floor(posNew.z - r));
	int endZ = int(ceil(posNew.z + r));

	for (int x = startX; x <= endX; ++x) {
		if (x < 0 || x >= int(gridSize.x)) continue;
		for (int y = startY; y <= endY; ++y) {
			if (y < 0 || y >= int(gridSize.y)) continue;
			for (int z = startZ; z <= endZ; ++z) {
				if (z < 0 || z >= int(gridSize.z)) continue;
				if (!((x == 0) || (y == 0) || (z == 0) || (x == (int(gridSize.x) - 1)) || (y == (int(gridSize.y) - 1)) || (z == (int(gridSize.z - 1)))))
				if (isWithinSphere(posNew.x, posNew.y, posNew.z, x, y, z, r)) {
					int flatIndex = x + y * int(gridSize.x) + z * int(gridSize.x) * int(gridSize.y);
					scalarValues[flatIndex] = -1.0f;
				}
			}
		}
	}
}