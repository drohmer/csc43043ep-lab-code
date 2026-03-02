#include <iostream>
#include <cmath>

#include "vec3.hpp"



vec3::vec3() : 
    x(0.0f), y(0.0f), z(0.0f) 
{}
vec3::vec3(float x_arg, float y_arg, float z_arg) : 
    x(x_arg), y(y_arg), z(z_arg) 
{}
void vec3::display() const {
    std::cout << "(" << x << "," << y << "," << z << ")\n";
}
