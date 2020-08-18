#include <stdio.h>
#include "Mouse.h"

Mouse::Mouse()
{
    leftbutton      = false;
    rightbutton     = false;    
    oldscreenx      = 0;
    oldscreeny      = 0;
    currentscreenx  = 0;
    currentscreeny  = 0;
}

void Mouse::setOldPosition()
{
    oldscreenx = currentscreenx;
    oldscreeny = currentscreeny;
}

void Mouse::SetPosition( int x, int y )
{
    setOldPosition();
    currentscreenx  = x;
    currentscreeny  = y;
}

bool Mouse::HasMoved()
{
    return ((currentscreenx - oldscreenx) != 0) || ((currentscreeny - oldscreeny) != 0);
}

void Mouse::Down( MouseButton whichButton )
{
    switch( whichButton )
    {
        case LEFT:	leftbutton  = true; break;
		case RIGHT: rightbutton = true; break;
    }
}
void Mouse::Up( MouseButton whichButton )
{
    switch( whichButton )
    {
        case LEFT:  leftbutton  = false; break;
		case RIGHT: rightbutton = false; break;
    }    
}
bool Mouse::IsActive()
{
	return leftbutton || rightbutton;
}

void Mouse::ApplyRotation()
{

    MouseUpdate.Rotation = Vector( GetDeltaX() * ROTATION_INCREMENT , GetDeltaY() * ROTATION_INCREMENT , 0.0f );
    printf("Rotation xy: %d %d\n", GetDeltaX(), GetDeltaY() );
	
}

void Mouse::ApplyTranslation()
{
    MouseUpdate.Translation = MouseUpdate.Translation + Vector( GetDeltaX() * TRANSLATION_INCREMENT , -GetDeltaY() * TRANSLATION_INCREMENT , 0.0f );
}

void Mouse::Update()
{
    if(leftbutton) ApplyTranslation();
    if(rightbutton) ApplyRotation();
}

void Mouse::ClearUpdate()
{
    MouseUpdate.Rotation = Vector( 0.0f, 0.0f, 0.0f );
}