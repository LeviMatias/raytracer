//
// Created by Matias Ignacio Levi Fernandez on 12/03/2021.
//

#include "draw_thread.h"
#include "../../utility/Ray.h"

DrawThread::DrawThread(Scene *w, Camera *c, Image *i, int start, int ofs) {
    canvas = i;
    cam = c;
    offset = ofs;
    offset_start = start;
    world = w;
}

void DrawThread::_run() {
    for (int i = offset_start; i < canvas->h * canvas->w; i += offset){
        Color pixel_color(0, 0, 0);
        int y = static_cast<int>(i / canvas->w);
        int x = i - canvas->w * y;

        for (int s = 0; s < SAMPLES; ++s) {
            auto u = (x + Random::NextNumber()) / (canvas->w-1);
            auto v = (y + Random::NextNumber()) / (canvas->h-1);
            Ray r = cam->GetRay(u, v);
            pixel_color += _ray2Color(r);
        }
        canvas->WriteColorAt(pixel_color / SAMPLES, x, y);
    }
}

Color DrawThread::_ray2Color(const Ray& r) const {

    return _ray2Color_rec(r, 0);
}

Color DrawThread::_ray2Color_rec(const Ray &r, int depth) const {
    if (depth >= MAX_DEPTH ) return {};

    hit_record rec;
    if (world->Hit(r, 0.0001, INF, rec)) {
        double x = Random::NextNumber(-1,1);
        double s = cos((PI/2) * x);
        double y = Random::NextNumber(-s, s);
        double eq = sqrt(1 - x*x - y*y);
        double z = Random::NextNumber(-eq, eq);

        Point3 target = rec.p + rec.normal + Vec3(x, y, z);
        return 0.5 * _ray2Color_rec(Ray(rec.p, target - rec.p), ++depth);
    }
    Vec3 unit_direction = r.direction.unit();
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t) * WHITE + t * LIGHT_YELLOW;
}
