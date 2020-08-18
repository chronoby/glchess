#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../glut/glut.h"
#include "../glut/glu.h"
#include "TextureLoader.h"        


#pragma warning (disable: 4996)
int num_texture=0;

int LoadBitmap(const char *filename)
{
    FILE * file;
    char temp;
    long i;

    BITMAPINFOHEADER infoheader;

    num_texture++;

    if( (file = fopen(filename, "rb"))==NULL) return (-1);

    fseek(file, 18, SEEK_CUR);
    fread(&infoheader.biWidth, sizeof(int), 1, file);

    fread(&infoheader.biHeight, sizeof(int), 1, file);

    fread(&infoheader.biPlanes, sizeof(short int), 1, file);
    if (infoheader.biPlanes != 1) {
	    printf("Planes from %s is not 1: %u\n", filename, infoheader.biPlanes);
	    return 0;
    }

    fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
    if (infoheader.biBitCount != 24) {
      printf("Bpp from %s is not 24: %d\n", filename, infoheader.biBitCount);
      return 0;
    }

    fseek(file, 24, SEEK_CUR);

    infoheader.data = (char *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
    if (infoheader.data == NULL) {
	    printf("Error allocating memory for color-corrected image data\n");
	    return 0;
    }

    if ((i = fread(infoheader.data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
	    printf("Error reading image data from %s.\n", filename);
	    return 0;
    }

    for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { 
	    temp = infoheader.data[i];
	    infoheader.data[i] = infoheader.data[i+2];
	    infoheader.data[i+2] = temp;
    }


    fclose(file);


    glBindTexture(GL_TEXTURE_2D, num_texture);

   
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND); 

    glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);
    
    free(infoheader.data);

    return (num_texture);
}
