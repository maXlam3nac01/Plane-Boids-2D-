#include "../include/plane.hpp"
#include <cmath>
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"

void Plane::move()
{
    glm::vec2 new_position = glm::vec2(
        this->get_position()[0] + this->get_speed() * glm::cos(this->get_angle()),
        this->get_position()[1] + this->get_speed() * glm::sin(this->get_angle())
    );
    this->set_position(new_position);
}

void Plane::border(float Xmax, float Ymax)
{
    if (this->get_position()[0] < -Xmax)
    {
        glm::vec2 position = glm::vec2(-Xmax, this->get_position()[1]);
        this->set_position(position);
        this->set_speed(-get_speed());
        this->set_angle(-get_angle());
    }
    if (this->get_position()[0] > Xmax)
    {
        glm::vec2 position = glm::vec2(Xmax, this->get_position()[1]);
        this->set_position(position);
        this->set_speed(-get_speed());
        this->set_angle(-get_angle());
    }
    if (this->get_position()[1] < -Ymax)
    {
        glm::vec2 position = glm::vec2(this->get_position()[0], -Ymax);
        this->set_position(position);
        this->set_speed(-get_speed());
        this->set_angle(glm::pi<float>() - get_angle());
    }
    if (this->get_position()[1] > Ymax)
    {
        glm::vec2 position = glm::vec2(this->get_position()[0], Ymax);
        this->set_position(position);
        this->set_speed(-get_speed());
        this->set_angle(glm::pi<float>() - get_angle());
    }
}

void Plane::no_border(float Xmax, float Ymax)
{
    if (this->get_position()[0] < -Xmax || this->get_position()[0] > Xmax)
    {
        glm::vec2 position = glm::vec2(-1 * this->get_position()[0], this->get_position()[1]);
        this->set_position(position);
    }
    if (this->get_position()[1] < -Ymax || this->get_position()[1] > Ymax)
    {
        glm::vec2 position = glm::vec2(this->get_position()[0], -1 * this->get_position()[1]);
        this->set_position(position);
    }
}

void Plane::turn(float direction)
{
    this->set_angle(this->get_angle() + direction);
}

float distance(Plane Plane1, Plane Plane2)
{
    float distanceX = Plane1.get_position()[0] - Plane2.get_position()[0];
    float distanceY = Plane1.get_position()[1] - Plane2.get_position()[1];
    float distance  = std::sqrt(distanceX * distanceX + distanceY * distanceY);
    return distance;
}

// LES 3 RÈGLES DE BASE

void Plane::separation(Plane plane, Plane otherPlane, const float& private_zone, const float& extented_private_zone, const float& separation_strength)
{
    float dist      = distance(plane, otherPlane);
    float direction = plane.get_angle() - otherPlane.get_angle();
    if (dist < private_zone)
    {
        plane.turn(separation_strength * direction / (dist * 20));
    }
    if (dist < extented_private_zone)
    {
        otherPlane.turn(separation_strength * direction / (dist * 20));
    }
}

void Plane::cohesion(Plane& plane, Plane& otherPlane)
{
}