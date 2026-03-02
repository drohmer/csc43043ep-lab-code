#include <iostream>
#include "mat3.hpp"

mat3::mat3() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

mat3::mat3(float diag) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m[i][j] = (i == j) ? diag : 0.0f;
        }
    }
}

float mat3::operator()(int i, int j) const {
    return m[i][j];
}

float& mat3::operator()(int i, int j) {
    return m[i][j];
}

void mat3::display() const {
    for (int i = 0; i < 3; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 3; ++j) {
            std::cout << m[i][j] << " ";
        }
        std::cout << "|\n";
    }
}
