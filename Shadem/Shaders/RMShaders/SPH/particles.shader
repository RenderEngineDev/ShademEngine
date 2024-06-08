# version 440
//kacper\\
uniform int N;
uniform float de;
uniform float h;   // smoothing length
uniform float powh9;
uniform float powh2;
uniform float powh6;
uniform float powh5;
uniform float rho0;             // density of water - kg/m^3
uniform float mass0;
uniform float mu;            // dymamic viscosity of water - kg(m*s)
uniform float gy;                     // m/s^2
uniform float gz;                     // m/s^2
uniform float gx;
uniform float dt;
uniform float k;                 // J/(mol*K) - gas constant
uniform float M_PI;

layout(binding = 0) buffer fparticle { vec4 p [ ]; };
layout(binding = 1) buffer fvx { float vx [  ]; };
layout(binding = 2) buffer fvy { float vy [  ]; };
layout(binding = 3) buffer frho { float rho [  ]; };
layout(binding = 4) buffer fmass { float mass [  ]; };
layout(binding = 5) buffer fforce_pres_x { float force_pres_x [  ]; };
layout(binding = 6) buffer fforce_pres_y { float force_pres_y [  ]; };
layout(binding = 7) buffer fforce_visc_x { float force_visc_x [  ]; };
layout(binding = 8) buffer fforce_visc_y { float force_visc_y [  ]; };
layout(binding = 9) buffer fvz { float vz [  ]; };
layout(binding = 10) buffer fforce_pres_z { float force_pres_z [  ]; };
layout(binding = 11) buffer fforce_visc_z { float force_visc_z [  ]; };

layout( local_size_x = 64, local_size_y = 1, local_size_z = 1 ) in;
const int SW = 1;
const int SH = 1;


void main()
{
	int idx = int(gl_GlobalInvocationID.x);
	//p[idx] = vec4(1.0f);

	float px = p[idx].x;
	float py = p[idx].y;
	float pz = p[idx].z;
	//px /= float(W);
	//py /= float(H);
	// move particles here
	float st, fx, fy, fz, ax, ay, az;
	float pxnew=px, pynew=py, pznew = pz;

	fx = force_pres_x[idx] + force_visc_x[idx];
	fy = force_pres_y[idx] + force_visc_y[idx];
	fz = force_pres_z[idx] + force_visc_z[idx];


	float cx = 0.5, cy=0.5, cz = 0.4, r = 0.5;
	float k = 5.0;
	float fsx=0, fsy = 0, fsz = 0;
	/*
	 //if (pznew > cz)
		if ((pxnew-cx)*(pxnew-cx)+(pynew-cy)*(pynew-cy)+(pznew-cz)*(pznew-cz)>r*r)
		{
			//Get distance from edge
			float d = sqrt((pxnew - cx) * (pxnew - cx) + (pynew - cy) * (pynew - cy) + (pznew - cz) * (pznew - cz)) - r;
			float nx = pxnew - cx;
			float ny = pynew - cy;
			float nz = pznew - cz;
			float dn = sqrt(nx*nx + ny*ny + nz*nz);
			//nx = nx / dn;
			//ny = ny / dn;
			fsx = -k * nx;	// pxnew = pxnew - nx * d;
			fsy = -k * ny;	// pynew = pynew - ny * d;
			fsz = -k * nz;	// pynew = pynew - ny * d;
			//pxnew = pxnew - nx * d;
			//pynew = pynew - ny * d;
			//vx[idx] *= 0.96;
			//vy[idx] *= 0.96;
			//vz[idx] *= 0.96;
		}*/


	//if (rho[idx]!=0.0)
	{
		ax = dt * fx / rho[idx] + gx + fsx;///rho[idx];
		ay = dt * fy / rho[idx] + gy + fsy;///rho[idx];
		az = dt * fz / rho[idx] + gz + fsz;///rho[idx];
		vx[idx] = vx[idx] + ax * dt;
		vy[idx] = vy[idx] + ay * dt;
		vz[idx] = vz[idx] + az * dt;
		pxnew = px + dt * vx[idx];
		pynew = py + dt * vy[idx];
		pznew = pz + dt * vz[idx];
	}




	const float FRICT = 0.9;

	if (pznew < 0)
	{
		pznew = -pznew;
		vz[idx] = -vz[idx] * FRICT;
	}
	if (pznew > 1)
	{
		pznew = 1 - (pznew - 1);
		vz[idx] = -vz[idx] * FRICT;
	}
	if (pxnew < 0)
	{
		pxnew = -pxnew;
		vx[idx] = -vx[idx] * FRICT;
	}
	if (pxnew > 1)
	{
		pxnew = 1 - (pxnew - 1);
		vx[idx] = -vx[idx] * FRICT;
	}
	if (pynew < 0)
	{
		pynew = -pynew;
		vy[idx] = -vy[idx] * FRICT;
	}
	if (pynew > 1)
	{
		pynew = 1 - (pynew - 1);
		vy[idx] = -vy[idx] * FRICT;
	}
	
	p[idx].x = pxnew; // *W;
	p[idx].y = pynew; // *H;
	p[idx].z = pznew; // *H;

}

