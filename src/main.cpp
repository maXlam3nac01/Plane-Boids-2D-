#include <imgui.h>
#include "glm/gtc/constants.hpp"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <vector>
#include "../include/plane.hpp"
#include "doctest/doctest.h"

// FUNDAMENTALS PARAMETERS //

int   NUMBER_OF_PLANE = 10;
float MAX_SPEED       = 0.01f;

float       PLANE_HEIGHT             = 0.2f;
const float PLANE_WIDTH_HEIGHT_RATIO = 0.85545753f; // = width / height

const float PLANE_STROKE = 0.003f;

// BORDERS AVOIDING
float MARGIN                = 0.1f;
float BORDER_AVOID_STRENGTH = 0.1f;

// SEPARATION
float PRIVATE_ZONE          = 0.3f;
float EXTENDED_PRIVATE_ZONE = 0.6f;
float SEPARATION_STRENGTH   = 0.005;

// COHESION
float COHESION_DISTANCE = 0.5f;
float COHESION_STRENGTH = 0.0001;

// ALIGNMENT
float ALIGNMENT_DISTANCE = 0.5f;
float ALIGNMENT_STRENGTH = 0.0025;

void displayGUI()
{
    ImGui::Begin("Test");

    ImGui::SliderInt("NUMBER_OF_PLANE", &NUMBER_OF_PLANE, 1, 50);
    ImGui::SliderFloat("PLANE_SIZE", &PLANE_HEIGHT, 0.05f, 0.6f);
    ImGui::SliderFloat("MAX_SPEED", &MAX_SPEED, 0, 0.05);

    ImGui::SliderFloat("MARGIN", &MARGIN, 0.0f, 1.0f);
    ImGui::SliderFloat("BORDER_AVOID_STRENGTH", &BORDER_AVOID_STRENGTH, 0.0f, 0.02f);

    ImGui::SliderFloat("PRIVATE_ZONE", &PRIVATE_ZONE, 0.1f, 0.5f);
    ImGui::SliderFloat("EXTENDED_PRIVATE_ZONE", &EXTENDED_PRIVATE_ZONE, PRIVATE_ZONE, 1.5f);
    ImGui::SliderFloat("SEPARATION_STRENGTH", &SEPARATION_STRENGTH, 0, 0.02f);

    ImGui::SliderFloat("COHESION_DISTANCE", &COHESION_DISTANCE, 0.0f, 2.0f);
    ImGui::SliderFloat("COHESION_STRENGTH", &COHESION_STRENGTH, 0, 0.02f);

    ImGui::SliderFloat("ALIGNMENT_DISTANCE", &ALIGNMENT_DISTANCE, 0.0f, 2.0f);
    ImGui::SliderFloat("ALIGNMENT_STRENGTH", &ALIGNMENT_STRENGTH, 0, 0.02f);

    ImGui::End();
}

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

        displayGUI();

        // Update the planes' parameters with the new values from ImGui
        for (auto& plane : planes)
        {
            plane.set_speed(MAX_SPEED);
        }

        // Add or remove planes based on the value of NUMBER_OF_PLANE
        while (planes.size() < NUMBER_OF_PLANE)
        {
            glm::vec2 position = glm::vec2(p6::random::number(-Xmax, Xmax), p6::random::number(-Ymax, Ymax));
            float     speed    = p6::random::number(MAX_SPEED / 4, MAX_SPEED);
            float     angle    = p6::random::number(0, glm::two_pi<float>());
            planes.emplace_back(position, speed, angle, planes.size());
        }

        while (planes.size() > NUMBER_OF_PLANE)
        {
            planes.pop_back();
        }

        for (auto& plane : planes)
        {
            plane.draw(ctx, plane.get_angle(), PLANE_HEIGHT, PLANE_WIDTH_HEIGHT_RATIO, PLANE_STROKE);
            plane.move();
            plane.border(Xmax, Ymax, PLANE_HEIGHT, PLANE_WIDTH_HEIGHT_RATIO);
            plane.avoid_borders(Xmax, Ymax, MARGIN, BORDER_AVOID_STRENGTH);
            // no_border(Xmax, Ymax);
            for (auto& otherPlane : planes)
            {
                if (plane.get_id() != otherPlane.get_id())
                {
                    plane.separation(otherPlane, PRIVATE_ZONE, EXTENDED_PRIVATE_ZONE, SEPARATION_STRENGTH);
                    plane.cohesion(otherPlane, COHESION_DISTANCE, COHESION_STRENGTH);
                    plane.alignment(otherPlane, ALIGNMENT_DISTANCE, ALIGNMENT_STRENGTH);
                }
            }
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    return 0;
}