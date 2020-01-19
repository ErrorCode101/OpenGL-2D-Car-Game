/*
 5CCGD003W -Coursework 1 - 2D Car Game
*/
// Code by M. M. Aadhil

#include "include\freeglut.h"	// OpenGL toolkit - in the local shared folder
#include <iostream>
#include "2DCarGame\shapes.h"
#include "2DCarGame\colors.h"
#include "2DCarGame\metafunctions.h"
#include <string>
#include <list>
#include <vector> 
#include <iterator> 
#include <time.h> 
#include <algorithm>

//constants
const GLsizei width = 600, height = 600; // window width/ height pixel resoultion
const int border[4] = { 25, 575, 50, 550 }; //border area (left, right, bottom, top)
const int midPoint[2] = { 300, 300 }; //centre of the area (x, y)
const int carExitGate[4] = { 500, 550, 100, 25 }; //car exit gate (x, y, width, height)
const int carEnranceGate[4] = { 100, 50, 100, 25 }; //car entance gate (x, y, width, height)


std::vector<std::vector<int>> hazardBlocks; // all hazard blocks positions will be stored 
bool gridLayout = true; //grid or texture layout
int score = 0; //recorded score

int carPosition[2] = { 100, 100 }; //real time car coordinates (x, y)
int carAcceleration = 5; //car accelaration speed on key press
char carDirection = 'T'; //current turned direction of the car
int carPrePosition[2] = { 100, 100 }; //previous position of the car
GLfloat carRotation = 0.0f; //car rotation counter clock wise

bool collided = false; //true if car has been collided in to something
int gapSize = 50; //size of the blocks
int blockCount = 5; //hazard block count
bool holdR = false;

void carRotate(char direction);
void resetGame();



bool checkBoxCollider(std::vector<int> sourcePoint, float* souceSize, std::vector<int> targetPoint, float* targetSize)
{
	float minXDistance = (souceSize[0]/2) + (targetSize[1]/2);
	float minYDistance = (souceSize[1]/ 2) + (targetSize[1]/ 2);
	int xDistance = abs(targetPoint[0] - sourcePoint[0]), yDistance = abs(targetPoint[1] - sourcePoint[1]);

	if (minXDistance >= xDistance && minYDistance >= yDistance) 
	{
		return true;
	}
	return false;
}


bool checkCollisionOnBlocks(std::vector<int> point, float pointSize)
{
	float size[2] = { pointSize, pointSize };
	for each (std::vector<int> loc in hazardBlocks)
	{
		if (checkBoxCollider(loc, size, point, size)) return true;

	}
	return false;
}

bool checkCollisionOnBorder(std::vector<int> point, float* pointSize)
{
	if (point[0] < border[0] + gapSize/2 || point[0] > border[1] - gapSize/2 || point[1] < border[2] + gapSize/2 || point[1] > border[3] - gapSize/2)
	{
		return true;
	}
	return false;
}

bool blockingTheEntrnceOrExit(std::vector<int> point, float* pointSize)
{
	std::vector<int> exit = { carExitGate[0], carExitGate[1] };
	float exitSize[2] = { carExitGate[2], 3* gapSize };
	std::vector<int> entrance = { carEnranceGate[0], carEnranceGate[1] };
	float entranceSize[2] = { carEnranceGate[2], 3* gapSize };
	if (checkBoxCollider(exit, exitSize, point, pointSize) || checkBoxCollider(entrance, entranceSize, point, pointSize))
	{
		return true;
	}
	return false;
}

bool checkExit(std::vector<int> point, float* pointSize)
{
	std::vector<int> targetPoint = {carExitGate[0], carExitGate[1]};
	float targetSize[2] = { carExitGate[2], carExitGate[3] };
	if (checkBoxCollider(point, pointSize, targetPoint, targetSize))
	{
		return true;
	}
	return false;
}

