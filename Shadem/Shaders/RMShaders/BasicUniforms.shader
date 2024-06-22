#ifndef BASICUNIFORMS_SHADER
#define BASICUNIFORMS_SHADER

uniform mat4 View;
uniform vec3 CameraFront;
uniform vec2 CameraRange;
uniform vec3 CameraPos;
uniform vec2 WindowSize;
uniform float Zoom;
uniform bool sampleDepth;

in vec2 TexCoords;

uniform sampler2D depthSampler;
uniform sampler2D colorSampler;

float calculate_depth(in float x, in float y, in float z)
{
	return (1.0 / z - 1.0 / x) / (1.0 / y - 1.0 / x);
}

float calculate_linear_depth(in float x)
{
	return (x - CameraRange.x) / (CameraRange.y - CameraRange.x);
}

vec3 rayDirectionBeforeRotation;
vec2 uv;

#endif