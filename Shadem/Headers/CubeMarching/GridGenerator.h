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
        std::vector<std::vector<std::vector<float>>> generate_noise2D(float& frequency, float& amplitude, float& lacunarity, float& persistence, float& scale, glm::vec3& offset, std::vector<std::vector<std::vector<float>>>& scalarFunction);
        std::vector<std::vector<std::vector<float>>> generate_noise3D(float& frequency, float& amplitude, float& lacunarity, float& persistence, float& scale, glm::vec3& offset, std::vector<std::vector<std::vector<float>>>& scalarFunction);
        std::vector<std::vector<std::vector<float>>> read_grid_from_file(const char* path);


        // TODO: przechowywaæ siatke punktów i zrobiæ metodê do update (do trybu edycji w przyszlosci)
    };
}