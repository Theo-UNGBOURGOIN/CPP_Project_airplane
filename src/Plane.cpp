#include "Plane.hpp"

Plane::Plane(const float& trajectoire, const float& altitude, const float& speed) : trajectoire_(trajectoire), altitude_(altitude), speed_(speed) {
    /*initial_clock_ = high_resolution_clock::now();
    update();
    t_ = std::thread(update_plane, std::ref(*this), std::ref(stop_thread_));*/
}

float Plane::get_x() {
    return x_;
}

float Plane::get_y() {
    return y_;
}

void Plane::update()
{
    auto current_clock = high_resolution_clock::now();
    auto dt = (current_clock - initial_clock_).count() / 1e8;
    initial_clock_ = current_clock;
}

Plane::~Plane()
{
    stop_thread_ = true;
    t_.join();
}

void update_plane(Plane& plane, bool& stop_thread)
{
    while (!stop_thread)
    {
        std::this_thread::sleep_for(100ms);
        plane.update();
    }
}