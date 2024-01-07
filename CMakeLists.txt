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

########################################################################

# Utwórz zmienną zawierającą wszystkie pliki źródłowe i nagłówkowe z katalogu "src"
file(GLOB_RECURSE SOURCE_FILES "Shadem/Source/*.cpp")
file(GLOB_RECURSE HEADER_FILES "Shadem/Headers/*.h")
file(GLOB_RECURSE SHADER_FILES "Shadem/Shaders/*.glsl")

########################################################################

# Dodaj źródła do projektu
add_executable(ShademEngine ${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})
# Dodaj katalog z nagłówkami do listy ścieżek
include_directories(${CMAKE_SOURCE_DIR}/Shadem/Headers)

# Dodaj biblioteki do projektu
target_link_libraries(ShademEngine
    ${GLFW_LIBRARIES}
)

########################################################################
### OPEN MP
########################################################################

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

########################################################################
### COMPILER OPTIMALIZATION
########################################################################

# Ustaw flagi optymalizacji kompilatora
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

########################################################################
### GLAD
########################################################################

add_subdirectory(include/glad/)

# Dodaj pliki glad do projektu
add_library(glad STATIC
    include/glad/glad.c
    include/glad/glad.h
)

# Dodaj zależności glad do ShademEngine
target_link_libraries(ShademEngine glad)


########################################################################
### IMGUI
########################################################################

add_subdirectory(include/imgui/)

# Dodaj pliki glad do projektu
add_library(imgui STATIC
    include/imgui/imconfig.h
    include/imgui/imgui.cpp
    include/imgui/imgui.h
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_internal.h
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_impl_opengl3.cpp
    include/imgui/imgui_impl_opengl3.h
    include/imgui/imgui_impl_opengl3_loader.h
    include/imgui/imgui_impl_glfw.cpp
    include/imgui/imgui_impl_glfw.h
    include/imgui/imstb_rectpack.h
    include/imgui/imstb_textedit.h
    include/imgui/imstb_truetype.h
)

# Dodaj zależności imgui do ShademEngine
target_link_libraries(ShademEngine imgui)

########################################################################
# Przenieś pliki nagłówkowe do folderu "Header Files" w projekcie Visual Studio
foreach(header IN LISTS HEADER_FILES)
    get_filename_component(header_name "${header}" NAME)
    get_filename_component(header_path "${header}" DIRECTORY)
    string(REPLACE "${CMAKE_SOURCE_DIR}/Shadem/Headers/" "" header_path_msvc "${header_path}")
    string(REPLACE "/" "\\" header_path_msvc "${header_path_msvc}")
    source_group("Header Files\\${header_path_msvc}" FILES "${header}")
endforeach()

# Przenieś pliki źródłowe do folderu "Source Files" w projekcie Visual Studio
foreach(source IN LISTS SOURCE_FILES)
    get_filename_component(source_name "${source}" NAME)
    get_filename_component(source_path "${source}" DIRECTORY)
    string(REPLACE "${CMAKE_SOURCE_DIR}/Shadem/Source" "" source_path_msvc "${source_path}")
    string(REPLACE "/" "\\" source_path_msvc "${source_path_msvc}")
    source_group("Source Files\\${source_path_msvc}" FILES "${source}")
endforeach()

# Przenieś pliki shaderów do folderu "Shader Files" w projekcie Visual Studio
foreach(shader IN LISTS SHADER_FILES)
    get_filename_component(shader_name "${shader}" NAME)
    get_filename_component(shader_path "${shader}" DIRECTORY)
    string(REPLACE "${CMAKE_SOURCE_DIR}/Shadem/Shaders" "" shader_path_msvc "${shader_path}")
    string(REPLACE "/" "\\" shader_path_msvc "${shader_path_msvc}")
    source_group("Shader Files\\${shader_path_msvc}" FILES "${shader}")
endforeach()

########################################################################

# Ustaw projekt startowy
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ShademEngine)

# Ustaw widoczność folderów
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Stwórz foldery dla projektu głównego oraz dla dołączonych bibliotek
set_target_properties(glad PROPERTIES FOLDER Libraries)
set_target_properties(imgui PROPERTIES FOLDER Libraries)
set_target_properties(ShademEngine PROPERTIES FOLDER Develop)