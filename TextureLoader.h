#pragma once
extern int num_texture;

extern int LoadBitmap(const char *filename);

typedef struct                     
    {
    unsigned int   biSize;         
    int            biWidth;        
    int            biHeight;       
    unsigned short biPlanes;       
    unsigned short biBitCount;     
    unsigned int   biCompression;  
    unsigned int   biSizeImage;    
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;      
    unsigned int   biClrImportant; 
    char *data;
} BITMAPINFOHEADER;
