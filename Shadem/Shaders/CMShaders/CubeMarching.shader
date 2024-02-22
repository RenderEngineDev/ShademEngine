#ifndef CUBE_MARCHING_SHADER
#define CUBE_MARCHING_SHADER

#ifndef MAX_TRIANGLE_AMOUNT 
    #define MAX_TRIANGLE_AMOUNT 12
#endif

#ifndef GRID_CELL
#define GRID_CELL
struct GridCell
{
    vec3 vertex[8];
    float value[8];
};
#endif

/// Wyznacza, któr¹ z 256 kombinacji bêdzie reprezentowaæ dany szeœcian
int calculateCubeIndex(in const GridCell cell) {
    int cubeIndex = 0;
    // this loop decrease performance by 5/10+fps ;C
    /*for (int i = 0; i < 8; i++)
        if (cell.value[i] < isoValue) {
            cubeIndex |= (1 << i);
        }*/
    cubeIndex |= ((cell.value[0] < isoValue ? 1 : 0) << 0);
    cubeIndex |= ((cell.value[1] < isoValue ? 1 : 0) << 1);
    cubeIndex |= ((cell.value[2] < isoValue ? 1 : 0) << 2);
    cubeIndex |= ((cell.value[3] < isoValue ? 1 : 0) << 3);
    cubeIndex |= ((cell.value[4] < isoValue ? 1 : 0) << 4);
    cubeIndex |= ((cell.value[5] < isoValue ? 1 : 0) << 5);
    cubeIndex |= ((cell.value[6] < isoValue ? 1 : 0) << 6);
    cubeIndex |= ((cell.value[7] < isoValue ? 1 : 0) << 7);
    return cubeIndex;
}

/// na podstawie 2 punktów tworz¹cych krawêdŸ komórki i wartoœci isovalue okreœla wspó³rzêdne punktu le¿¹cego na tej krawêdzi
vec3 interpolate(in const vec3 v1, in const float val1, in const vec3 v2, in const float val2, in const float isovalue) {
    vec3 interpolated;
    float mu = (isoValue - val1) / (val2 - val1);
    interpolated.x = (v1.x + mu * (v2.x - v1.x));
    interpolated.y = (v1.y + mu * (v2.y - v1.y));
    interpolated.z = (v1.z + mu * (v2.z - v1.z));
    return interpolated;
}


vec3[MAX_TRIANGLE_AMOUNT] getIntersectionCoordinates(in const GridCell cell, in const int cubeIndex) {

    vec3[MAX_TRIANGLE_AMOUNT] intersections;
    int intersectionsKey = edgeTable[cubeIndex];
    int idx = 0;
    // BEZ PETLI +80FPS
    
#pragma FOR_LOOP(12)
if (intersectionsKey & 1) {
int v1 = edgeToVertices[i][0], v2 = edgeToVertices[i][1];
vec3 intersectionvec3 = interpolate(cell.vertex[v1], cell.value[v1], cell.vertex[v2], cell.value[v2], isoValue);
intersections[i] = intersectionvec3;}
intersectionsKey >>= 1;
if (intersectionsKey == 0) return intersections;
#pragma END_FOR_LOOP
    return intersections;
}

