#include "glm/gtc/constants.hpp"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <vector>
#include "../include/plane.hpp"
#include "doctest/doctest.h"

const int   NUMBER_OF_PLANE          = 10;
const float MAX_SPEED                = 0.005f;
const float PRIVATE_ZONE             = 0.3f;
const float EXTENDED_PRIVATE_ZONE    = 0.6;
const float PLANE_HEIGHT             = 0.2f;
const float PLANE_width_height_ratio = 0.85545753f; // = width / height
const float PLANE_STROKE             = 0.003f;

const float SEPARATION_STRENGHT = 0.05;
const float COHESION_STRENGHT   = 0.005;
const float ALIGNMENT_STRENGHT  = 0.0025;

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;

        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Aller, on va y arriver, j'y crois, (il faut)"}};
    // ctx.maximize_window();

    const float Xmax = ctx.aspect_ratio();
    const float Ymax = 1.f;

    std::vector<Plane> planes;
    for (int i = 0; i < NUMBER_OF_PLANE; i++)
    {
        glm::vec2 position = glm::vec2(p6::random::number(-Xmax, Xmax), p6::random::number(-Ymax, Ymax));
        float     speed    = p6::random::number(MAX_SPEED / 4, MAX_SPEED);
        float     angle    = p6::random::number(0, glm::two_pi<float>());
        planes.emplace_back(position, speed, angle, i);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::BabyBlue);

        for (auto& plane : planes)
        {
            plane.move();
            plane.draw(ctx, plane.get_angle());
            plane.border(Xmax, Ymax);
            // plane.no_border(Xmax, Ymax);
            for (auto& otherPlane : planes)
            {
                if (plane.get_id() != otherPlane.get_id())
                {
                    plane.separation(plane, otherPlane, PRIVATE_ZONE, EXTENDED_PRIVATE_ZONE, SEPARATION_STRENGHT);
                }
            }
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    return 0;
}