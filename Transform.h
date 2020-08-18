#pragma once
#include "defs.h"

struct Transform
{
    Vector Rotation;
    Vector Scaling;
    Vector Translation;

    Transform();
    Transform( const Transform &target );
    Transform( const Vector &rot, const Vector &scal, const Vector &trans) : Rotation(rot), Scaling(scal), Translation(trans) {};
   
    Transform operator+( const Transform &target );
    Transform operator-( const Transform &target );
};
