#include "../include/plane.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/vector_angle.hpp"

float distance(Plane plane1, Plane plane2)
{
    float distanceX = plane1.get_position()[0] - plane2.get_position()[0];
    float distanceY = plane1.get_position()[1] - plane2.get_position()[1];
    float distance  = glm::sqrt(distanceX * distanceX + distanceY * distanceY);
    return distance;
}

void Plane::border(float Xmax, float Ymax, const float plane_height, const float plane_width_height_ratio)
{
    float half_width  = plane_width_height_ratio * plane_height / 2;
    float half_height = plane_height / 2;

    bool angle_changed = false;

    for (int i = 0; i < 2; ++i)
    {
        float  max_value = (i == 0) ? Xmax : Ymax;
        float  half_size = (i == 0) ? half_width : half_height;
        float& position  = m_position[i];
        float& angle     = (i == 0) ? m_angle : m_angle;

        if (position < -max_value + half_size)
        {
            position      = -max_value + half_size;
            angle         = (i == 0) ? glm::pi<float>() - angle : -angle;
            angle_changed = true;
        }
        else if (position > max_value - half_size)
        {
            position      = max_value - half_size;
            angle         = (i == 0) ? glm::pi<float>() - angle : -angle;
            angle_changed = true;
        }
    }

    // Normalize the angle between 0 and 2*pi
    if (angle_changed)
    {
        while (m_angle < 0)
            m_angle += glm::two_pi<float>();
        while (m_angle > glm::two_pi<float>())
            m_angle -= glm::two_pi<float>();
    }
}

void Plane::no_border(float Xmax, float Ymax)
{
    for (int i = 0; i < 2; ++i)
    {
        float coordinate = this->get_position()[i];
        float max_value  = (i == 0) ? Xmax : Ymax;

        if (coordinate < -max_value || coordinate > max_value)
        {
            glm::vec2 position = this->get_position();
            position[i]        = -1 * coordinate;
            this->set_position(position);
        }
    }
}

//* ATTRIBUTES *//

// to see draw function go see draw.cpp file

void Plane::move()
{
    glm::vec2 new_position = glm::vec2(
        this->get_position()[0] + this->get_speed() * glm::cos(this->get_angle()),
        this->get_position()[1] + this->get_speed() * glm::sin(this->get_angle())
    );
    this->set_position(new_position);
}

void Plane::avoid_borders(float Xmax, float Ymax, float margin, float border_avoid_strength)
{
    glm::vec2 new_direction{0.f, 0.f};

    if (this->get_position()[0] <= -Xmax + margin)
    {
        new_direction += glm::vec2(border_avoid_strength, 0.f);
    }
    if (this->get_position()[0] >= Xmax - margin)
    {
        new_direction += glm::vec2(-border_avoid_strength, 0.f);
    }
    if (this->get_position()[1] <= -Ymax + margin)
    {
        new_direction += glm::vec2(0.f, border_avoid_strength);
    }
    if (this->get_position()[1] >= Ymax - margin)
    {
        new_direction += glm::vec2(0.f, -border_avoid_strength);
    }

    if (glm::length(new_direction) > 0)
    {
        new_direction = glm::normalize(new_direction);
        m_angle += glm::orientedAngle(glm::vec2(std::cos(m_angle), std::sin(m_angle)), new_direction);
        m_position += new_direction * m_speed;
    }
}

// LES 3 RÈGLES DE BASE

void Plane::separation(Plane otherPlane, const float& private_zone, const float& extented_private_zone, const float& separation_strength)
{
    float dist = distance(*this, otherPlane);

    if (dist < private_zone || dist < extented_private_zone)
    {
        // Calculer la direction de l'autre avion par rapport à l'avion actuel
        float direction_x = this->m_position[0] - otherPlane.get_position()[0];
        float direction_y = this->m_position[1] - otherPlane.get_position()[1];

        // Normaliser la direction
        float direction_magnitude = glm::sqrt(direction_x * direction_x + direction_y * direction_y);
        direction_x /= direction_magnitude;
        direction_y /= direction_magnitude;

        // Calculer l'angle de la direction
        float direction_angle = glm::atan(direction_y, direction_x);

        if (dist < extented_private_zone)
        {
            // Ajuster l'angle de l'autre avion pour s'éloigner de l'avion actuel
            otherPlane.set_angle(otherPlane.get_angle() + separation_strength * direction_angle);
        }

        if (dist < private_zone)
        {
            // Ajuster l'angle de l'avion actuel pour s'éloigner de l'autre avion
            this->set_angle(this->get_angle() + separation_strength * direction_angle);
        }
    }
}

void Plane::cohesion(Plane& otherPlane, float cohesion_distance, float cohesion_strength)
{
    float distance_to_other = distance(*this, otherPlane);

    if (distance_to_other < cohesion_distance)
    {
        // Calculer la direction moyenne entre les deux avions
        float average_direction = (this->get_angle() + otherPlane.get_angle()) / 2.0f;

        // Calculer la différence d'angle entre la direction moyenne et la direction actuelle de l'avion
        float angle_difference = average_direction - this->get_angle();

        // Ajuster progressivement l'angle de l'avion pour qu'il s'aligne sur la direction moyenne
        this->set_angle(this->get_angle() + cohesion_strength * angle_difference);
    }
}

void Plane::alignment(const Plane& otherPlane, float alignment_distance, float alignment_strength)
{
    float distance = glm::distance(this->get_position(), otherPlane.get_position());

    if (distance < alignment_distance)
    {
        float angle_difference     = otherPlane.get_angle() - this->get_angle();
        float alignment_adjustment = angle_difference * alignment_strength;

        this->set_angle(this->get_angle() + alignment_adjustment);
    }
}