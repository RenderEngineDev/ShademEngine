#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace MarchingCubeGenerator{

    class GridGenerator
    {
    public:
        glm::vec3 gridSize;

        GridGenerator(glm::vec3 gridSize);

        /**
         * @brief returns a random number between -1.0 and 1.0
         * @return float
         */
        float get_random_number();
        std::vector<std::vector<std::vector<float>>> generate_random_grid();
        std::vector<std::vector<std::vector<float>>> generate_sphere();
        std::vector<std::vector<std::vector<float>>> generate_nested_spheres();
        std::vector<std::vector<std::vector<float>>> read_grid_from_file(const char* path);


        // TODO: Po wprowadzeniu mo¿liwoœci modyfikowania obiektu Cube Marching w czasie rzeczywistym zaimplementowaæ - 
        //		 metodê update, która bêdzie przyjmowaæ grida i aktualizowa³a wartoœci w zale¿noœci od akcji 
    };
}