#pragma once
#include "math.h"

struct Vector
{
    Vector( ) : x(0.0f) , y(0.0f) , z(0.0f) , h(1.0f) { }
    Vector( float _x , float _y , float _z , float _h ) : x(_x), y(_y), z(_z), h(_h) { }
    Vector( float _x , float _y , float _z )  : x(_x) , y(_y) , z(_z) , h(1.0f) {}
    Vector( const Vector &target );
    Vector( const float coords[] );

    bool operator==( const Vector &target );
    bool operator!=( const Vector &target );

    Vector operator*( const float k ) const;
    Vector operator/( const float k ) const;
    Vector CrossProduct( const Vector &target ) const;
    Vector operator|( const Vector &target ) const;
    Vector operator+( const Vector &target ) const;
    Vector operator-( const Vector &target ) const;
    Vector ComponentWise( const Vector &target ) const;
    Vector operator*( const Vector &target ) const;
    float DotProduct( const Vector &target ) const;
    float operator^( const Vector &target ) const;

    Vector Normal() const;
    void Normalize();
    const float Length() const { return sqrt( LengthSquared() ); }
    const float LengthSquared() const { return x*x + y*y + z*z; }
    const float InvLength() const { return 1.0 / sqrt( LengthSquared() ); }
    union
    {
        struct
        {
            float x,y,z,h;
        };
        float Vec3[3];
    };
};