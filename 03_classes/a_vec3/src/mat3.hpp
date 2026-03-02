#pragma once

#include "vec3.hpp"

class mat3 {
public:
    float m[3][3];
    
    mat3(); // Constructeur par défaut (matrice identité)
    mat3(float diag); // Constructeur avec une valeur diagonale

    float operator()(int i, int j) const;
    float& operator()(int i, int j);

    void display() const;
};