void setHazardBlocks()
{
	hazardBlocks = {}; 
	bool validPoint;
	std::vector<int> tempPoint;
	std::vector<float> color;
	std::vector<int> car = { carPosition[0], carPosition[1] };
	float size[2] = { 50, 50 };
	for (int i = 0; i < blockCount; i++)
	{
		validPoint = false;
		tempPoint = { 0 , 0 };
		color = getRadomColorAvoid(0, 0, 0); //get a random primitive color avoiding black
		while (!validPoint) { //loop until getting a valid point to draw block
			tempPoint[0] = getRandomNumber(border[1] - gapSize, border[0]  + gapSize);
			tempPoint[1] = getRandomNumber(border[3] - gapSize, border[2] + gapSize);
			if (!checkCollisionOnBlocks(tempPoint, size[0]) && !blockingTheEntrnceOrExit(tempPoint, size)) { //check wether blocks collide with another block or blocking the entrance or exit
				validPoint = true; 
			};
		}
		
		tempPoint.push_back(color[0]);
		tempPoint.push_back(color[1]);
		tempPoint.push_back(color[2]);
		hazardBlocks.push_back(tempPoint);
	}
}

void collisionDetection()
{
	std::vector<int> carPoint = { carPosition[0], carPosition[1]};
	float carSize[2] = { 50 , 50 };

	if (checkCollisionOnBlocks(carPoint, 46.5)) {
		if (score > 0 && !collided) { score -= 10; }; //if its a collided for the dirst time reduce points
		collided = true;
		carPosition[0] = carPrePosition[0]; //taking the car poisiton to previous position
		carPosition[1] = carPrePosition[1];
		return; //breaking the method if collided
	}
	collided = false;
	//check whether car has reached Exit
	if (checkExit(carPoint, carSize))
	{
		resetGame();
	}

	//check wehter car is colliding with the border
	if (checkCollisionOnBorder(carPoint, carSize))
	{
		carPosition[0] = carPrePosition[0];
		carPosition[1] = carPrePosition[1];
	}

	carPrePosition[0] = carPosition[0]; //saving the previous car position if no collision has happned
	carPrePosition[1] = carPosition[1];
}

void carRotate(char direction)
{
	//rotate the car to the direction
	switch (direction)
	{
	case 'R': 
		if (carDirection == 'T' || carDirection == 'B') carRotation = 270; carDirection = 'R';
		break;
	case 'L':
		if (carDirection == 'T' || carDirection == 'B') carRotation = 90; carDirection = 'L';
		break;
	case 'T':
		if (carDirection == 'L' || carDirection == 'R') carRotation = 0; carDirection = 'T';
		break;
	case 'B':
		if (carDirection == 'L' || carDirection == 'R') carRotation = 180; carDirection = 'B';
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void setFrame()
{
	drawText("Score: " + std::to_string(score), GLUT_BITMAP_9_BY_15, red, 50, 570); //setting the score card (string, font, color, x, y)
	drawBoxLine(550, 500, white, 300, 300); //frame border (width, height, color, x, y)
	drawGrid(550, 500, 25, 50, gapSize, grey, gridLayout); //background grid/ texture
	drawRectangle(100, 25, 100, 50, red); //car enter gate (length, height, x, y, color)
	drawRectangle(carExitGate[2], carExitGate[3], carExitGate[0], carExitGate[1], green); //car exit gate
}
// Function to create menu.
void createMenu(GLint id)
{
	if (id == 1)
	{
		gridLayout = true; //background grid pattern
	}
	else
	{
		gridLayout = false;  //background texture pattern
	}

	glutPostRedisplay();
}

void keyBoardEvent(unsigned char key, int x, int y)
{
	if (carRotation > 360) {
		carRotation = 0; //resetting carRotation to not to overflow
	}
	if (!holdR) { //if R key is currently not holding down
		switch (key)
		{
		case 'w': case 'W':; carPosition[1] += carAcceleration; carRotate('T'); break;
		case 's':; case 'S':; carPosition[1] -= carAcceleration; carRotate('B'); break;
		case 'a':; case 'A':; carPosition[0] -= carAcceleration; carRotate('L'); break;
		case 'd':; case 'D':; carPosition[0] += carAcceleration; carRotate('R'); break;
		case 'r':; case 'R':; holdR = true; break; //mkaing r key hold true
		case 27: exit(0); break; //escape key
		}
	}
	else
	{
		switch (key) //with R key pressed
		{
		case 'a':; case 'A':; carRotation += 2; break;
		case 'd':; case 'D':; carRotation -= 2; break;
		}
	}
	collisionDetection();
	glutPostRedisplay();
}


void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'r':; case 'R':; holdR = false; break; // listning whether R key has been released
	}
}

