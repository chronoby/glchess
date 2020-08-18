#pragma once
#include "Transform.h"

enum MouseButton
{
    LEFT, RIGHT
};

class Mouse
{
private:
    
    bool leftbutton, rightbutton;
    int oldscreenx, oldscreeny;
    int currentscreenx, currentscreeny;
    float ROTATION_INCREMENT      = 0.2f;
	float TRANSLATION_INCREMENT   = 0.05f;
    void setOldPosition();
    
public:
	Mouse();
    int GetOldX()   { return oldscreenx; }
    int GetOldY()   { return oldscreeny; }
    int GetDeltaX() { return currentscreenx - oldscreenx; }
    int GetDeltaY() { return currentscreeny - oldscreeny; }
    void SetPosition(int x, int y);    
    bool HasMoved();
    void Down( MouseButton );
    void Up ( MouseButton );
    bool IsActive(); 
    void Update();
    void ClearUpdate();        
    void ApplyRotation();
    void ApplyTranslation();

	Transform MouseUpdate;
};