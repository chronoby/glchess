#include "Transform.h"

Transform::Transform()
{
    Rotation    = Vector(0.0f, 0.0f, 0.0f);
    Scaling     = Vector(0.0f, 0.0f, 0.0f);
    Translation = Vector(0.0f, 0.0f, 0.0f);
}
Transform::Transform( const Transform &target )
{
    Rotation    = target.Rotation;
    Scaling     = target.Scaling;
    Translation = target.Translation;
}

Transform Transform::operator+( const Transform &target )
{
    return Transform( Rotation + target.Rotation,
                        Scaling  + target.Scaling,
                        Translation + target.Translation );
}

Transform Transform::operator-( const Transform &target )
{
    return Transform( Rotation - target.Rotation,
                        Scaling  - target.Scaling,
                        Translation - target.Translation );
}
