#include "csc3406.h"
#include <windows.h>
#include "gl.h"
#include "glu.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lodepng.h"

#define BITMAP_ID   0x4D42    // the universal bitmap ID


/*****************************************************************************
LoadBitmapFile()

Returns a pointer to the bitmap image of the bitmap specified by filename.
Also returns the bitmap header information. No support for 8-bit bitmaps.
*****************************************************************************/
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, int *components)
{
	FILE *filePtr;                        // the file pointer
	BITMAPFILEHEADER  bitmapFileHeader;   // bitmap file header
	unsigned char    *bitmapImage;        // bitmap image data
	unsigned int      imageIdx = 0;       // image index counter
	unsigned char     tempRGB;            // swap variable

	*components = 3;

	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// read the bitmap information header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
} // end LoadBitmapFile()

unsigned char* loadPNGFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, int *components)
{
	LodePNG::Decoder decoder; 
	std::vector<unsigned char> buffer; 
	std::vector<unsigned char> image; 

	LodePNG::loadFile(filename, buffer); //load the image file with given filename

	decoder.decode32(buffer, image); //decode the PNG

	unsigned long w = bitmapInfoHeader->biWidth = decoder.getWidth();
	unsigned long h = bitmapInfoHeader->biHeight = decoder.getHeight();
	*components = decoder.getInfo().colorChannels;

	unsigned char * buf;
	buf = new unsigned char[w*h*4];
	
	int x,y;
	for(y = 0; y < h; y++)
		for(x = 0; x < w*4; x+=4)
		{
			//get RGBA components
			unsigned int r = image[x + w * y * 4 + 0]; //red
			unsigned int g = image[x + w * y * 4 + 1]; //green
			unsigned int b = image[x + w * y * 4 + 2]; //blue
			unsigned int a = image[x + w * y * 4 + 3]; //alpha
			
			buf[x + w * y * 4] = r;
			buf[x + 1 + w * y * 4] = g;
			buf[x + 2 + w * y * 4] = b;
			buf[x + 3 + w * y * 4] = a;
		}

	
	return buf;
}

void FileReadWriter::writeLogFile(char *fileName, char *message)
{
	FILE *log;
	if ((log = fopen(fileName,"w")) == (FILE *) NULL)
	{
		return;		//couldn't write to log file
	}
	fprintf(log,"%s\n", message);
	fclose(log);
}


void FileReadWriter::loadSetupFile(char *fileName, WindowSetup *settings)
{
	FILE *setup;
	int valuesRead;

	//use default settings
	settings->SCREENWIDTH = 640;
	settings->SCREENHEIGHT= 480;

	settings->VIEW_LEFT = settings->WORLD_LEFT = 0;
	settings->VIEW_RIGHT = settings->WORLD_RIGHT = settings->SCREENWIDTH;
	settings->VIEW_TOP = settings->WORLD_TOP = 0;
	settings->VIEW_BOTTOM = settings->WORLD_BOTTOM = settings->SCREENHEIGHT;

	settings->ASPECT_RATIO = fabs((float)(settings->WORLD_RIGHT - settings->WORLD_LEFT)/(settings->WORLD_TOP - settings->WORLD_BOTTOM));

	if ((setup = fopen("setup.ini","r")) != (FILE *) NULL)
	{
		valuesRead = fscanf(setup,"%d %d", &settings->SCREENWIDTH, &settings->SCREENHEIGHT);
		valuesRead += fscanf(setup,"%d", &settings->WORLD_LEFT);
		valuesRead += fscanf(setup,"%d", &settings->WORLD_RIGHT);
		valuesRead += fscanf(setup,"%d", &settings->WORLD_TOP);
		valuesRead += fscanf(setup,"%d", &settings->WORLD_BOTTOM);

		if(valuesRead != 6)
			return;

		settings->VIEW_LEFT = settings->WORLD_LEFT;
		settings->VIEW_RIGHT = settings->WORLD_RIGHT;
		settings->VIEW_TOP = settings->WORLD_TOP;
		settings->VIEW_BOTTOM = settings->WORLD_BOTTOM;
		settings->ASPECT_RATIO = fabs((float)(settings->WORLD_RIGHT - settings->WORLD_LEFT)/(settings->WORLD_TOP - settings->WORLD_BOTTOM));

		fclose(setup);
		return;
	}
}

void FileReadWriter::captureScreen(int width, int height, char *filename)
{
  //try to open file
  FILE *file;
  if( (file = fopen(filename, "wb")) == (FILE *) NULL)
	  return;

  //create file headers for windows bitmap file
  BITMAPFILEHEADER bf;
  BITMAPINFOHEADER bi;

  //create memory to store the pixels of the image in
  unsigned char *image = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);

  if( image!=NULL )  //if image memory buffer was created
  {
	//read all the pixels from the graphics buffer into image
	glReadPixels( 0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image );
	memset( &bf, 0, sizeof( bf ) );
	memset( &bi, 0, sizeof( bi ) );

	//setup some windows bitmap specific values for the file
	bf.bfType = 'MB';
	bf.bfSize = sizeof(bf)+sizeof(bi)+width*height*3;
	bf.bfOffBits = sizeof(bf)+sizeof(bi);
	bi.biSize = sizeof(bi);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biSizeImage = width*height*3;

	//write the headers and data  into the file
	fwrite( &bf, sizeof(bf), 1, file );
	fwrite( &bi, sizeof(bi), 1, file );
	fwrite( image, sizeof(unsigned char), height*width*3, file );
	fclose( file );
	free( image );
  }
}

/*****************************************************************************
loadTextures()

Loads the data for the water and land textures and stores them in texture
objects.  Returns the value of OpenGL id of the texture.
*****************************************************************************/
int FileReadWriter::loadTextures(char *filename)
{
	   
	unsigned char *textureData;    //texture data
	unsigned int texId;
	BITMAPINFOHEADER  bitmapInfo; 
	int colourChannels;

	if(strstr(filename,".bmp") || strstr(filename,".BMP"))
	{
		// load the texture data
		
		textureData = LoadBitmapFile(filename, &bitmapInfo, &colourChannels);
		if (!textureData)
			return -1;
	}
	else if(strstr(filename,".png") || strstr(filename,".PNG"))
	{
		textureData = loadPNGFile(filename, &bitmapInfo, &colourChannels);
	}

	// generate the texture as a mipmap
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if(colourChannels == 4)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bitmapInfo.biWidth, bitmapInfo.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bitmapInfo.biWidth, bitmapInfo.biHeight, GL_RGB, GL_UNSIGNED_BYTE, textureData);


	return texId;
} // end LoadTextures()