GridCell calculateCellValuesForNoise3D(in const vec3 offset, in const float noiseScale, in const float frequency, in const float amplitude, in const float persistence, in const float lacunarity) {
    vec3 cvOffset = vec3(1.0f / gridSize.x, 1.0f / gridSize.y, 1.0f / gridSize.z); // cell vertex offset
    //float x = gl_in[0].gl_Position.x / gridSize.z - 0.5f, y = gl_in[0].gl_Position.y / gridSize.y - 0.5f, z = gl_in[0].gl_Position.z / gridSize.z - 0.5f;
    float x  = FragPos[0].x;
    float xo = FragPos[0].x + cvOffset.x;   // x + offset
    float y  = FragPos[0].y;
    float yo = FragPos[0].y + cvOffset.y;
    float z  = FragPos[0].z;
    float zo = FragPos[0].z + cvOffset.z;
    float nX  = (x  + offset.x * noiseScale);
    float nXo = (xo + offset.x * noiseScale);
    float nY  = (y  + offset.y * noiseScale);
    float nYo = (yo + offset.y * noiseScale);
    float nZ  = (z  + offset.z * noiseScale);
    float nZo = (zo + offset.z * noiseScale);

    GridCell cell;
    cell.vertex[0] = vec3(x,  y,  z);
    cell.vertex[1] = vec3(xo, y,  z);
    cell.vertex[2] = vec3(xo, y,  zo);
    cell.vertex[3] = vec3(x,  y,  zo);
    cell.vertex[4] = vec3(x,  yo, z);
    cell.vertex[5] = vec3(xo, yo, z);
    cell.vertex[6] = vec3(xo, yo, zo);
    cell.vertex[7] = vec3(x,  yo, zo);
    cell.value[0] = cnoise(vec3(nX,     nY,     nZ),    frequency, amplitude, persistence, lacunarity);
    cell.value[1] = cnoise(vec3(nXo,    nY,     nZ),    frequency, amplitude, persistence, lacunarity);
    cell.value[2] = cnoise(vec3(nXo,    nY,     nZo),   frequency, amplitude, persistence, lacunarity);
    cell.value[3] = cnoise(vec3(nX,     nY,     nZo),   frequency, amplitude, persistence, lacunarity);
    cell.value[4] = cnoise(vec3(nX,     nYo,    nZ),    frequency, amplitude, persistence, lacunarity);
    cell.value[5] = cnoise(vec3(nXo,    nYo,    nZ),    frequency, amplitude, persistence, lacunarity);
    cell.value[6] = cnoise(vec3(nXo,    nYo,    nZo),   frequency, amplitude, persistence, lacunarity);
    cell.value[7] = cnoise(vec3(nX,     nYo,    nZo),   frequency, amplitude, persistence, lacunarity);

   
    return cell;
}

float getNoise2Value(float noiseResult, float currentVertexYPosition) {
    if (currentVertexYPosition > noiseResult) {
        return -1.0f;
    }
    else {
        return 1.0;
    }
}

GridCell calculateCellValuesForNoise2D(in const vec3 offset, in const float noiseScale, in const float frequency, in const float amplitude, in const float persistence, in const float lacunarity) {
    vec3 cvOffset = vec3(1.0f / gridSize.x, 1.0f / gridSize.y, 1.0f / gridSize.z); // cell vertex offset
    //float x = gl_in[0].gl_Position.x / gridSize.z - 0.5f, y = gl_in[0].gl_Position.y / gridSize.y - 0.5f, z = gl_in[0].gl_Position.z / gridSize.z - 0.5f;
    float x = FragPos[0].x;
    float y = FragPos[0].y;
    float z = FragPos[0].z;
    float nX = (x + offset.x * noiseScale);
    float nY = (y + offset.y * noiseScale);
    float nZ = (z + offset.z * noiseScale);

    GridCell cell;
    cell.vertex[0] = vec3(x, y, z);
    cell.vertex[1] = vec3(x + cvOffset.x, y, z);
    cell.vertex[2] = vec3(x + cvOffset.x, y, z + cvOffset.z);
    cell.vertex[3] = vec3(x, y, z + cvOffset.z);
    cell.vertex[4] = vec3(x, y + cvOffset.y, z);
    cell.vertex[5] = vec3(x + cvOffset.x, y + cvOffset.y, z);
    cell.vertex[6] = vec3(x + cvOffset.x, y + cvOffset.y, z + cvOffset.z);
    cell.vertex[7] = vec3(x, y + cvOffset.y, z + cvOffset.z);
    cell.value[0] = getNoise2Value(cnoise(vec2(nX, nZ), frequency, amplitude, persistence, lacunarity), nY);
    cell.value[1] = getNoise2Value(cnoise(vec2(nX + cvOffset.x, nZ), frequency, amplitude, persistence, lacunarity), nY);
    cell.value[2] = getNoise2Value(cnoise(vec2(nX + cvOffset.x, nZ + cvOffset.z), frequency, amplitude, persistence, lacunarity), nY);
    cell.value[3] = getNoise2Value(cnoise(vec2(nX, nZ + cvOffset.z), frequency, amplitude, persistence, lacunarity), nY);
    cell.value[4] = getNoise2Value(cnoise(vec2(nX, nZ), frequency, amplitude, persistence, lacunarity), nY + cvOffset.y);
    cell.value[5] = getNoise2Value(cnoise(vec2(nX + cvOffset.x, nZ), frequency, amplitude, persistence, lacunarity), nY + cvOffset.y);
    cell.value[6] = getNoise2Value(cnoise(vec2(nX + cvOffset.x, nZ + cvOffset.z), frequency, amplitude, persistence, lacunarity), nY + cvOffset.y);
    cell.value[7] = getNoise2Value(cnoise(vec2(nX, nZ + cvOffset.z), frequency, amplitude, persistence, lacunarity), nY + cvOffset.y);

    return cell;
}

#endif