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


layout(binding = 0) buffer fparticle { vec4 p []; };
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
//const int W = 1280;
//const int H = 720;
const float SW = 1;
const float SH = 1;
const float SD = 1;

float W_poly6(float d) // eq. 2.30, pp. 24
{
    if (d <= h)
        return 315./ (64.* M_PI * powh9) * (powh2 - d * d) * (powh2 - d * d) * (powh2 - d * d);
    else
        return 0;
}

void main()
{
    //return;
    int idx = int(gl_GlobalInvocationID.x);
	float px = p[idx].x;
    float py = p[idx].y;
    float pz = p[idx].z;
    //px /= float(W);
    //py /= float(H);

    // calc density here
    float rx, ry, rz, d;
    int n;
    rho[idx] = rho0;
    n = 0;
    for (int j = 0; j < N; j++)
    {
        if (idx != j)
        {
            rx = px - p[j].x;
            ry = py - p[j].y;
            rz = pz - p[j].z;
            d = sqrt(rx*rx + ry*ry + rz*rz);
            if (d <= h)
            {
                rho[idx] += mass[j] * W_poly6(d); // i.e. Mueller et.al., eq. 3
            }
        }
    }

}