void displayInformation()
{
	std::cout << "--- WELCOME - 2D Classic Car Game --- \n" <<std:: endl;
	std::cout << "How to Play ---\n" << std::endl;
	std::cout << "1. Use W/A/S/D keys to navigate the Car." << std::endl;
	std::cout << "2. Hold R and Press A Or D to rotate the car to left or right." << std::endl;
	std::cout << "3. Game will start with 50 points and will deduct 10 points if hit any hazard block." << std::endl;
	std::cout << "4. Right click to change the background." << std::endl;
}

void resetGame()
{
	carPosition[0] = 100;
	carPosition[1] = 100;
	carRotate('T');
	setHazardBlocks(); // set hazard blocks
	score += 50; //add 50 points before each level starts
}

/* reshape callback function*/
void reshape(GLsizei w, GLsizei h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	//set the current matrix to the Identity matrix
	glLoadIdentity();

	// Set the aspect ratio of the clipping area to match the viewport
	if (w <= h)
		gluOrtho2D(0.0f, width, 0.0f, height * h / w);
	else
		gluOrtho2D(0.0f, width * w / h, 0.0f, height);
}

/* display callback function */
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //setting the matrix mode from projection to model view
	glLoadIdentity(); //loading the idntity matrix

	glPushMatrix();
	setFrame(); //setiing the border frame, score car, enter and exit gates
	glPopMatrix();


	glPushMatrix();
	GLfloat color[3] = { 0, 0, 0 };
	for (size_t i = 0; i < blockCount; i++) //drawing the hazardblocks on screen
	{
		color[0] = hazardBlocks[i][2]; //setting the stored color
		color[1] = hazardBlocks[i][3];
		color[2] = hazardBlocks[i][4];
		drawSquare(50, hazardBlocks[i][0], hazardBlocks[i][1], color);
	}
	glPopMatrix();

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(carPosition[0], carPosition[1], 0); //car translation
	glRotatef(carRotation, 0, 0, 1); //appplying the car rotation
	glTranslatef(-100, -100, 0);  //translation car to the 0, 0
	drawCar(100, 100); //draw car
	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
	glFlush();

}

//Timer Fucntion
void TimerFunction(int value)
{
	//exhaust Animaton Variable Change
	exhaustP++;
	if (exhaustP > 6)
	{
		exhaustP = 0;
	}
	glutTimerFunc(5, TimerFunction, 0);
}

/* graphics initialisation */
void init(void)
{
	srand(time(NULL)); //random number changes with time
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	displayInformation(); //console out put
	resetGame(); //resetting hazard blocks
}


int main(int argc, char** argv)
{
	/* window management code ... */
	/* initialises GLUT and processes any command line arguments */
	glutInit(&argc, argv);
	/* use single-buffered window and RGBA colour model */
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	/* window width = 400 pixels, height = 400 pixels */
	glutInitWindowSize(400, 400);
	/* window upper left corner at (100, 100) */
	glutInitWindowPosition(100, 100);
	/* creates an OpenGL window with command argument in its title bar */
	glutCreateWindow("2D CarGame");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyBoardEvent);
	glutKeyboardUpFunc(keyUp);
	//Create Menu
	glutCreateMenu(createMenu);
	glutAddMenuEntry("Grid", 1);
	glutAddMenuEntry("Pattern", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(5, TimerFunction, 0);
	glutMainLoop();
	return 0;
}