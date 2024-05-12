#version 430

layout(local_size_x = 128) in;

struct Particle {
    vec3 position;
    vec3 velocity;
    vec3 acceleration;
    vec3 force;
    float density;
    float pressure;
};

layout(std430, binding = 1) buffer scalarFunctionBuffer {
    float scalarValues[];
};
layout(std430, binding = 2) buffer Particles {
    Particle particles[];
};

uniform float deltaTime;
uniform float restDensity;
uniform float gasConstant;
uniform float viscosity;
uniform float kernelRadius;
uniform float mass;
uniform vec3 minBoundary;
uniform vec3 maxBoundary;
uniform vec3 gridSize;

const float maxForce = 100.0;
const float maxVelocity = 25.0;

float poly6Kernel(float r, float h) {
    if (r >= 0.0 && r <= h) {
        float factor = 315.0 / (64.0 * 3.141592 * pow(h, 9));
        return factor * pow(h * h - r * r, 3);
    }
    return 0.0;
}

vec3 spikyGradientKernel(vec3 rVec, float r, float h) {
    if (r > 0.0 && r <= h) {
        float factor = -45.0 / (3.141592 * pow(h, 6));
        return factor * pow(h - r, 2) * normalize(rVec);
    }
    return vec3(0.0);
}

float viscosityLaplacianKernel(float r, float h) {
    if (r >= 0.0 && r <= h) {
        float factor = 45.0 / (3.141592 * pow(h, 6));
        return factor * (h - r);
    }
    return 0.0;
}

bool isWithinSphere(float px, float py, float pz, int x, int y, int z, float radius) {
    float dx = px - float(x);
    float dy = py - float(y);
    float dz = pz - float(z);
    return (dx * dx + dy * dy + dz * dz) <= (radius * radius);
}

void main() {
    uint index = gl_GlobalInvocationID.x;
    if (index >= particles.length()) return;

    Particle pi = particles[index];

    // Compute density and pressure
    pi.density = 0.0;
    for (int j = 0; j < particles.length(); ++j) {
        float r = length(pi.position - particles[j].position);
        if (r < kernelRadius) {
            pi.density += mass * poly6Kernel(r, kernelRadius);
        }
    }
    pi.pressure = gasConstant * (pi.density - restDensity);

    // Compute forces
    vec3 pressureForce = vec3(0.0);
    vec3 viscosityForce = vec3(0.0);
    for (int j = 0; j < particles.length(); ++j) {
        if (index == j) continue;
        vec3 rVec = pi.position - particles[j].position;
        float r = length(rVec);

        // Pressure force
        if (r < kernelRadius) {
            pressureForce += -normalize(rVec) * mass * (pi.pressure + particles[j].pressure) / (2.0 * particles[j].density) * spikyGradientKernel(rVec, r, kernelRadius);
        }

        // Viscosity force
        if (r < kernelRadius) {
            viscosityForce += viscosity * mass * (particles[j].velocity - pi.velocity) / particles[j].density * viscosityLaplacianKernel(r, kernelRadius);
        }
    }
    vec3 gravityForce = vec3(0.0, -9.8, 0.0) * pi.density;

    pi.force = pressureForce + viscosityForce + gravityForce;

    // Clamp forces to prevent excessive acceleration
    if (length(pi.force) > maxForce) {
        pi.force = normalize(pi.force) * maxForce;
    }

    // Integration
    pi.acceleration = pi.force / pi.density;
    pi.velocity += deltaTime * pi.acceleration;

    // Clamp velocities to prevent excessive speed
    if (length(pi.velocity) > maxVelocity) {
        pi.velocity = normalize(pi.velocity) * maxVelocity;
    }

    pi.position += deltaTime * pi.velocity;

    // Bounding box collision handling
    if (pi.position.x < minBoundary.x) {
        pi.position.x = minBoundary.x;
        pi.velocity.x *= -0.5;
    }
    if (pi.position.x > maxBoundary.x) {
        pi.position.x = maxBoundary.x;
        pi.velocity.x *= -0.5;
    }
    if (pi.position.y < minBoundary.y) {
        pi.position.y = minBoundary.y;
        pi.velocity.y *= -0.5;
    }
    if (pi.position.y > maxBoundary.y) {
        pi.position.y = maxBoundary.y;
        pi.velocity.y *= -0.5;
    }
    if (pi.position.z < minBoundary.z) {
        pi.position.z = minBoundary.z;
        pi.velocity.z *= -0.5;
    }
    if (pi.position.z > maxBoundary.z) {
        pi.position.z = maxBoundary.z;
        pi.velocity.z *= -0.5;
    }

    particles[index] = pi;

    vec3 p = particles[index].position;

    int startX = int(floor(p.x - kernelRadius));
    int endX = int(ceil(p.x + kernelRadius));
    int startY = int(floor(p.y - kernelRadius));
    int endY = int(ceil(p.y + kernelRadius));
    int startZ = int(floor(p.z - kernelRadius));
    int endZ = int(ceil(p.z + kernelRadius));

    for (int x = startX; x <= endX; ++x) {
        if (x < 0 || x >= int(gridSize.x)) continue;
        for (int y = startY; y <= endY; ++y) {
            if (y < 0 || y >= int(gridSize.y)) continue;
            for (int z = startZ; z <= endZ; ++z) {
                if (z < 0 || z >= int(gridSize.z)) continue;
                if (isWithinSphere(p.x, p.y, p.z, x, y, z, kernelRadius)) {
                    int flatIndex = x + y * int(gridSize.x) + z * int(gridSize.x) * int(gridSize.y);
                    scalarValues[flatIndex] = -1.0;
                }
            }
        }
    }
}
