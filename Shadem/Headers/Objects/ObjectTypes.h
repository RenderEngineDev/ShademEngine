#pragma once

namespace ObjectTypes {

    class PrimitiveObjectType {
    public:
        enum Type {
            CUBE,
            PLANE,
            SPHERE,
            CIRCLE,
            CYLINDER,
            TORUS,
            CONE
            // ... 
        };

        PrimitiveObjectType(Type type) : type(type) {}
        Type getType() const {
            return type;
        }

    private:
        Type type;
    };

    class LightObjectType {
    public:
        enum Type {
            POINT_LIGHT,
            DIRECTION_LIGHT,
            POSITION_LIGHT,
            SPOT_LIGHT
        };

        LightObjectType(Type type) : type(type) {}
        Type getType() const {
            return type;
        }

    private:
        Type type;
    };

    class CubeMarchingObjectType {
    public:
        enum Type {
            CUBE,
            PLANE,
            SPHERE,
            RANDOM,
            NOISE_2D,
            NOISE_3D,
            NOISE_2D_GEOMETRY,
            NOISE_3D_GEOMETRY,
            NOISE_3D_COMPUTE,
            // ... 
        };

        CubeMarchingObjectType(Type type) : type(type) {}
        Type getType() const {
            return type;
        }

    private:
        Type type;
    };

    class RayMarchingObjectType {
    public:
        enum Type {
            SPHERE,
            CUBE
            // ...
        };

        RayMarchingObjectType(Type type) : type(type) {}
        Type getType() const {
            return type;
        }

    private:
        Type type;
    };

    class ComplexObjectType {
    public:
        enum Type {
            CAMERA,
            // ...
        };

        ComplexObjectType(Type type) : type(type) {}
        Type getType() const {
            return type;
        }

    private:
        Type type;
    };
}