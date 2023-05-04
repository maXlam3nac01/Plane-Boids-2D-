#pragma once

#include <vector>
#include "p6/p6.h"

class Plane {
private:
    glm::vec2    m_position; // center of the plane
    float        m_speed;
    float        m_angle;
    unsigned int m_id;

public:
    // Constructors
    Plane() = default;

    ~Plane() = default;

    Plane(const glm::vec2 position, const float speed, const float angle, const unsigned int id)
        : m_position(position), m_speed(speed), m_angle(angle), m_id(id) {}

    // Getter and Setter
    inline glm::vec2    get_position() const { return this->m_position; };
    inline float        get_speed() const { return this->m_speed; };
    inline float        get_angle() const { return this->m_angle; };
    inline unsigned int get_id() const { return this->m_id; };

    inline void set_position(const glm::vec2 position) { this->m_position = position; };
    inline void set_speed(const float speed) { this->m_speed = speed; };
    inline void set_angle(const float angle) { this->m_angle = angle; };
    inline void set_id(const unsigned int id) { this->m_id = id; };

    // Methods
    void draw(p6::Context& ctx, float angle_r, float plane_heigt, float plane_width_height_ratio, float plane_stroke);

    void move();

    void no_border(float Xmax, float Ymax);

    void border(float Xmax, float Ymax, const float plane_height, const float plane_width_height_ratio);

    void avoid_borders(float Xmax, float Ymax, float margin, float border_avoid_strength);

    void separation(Plane otherPlane, const float& private_zone, const float& extented_private_zone, const float& separation_strength);

    void cohesion(Plane& otherPlane, float cohesion_distance, float cohesion_strength);

    void alignment(const Plane& otherPlane, float alignment_distance, float alignment_strength);
};