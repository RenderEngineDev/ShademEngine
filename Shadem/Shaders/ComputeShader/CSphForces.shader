# version 440

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
uniform float gz;                     // m/s^2
uniform float gy;                     // m/s^2
uniform float gx;
uniform float dt;
uniform float k;                 // J/(mol*K) - gas constant
uniform float M_PI;


layout(std430, binding = 0) buffer fparticle { vec4 p[]; };
layout(std430, binding = 1) buffer fvx { float vx[]; };
layout(std430, binding = 2) buffer fvy { float vy[]; };
layout(std430, binding = 3) buffer frho { float rho[]; };
layout(std430, binding = 4) buffer fmass { float mass[]; };
layout(std430, binding = 5) buffer fforce_pres_x { float force_pres_x[]; };
layout(std430, binding = 6) buffer fforce_pres_y { float force_pres_y[]; };
layout(std430, binding = 7) buffer fforce_visc_x { float force_visc_x[]; };
layout(std430, binding = 8) buffer fforce_visc_y { float force_visc_y[]; };
layout(std430, binding = 9) buffer fvz { float vz[]; };
layout(std430, binding = 10) buffer fforce_pres_z { float force_pres_z[]; };
layout(std430, binding = 11) buffer fforce_visc_z { float force_visc_z[]; };

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
const int SW = 1;// 1280;
const int SH = 1;// 720;
const int SD = 1;// 720;

float W_poly6(float d) // eq. 2.30, pp. 24
{
	if (d <= h)
		return 315. / (64. * M_PI * powh9) * (powh2 - d * d) * (powh2 - d * d) * (powh2 - d * d);
	else
		return 0;
}
float W_poly6_grad(float d)
{
	return 945. / (32. * M_PI * powh9) * pow(powh2 - pow(d, 2), 2);
}
float W_poly6_grad_x(float rx, float ry, float rz, float d)
{
	return -rx * W_poly6_grad(d);
}
float W_poly6_grad_y(float rx, float ry, float rz, float d)
{
	return -ry * W_poly6_grad(d);
}
float W_poly6_grad_z(float rx, float ry, float rz, float d)
{
	return -rz * W_poly6_grad(d);
}
float W_poly6_lapl(float d)
{
	float powd2 = pow(d, 2);
	return 945. / (8. * M_PI * powh9) * (powh2 - pow(d, 2)) * (powd2 - 3.0 / 4.0 * (powh2 - powd2));
}
float W_spiky_grad(float d)
{
	return 45. / (M_PI * powh6 * d) * (h - d) * (h - d);
}
float W_spiky_grad_x(float rx, float ry, float rz, float d)
{
	return -rx * W_spiky_grad(d);
}
float W_spiky_grad_y(float rx, float ry, float rz, float d)
{
	return -ry * W_spiky_grad(d);
}
float W_spiky_grad_z(float rx, float ry, float rz, float d)
{
	return -rz * W_spiky_grad(d);
}
float W_viscosity_lapl(float d)
{
	return 45. / (M_PI * powh5) * (1 - d / h);
}

void main()
{
	//return;
	int idx = int(gl_GlobalInvocationID.x);
	if (idx >= N) return;
	float px = p[idx].x;
	float py = p[idx].y;
	float pz = p[idx].z;
	//px /= W;
	//py /= H;

	// calc forces here

	float rx, ry, rz, d;
	float pi, pj;
	force_pres_x[idx] = 0;
	force_pres_y[idx] = 0;
	force_pres_z[idx] = 0;
	force_visc_x[idx] = 0;
	force_visc_y[idx] = 0;
	force_visc_z[idx] = 0;

	for (int j = 0; j < N; j++)
		if (idx != j)
		{
			rx = px - p[j].x;///SW;
			ry = py - p[j].y;///SH;
			rz = pz - p[j].z;///SH;
			d = sqrt(rx * rx + ry * ry + rz * rz);
			if (d <= h)
				//if(rho[j]!=0.0 && rho[idx]!=0.0)
			{
				// equation of state, Mueller et. al., p=k(rho-rho0)
				pi = k * (rho[idx] - rho0);
				pj = k * (rho[j] - rho0);
				//			  cout << rho[idx] << " " << rho[j] << " " << rho0 << " " << pi <<  " " << pj << endl;
				force_pres_x[idx] -= mass[idx] * (pi + pj) / (2. * rho[j]) * W_spiky_grad_x(rx, ry, rz, d);            // p. 22, Auer za MCG03
				force_pres_y[idx] -= mass[idx] * (pi + pj) / (2. * rho[j]) * W_spiky_grad_y(rx, ry, rz, d);            // p. 22, Auer za MCG03
				force_pres_z[idx] -= mass[idx] * (pi + pj) / (2. * rho[j]) * W_spiky_grad_z(rx, ry, rz, d);            // p. 22, Auer za MCG03
				force_visc_x[idx] += (mu / rho0) * mass[idx] * (vx[j] - vx[idx]) / rho[j] * W_viscosity_lapl(d);   // 2.26
				force_visc_y[idx] += (mu / rho0) * mass[idx] * (vy[j] - vy[idx]) / rho[j] * W_viscosity_lapl(d);   // 2.26
				force_visc_z[idx] += (mu / rho0) * mass[idx] * (vz[j] - vz[idx]) / rho[j] * W_viscosity_lapl(d);   // 2.26
			}
		}
}
