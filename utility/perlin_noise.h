//
// Created by matil on 4/23/2021.
//

#ifndef RAYTRACER_PERLIN_NOISE_H
#define RAYTRACER_PERLIN_NOISE_H

#include <random>

class PerlinNoise {
public:
    static double Noise(point3 vec3& p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());

        auto i = static_cast<int>(floor(p.x()));
        auto j = static_cast<int>(floor(p.y()));
        auto k = static_cast<int>(floor(p.z()));
        double c[2][2][2];

        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    c[di][dj][dk] = ranfloat[
                            perm_x[(i+di) & 255] ^
                            perm_y[(j+dj) & 255] ^
                            perm_z[(k+dk) & 255]
                    ];

        return trilinear_interp(c, u, v, w);
    }

private:
    static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
        auto accum = 0.0;
        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++)
                    accum += (i*u + (1-i)*(1-u))*
                             (j*v + (1-j)*(1-v))*
                             (k*w + (1-k)*(1-w))*c[i][j][k];

        return accum;
    }
};
#endif //RAYTRACER_PERLIN_NOISE_H
