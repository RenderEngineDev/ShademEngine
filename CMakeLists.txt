cmake_minimum_required(VERSION 3.10)
project(ShademEngine)

# Ścieżka do głównego katalogu include, w którym znajdują się katalogi glad, GLFW, glm, KHR itp.
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(GLFW_LIBRARY_DIR "lib")

# Dodaj katalog include do listy ścieżek
include_directories(${INCLUDE_DIR})

# Znajdź i dołącz bibliotekę GLFW
# find_package(glfw3 REQUIRED)
# target_link_libraries(ShademEngine glfw)

# Ustaw zmienne dla bibliotek GLFW, GLEW i GLM
find_library(GLFW_LIBRARIES glfw3 HINTS ${GLFW_LIBRARY_DIR})

# Dodaj źródło projektu
add_executable(ShademEngine src/Shadem.cpp)

# Dodaj biblioteki do projektu
target_link_libraries(ShademEngine
    ${GLFW_LIBRARIES}
)

# Ustaw projekt startowy
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ShademEngine)

# Tłumienie ZERO_CHECK - cmake update
# set(CMAKE_SUPPRESS_REGENERATION true)