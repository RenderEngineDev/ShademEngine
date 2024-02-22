#pragma once
/*
    Tables and conventions from
    http://paulbourke.net/geometry/polygonise/
*/

#include <iostream>
#include "CubeMarching/TrianglesGenerator.h"
//#include <chrono>

#include <omp.h>

/// <summary>
/// Wyznacza, któr¹ z 256 kombinacji bêdzie reprezentowaæ dany szeœcian
/// </summary>
int TrianglesGenerator::calculate_cube_index(const GridCell &cell, const float &isovalue) {
    int cubeIndex = 0;
    for (int i = 0; i < 8; i++)
        if (cell.value[i] < isovalue) {
            cubeIndex |= (1 << i);
        }
    return cubeIndex;
}

/// <summary>
/// Oblicza, które krawêdzie bêd¹ tworzy³ powierzchnie (na których krawêdziach bêd¹ wierzcho³ki tworz¹ce trójk¹ty - oblicza wspó³rzêdne tych wierzcho³ków) i zwraca wszystkie takie punkty
/// </summary>
std::vector<Point> TrianglesGenerator::get_intersection_coordinates(const GridCell &cell, const float &isovalue) {
    std::vector<Point> intersections (12);

    int cubeIndex = calculate_cube_index(cell, isovalue);
    int intersectionsKey = edgeTable[cubeIndex];
    int idx = 0;
    while (intersectionsKey)
    {
        if (intersectionsKey&1)
        {
            int v1 = edgeToVertices[idx].first, v2 = edgeToVertices[idx].second;
            Point intersectionPoint = interpolate(cell.vertex[v1], cell.value[v1],
                                                    cell.vertex[v2], cell.value[v2], isovalue);
            intersections[idx] = intersectionPoint;
        }
        idx++;
        intersectionsKey >>= 1;
    }


    return intersections;
}

/// <summary>
/// na podstawie 2 punktów tworz¹cych krawêdŸ komórki i wartoœci isovalue okreœla wspó³rzêdne punktu le¿¹cego na tej krawêdzi
/// </summary>
/// <returns></returns>
Point TrianglesGenerator::interpolate(const Point &v1, const float &val1, const Point &v2, const float &val2, const float &isovalue) {
    Point interpolated;
    float mu = (isovalue - val1) / (val2 - val1);
    interpolated.x = (v1.x + mu * (v2.x - v1.x));
    interpolated.y = (v1.y + mu * (v2.y - v1.y));
    interpolated.z = (v1.z + mu * (v2.z - v1.z));
    return interpolated;
}

/// <summary>
/// z wektora wszystkich wierzcho³ków w danej komórce siatki okreœlia trójk¹ty
/// </summary>
/// <returns> 
/// wektor wektora 3 punktów (wektor trójk¹tów)
/// </returns> 
std::vector<std::vector<Point>> TrianglesGenerator::get_triangles(const std::vector<Point> &intersections, const int &cubeIndex) {
    std::vector<std::vector<Point>> triangles;
    for (int i = 0; triangleTable[cubeIndex][i] != -1; i += 3)
    {
        std::vector<Point> triangle (3);
        for (int j = 0; j < 3; j++)
            triangle[j] = intersections[triangleTable[cubeIndex][i + j]];
        triangles.push_back(triangle);
    }

    return triangles;
}

void TrianglesGenerator::print_triangles(std::vector<std::vector<Point>> triangles) {
    for (int i = 0; i < (int)triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
            std::cout << triangles[i][j].x << ",\t" << triangles[i][j].y << ",\t" << triangles[i][j].z << "\n";
        std::cout << "\n";
    }
}

/// <summary>
/// oblicza wszystkie wierzcho³ki dla danej "komórki" ca³ej siatki tworz¹cej obiekt i zwracaj¹cej wektor trójkatów (wektor wektora 3 punktów tworz¹cych trójk¹t)
/// </summary>
/// <returns></returns>
std::vector<std::vector<Point>> TrianglesGenerator::triangulate_cell(const GridCell &cell, const float &isovalue) {
    int cubeIndex = calculate_cube_index(cell, isovalue);
    std::vector<Point> intersections = get_intersection_coordinates(cell, isovalue);
    std::vector<std::vector<Point>> triangles = get_triangles(intersections, cubeIndex);

    return triangles;
}


/// <summary>
/// oblicza i zwraca wszystkie wierzcho³ki tworz¹ce trójk¹ty (co 3)
/// </summary>
/// <returns></returns>
void TrianglesGenerator::triangulate_field(const std::vector<std::vector<std::vector<float>>>& scalarFunction, const float &isovalue) {
    //auto start = std::chrono::steady_clock::now();
    int i, j, k;
    float offsetX = 1.0f / gridSize.x, offsetY = 1.0f / gridSize.y, offsetZ = 1.0f / gridSize.z;
    //std::vector<std::vector<Point>> triangles;
    glm::ivec3 igridSize = gridSize;
    if (triangles.size() == 0) {
        triangles.reserve(igridSize.x * igridSize.y * igridSize.z * 4);
        triangles.resize(igridSize.x * igridSize.y * igridSize.z * 4);
    } else {
        for (auto& triangle : triangles) {
            triangle = {};
        }
    }

    #pragma omp parallel for private(i, j, k) collapse(3)
    for (i = 0; i < igridSize.x - 1; i++) {
        for (j = 0; j < igridSize.y - 1; j++) {
            for (k = 0; k < igridSize.z - 1; k++) {
                float x = i / gridSize.x - 0.5f, y = j / gridSize.y - 0.5f, z = k / gridSize.z - 0.5f;
                // cell ordered according to convention in referenced website
                const GridCell cell = {
                    {
                        {x, y, z}, {x + offsetX, y, z},
                        {x + offsetX, y, z + offsetZ}, {x, y, z + offsetZ},
                        {x, y + offsetY, z}, {x + offsetX, y + offsetY, z},
                        {x + offsetX, y + offsetY, z + offsetZ}, {x, y + offsetY, z + offsetZ}
                    },
                    {
                        scalarFunction[i][j][k], scalarFunction[i + 1][j][k],
                        scalarFunction[i + 1][j][k + 1], scalarFunction[i][j][k + 1],
                        scalarFunction[i][j + 1][k], scalarFunction[i + 1][j + 1][k],
                        scalarFunction[i + 1][j + 1][k + 1], scalarFunction[i][j + 1][k + 1]
                    }
                };
                int cubeIndex = calculate_cube_index(cell, isovalue);
                if (cubeIndex != 0 && cubeIndex != 255) {
                    std::vector<std::vector<Point>> cellTriangles = triangulate_cell(cell, isovalue);
                    // nie powinno byæ wiêcej ni¿ 4 elementy w wektorze wektora cell triangles (mo¿e byæ wiêcej przy bardzo gêstych siatkach 350+, ale chyba wprowadzimy limit)
                    int index = (i * igridSize.x * igridSize.y + j * igridSize.z + k) * 4;
                    //#pragma omp critical
                    //triangles.insert(triangles.end(), cellTriangles.begin(), cellTriangles.end());
                    for (int i = 0; i < (int)cellTriangles.size(); i++) {
                        triangles[index + i] = cellTriangles[i];
                        //triangles.push_back(cellTriangles[i]);
                    }
                }
            }
        }
    }

    //std::cout << "Elapsed(ms)= triangles " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << std::endl;
    //return triangles;
}