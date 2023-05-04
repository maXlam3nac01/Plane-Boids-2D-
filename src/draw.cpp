#include <p6/p6.h>
#include <vcruntime.h>
#include <iostream>
#include "../include/plane.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"

const std::vector<glm::vec2> PlanePoint = {
    {0.f, 1.f},
    {0.0690f, 0.9882f},
    {0.1034f, 0.9440f},
    {0.1293f, 0.8850f},
    {0.1500f, 0.7847f},
    {0.1793f, 0.7448f},
    {0.1966f, 0.6755f},
    {0.2000f, 0.5800f},
    {0.2207f, 0.5133f},
    {0.2310f, 0.4277f},
    {0.2328f, 0.3540f},
    {0.9690f, -0.0118f},
    {1.f, -0.0649f},
    {1.f, -0.2655f},
    {0.9793f, -0.3068f},
    {0.9586f, -0.2950f},
    {0.2586f, -0.1209f},
    {0.2138f, -0.3982f},
    {0.1828f, -0.6342f},
    {0.5207f, -0.8053f},
    {0.5690f, -0.8818f},
    {0.5517f, -1.f},
    {0.1034f, -0.8584f},
    {0.0517f, -0.8968f},
    {0.f, -0.9056f}

};

void Plane::draw(p6::Context& ctx, float angle, const float plane_heigt, const float plane_width_height_ratio, const float plane_stroke)
{
    float x           = this->get_position()[0];
    float y           = this->get_position()[1];
    float half_height = plane_heigt / 2;
    float half_width  = plane_width_height_ratio * plane_heigt / 2;

    ctx.stroke_weight = plane_stroke;

    for (size_t i = 0; i < PlanePoint.size() - 1; i++)
    {
        ctx.line(
            {x + PlanePoint[i][0] * half_width, y + PlanePoint[i][1] * half_height},
            {x + PlanePoint[i + 1][0] * half_width, y + PlanePoint[i + 1][1] * half_height}
        );
    }
    for (size_t i = PlanePoint.size() - 1; i > 0; i--)
    {
        ctx.line(
            {x - PlanePoint[i][0] * half_width, y + PlanePoint[i][1] * half_height},
            {x - PlanePoint[i - 1][0] * half_width, y + PlanePoint[i - 1][1] * half_height}
        );
    }
}
