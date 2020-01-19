#include "..\include\freeglut.h"
#include "shapes.h"
#include <string>
#include "colors.h"

int exhaustP = 0; //car exhaust flame animation variable

// A stipple pattern of a fly
const GLubyte fly[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x80, 0x01, 0xC0, 0x06, 0xC0, 0x03, 0x60,
	0x04, 0x60, 0x06, 0x20, 0x04, 0x30, 0x0C, 0x20,
	0x04, 0x18, 0x18, 0x20, 0x04, 0x0C, 0x30, 0x20,
	0x04, 0x06, 0x60, 0x20, 0x44, 0x03, 0xC0, 0x22,
	0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
	0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
	0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
	0x66, 0x01, 0x80, 0x66, 0x33, 0x01, 0x80, 0xCC,
	0x19, 0x81, 0x81, 0x98, 0x0C, 0xC1, 0x83, 0x30,
	0x07, 0xe1, 0x87, 0xe0, 0x03, 0x3f, 0xfc, 0xc0,
	0x03, 0x31, 0x8c, 0xc0, 0x03, 0x33, 0xcc, 0xc0,
	0x06, 0x64, 0x26, 0x60, 0x0c, 0xcc, 0x33, 0x30,
	0x18, 0xcc, 0x33, 0x18, 0x10, 0xc4, 0x23, 0x08,
	0x10, 0x63, 0xC6, 0x08, 0x10, 0x30, 0x0c, 0x08,
	0x10, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x08,
};

void drawGrid(int length, int height, int startX, int startY, int gap, float* color, bool lines)
{
	/* define object to be drawn as a square polygon */
	glPushAttrib(GL_ENABLE_BIT);
	glColor3f(color[0], color[1], color[2]);

	if (lines) {	//if grid pattern
		glLineStipple(1, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		//horizontal lines
		for (size_t i = startX; i < length - gap; i = i + gap)
		{

			glBegin(GL_LINES);
			glVertex2f(i + gap, startY);
			glVertex2f(i + gap, startY + height);
			glEnd();
		}

		//vertical lines
		for (size_t i = startY; i < height; i = i + gap)
		{
			glBegin(GL_LINES);
			glVertex2f(startX, i + gap);
			glVertex2f(startX + length, i + gap);
			glEnd();
		}

		glDisable(GL_LINE_STIPPLE);
	}
	else //if texture
	{
		glLoadIdentity();
		
		glEnable(GL_POLYGON_STIPPLE);		// Enable POLYGON STIPPLE
		glPolygonStipple(fly);
		//horizontal lines
		for (size_t i = startY; i < height; i = i + gap)
		{
			glPushMatrix();
			glRectf(startX, (i-5)+ gap, startX + length, i+gap+10);
			glPopMatrix();			
		}
		glDisable(GL_POLYGON_STIPPLE);	
	}

	glPopAttrib();
}

void drawFlame(GLfloat centreX, GLfloat centreY, float* color)
{
	glColor3f(color[0], color[1], color[2]);

	glBegin(GL_POINTS);

	glVertex2f(centreX - 2, centreY - 1 - exhaustP); //exhaustP will change with the time for the animation
	glVertex2f(centreX + 2, centreY - 1 - exhaustP);
	glVertex2f(centreX, centreY - 2 - exhaustP);

	glEnd();
}

void drawCarExhaust(GLfloat centreX, GLfloat centreY)
{
	drawRectangle(8.0f, 5.0f, centreX, centreY, grey);
	drawFlame(centreX, centreY - 3, red);
	drawFlame(centreX, centreY - 4, white);
	drawFlame(centreX, centreY - 5, red);
}



void drawTires(GLfloat centreX, GLfloat centreY)
{
	drawRectangle(3.0f, 10.0f, centreX - 20 , centreY + 10, white);
	drawRectangle(3.0f, 10.0f, centreX + 20, centreY + 10, white);
	drawRectangle(3.0f, 10.0f, centreX - 20, centreY - 10, white);
	drawRectangle(3.0f, 10.0f, centreX + 20, centreY - 10, white);
}

void drawCar(GLfloat centreX, GLfloat centreY)
{
	
	drawRectangle(35.0f, 45.0f, centreX, centreY, grey);
	drawSquare(20.0f, centreX, centreY + 5.0f, white);
	drawCarExhaust(centreX - 5, centreY - 25);
	drawTires(centreX, centreY);
}

void drawRectangle(GLfloat length, GLfloat height, GLfloat centreX, GLfloat centreY, float* color)
{
	glColor3f(color[0], color[1], color[2]);

	glBegin(GL_POLYGON);
	glVertex2f(centreX - length / 2, centreY - height / 2);
	glVertex2f(centreX - length / 2, centreY + height / 2);
	glVertex2f(centreX + length / 2, centreY + height / 2);
	glVertex2f(centreX + length / 2, centreY - height / 2);
	glEnd();
}

void drawSquare(GLfloat length, GLfloat centreX, GLfloat centreY, float *color)
{
	glColor3f(color[0], color[1], color[2]);

	glBegin(GL_POLYGON);
	glVertex2f(centreX - length / 2, centreY - length / 2);
	glVertex2f(centreX - length / 2, centreY + length / 2);
	glVertex2f(centreX + length / 2, centreY + length / 2);
	glVertex2f(centreX + length / 2, centreY - length / 2);
	glEnd();
}

void drawBoxLine(int width, int height, float *color, int x, int y)
{
	glColor3f(color[0], color[1], color[2]);
	  

	glBegin(GL_LINE_LOOP);
	glVertex2f(x - (width / 2), y - (height / 2));
	glVertex2f(x + (width / 2), y - (height / 2));
	glVertex2f(x + (width / 2), y + (height / 2));
	glVertex2f(x - (width / 2), y + (height / 2));
	glEnd();
}
