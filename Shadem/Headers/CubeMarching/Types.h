#pragma once

#include <map>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

struct Point
{
    float x;
    float y;
    float z;
};

struct GridCell
{
    Point vertex[8];
    float value[8];
};

struct GridCellValues
{
    glm::vec4 valuesL;
    glm::vec4 valuesR;
};

//struct Particle {
//    glm::vec3 position;
//    glm::vec3 velocity;
//    glm::vec3 acceleration;
//    glm::vec3 force;
//    float density;
//    float pressure;
//
//    Particle(){}
//    Particle(glm::vec3 pos, glm::vec3 velocity)
//        : position(pos), velocity(velocity), force(0.0f), acceleration(0.0f), density(0.0f), pressure(0.0f) {}
//    Particle(glm::vec3 pos)
//        : position(pos), velocity(0.0f), force(0.0f), acceleration(0.0f), density(0.0f), pressure(0.0f) {}
//};

struct Particle {
    //float rho;
    //float mass;
    glm::vec3 RM; // rho, mass, slot1
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 fPresure;
    glm::vec3 fViscosity;

   /* Particle() : position(0.0f), velocity(0.0f), fPresure(0.0f), fViscosity(0.0f), rho(5.0f), mass(0.003f) {}
    Particle(glm::vec3 pos)
        : position(pos), velocity(0.0f), fPresure(0.0f), fViscosity(0.0f), rho(5.0f), mass(0.003f) {}*/
    Particle() : position(0.0f), velocity(0.0f), fPresure(0.0f), fViscosity(0.0f), RM(5.0f, 0.003f, 0.0f) {}
    Particle(glm::vec3 pos)
        : position(pos), velocity(0.0f), fPresure(0.0f), fViscosity(0.0f), RM(5.0f, 0.003f, 0.0f) {}

};