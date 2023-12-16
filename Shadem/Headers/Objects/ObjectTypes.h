#pragma once

namespace ObjectTypes {
	class Enum {
	public:
		enum Type	 {};
	};

	class PrimitiveObjectType : Enum {
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
	};

	class LightObjectType : Enum {
	public:
		enum Type {
			POINT_LIGHT,
			DIRECTION_LIGHT,
			POSITION_LIGHT,
			SPOT_LIGHT
		};
	};

	class CubeMarchingObjectType : Enum {
	public:
		enum Type {
			CUBE,
			PLANE,
			SPHERE,
			RANDOM
			// ... 
		};
	};

	class RayMarchingObjectType : Enum {
	public:
		enum Type {
			SPHERE,
			CUBE
			// ...
		};
	};


	// TODO: tutaj bêd¹ inne obiekty (jakie?), kamera chyba nie bêdzie obiektem
	class ComplexObjectType : Enum {
	public:
		enum Type {
			CAMERA,
			// ...
		};
	};
}