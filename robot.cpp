//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "basicLibrary.h"
#include <iostream>
#include <random>
#include <time.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

using namespace std;

int gridSize = 10;

vector<point2D> points(0);
//points.push_back(point2D(x,y));
//points.at(i).x;
//points.at(i).y;
//points.size();

int bodyX, bodyY;	//variables for coordinates of x and y position of body
int origX, origY;	//variables for storing the original coordinates of body
int headPos;	//0 - north, 1 - east, 2 - south, 3 - west
int headX, headY;	//variables for coordinates of x and y position of head

void randomXY(){
	srand(time(0));	//randomly placing the robot somewhere on the grid
	bodyX = rand() % gridSize;	//10 bc 10 numbers from 0-9
	bodyY = rand() % gridSize;
	origX = bodyX;	//storing the original position of the body (that is randomly generated)
	origY = bodyY;
	if (bodyY == gridSize-1){	//if body is placed in top row
		headPos = 2;	//move head to be below
	} else {
		headPos = 0; 	//otherwise, put head to be at the top
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)	
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;	
	}
}

void display(void)
{
	glClearColor(0.0,0.0,0.0,0.0);	//colour change for background
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(1.0f);
	glColor3f(1.0f,1.0f,1.0f);
	points.clear();
	glBegin(GL_POINTS);
		for (unsigned int i = 0; i < points.size(); i++){
			glVertex2f(points.at(i).x, points.at(i).y);			
		}

		//drawing vertical lines
		for (int i = -gridSize/2; i < gridSize/2; i++){
			for(int j = -200; j < 200; j++){
				glVertex2f(i*400/gridSize, j);
			}
		}
		//drawing horizontal lines
		for (int i = -200; i < 200; i++){
			for(int j = -gridSize/2; j < gridSize/2; j++){
				glVertex2f(
					i, j*400/gridSize);
			}
		}
		//My attempt to make 30x30 grid aligned.. nothing worked so I just left it 
		/*if (gridSize == 30){	
			for (int i = (-200+((400/13)*bodyX));	i < (-200+((400/13)*bodyX)); i++){
				for (int j = (-200+((400/13)*bodyY));	j < (-200+((400/13)*bodyY)); j++){
					glColor3f(0,1,0);
					glVertex2f(i,j);
					}
				}
		} else {*/
		//drawing body
		for(int i = -200+((400/gridSize)*bodyX); i < -200+((400/gridSize)*bodyX)+400/gridSize; i++){
			for(int j = -200+((400/gridSize)*bodyY); j < -200+((400/gridSize)*bodyY)+400/gridSize; j++){
				glColor3f(0,1,0);
				glVertex2f(i,j);	
			}
		}
			//}
		
		//drawing head
		
		//determining the position of head
		if (headPos == 0){
			headY = bodyY+1;	//moves it ontop
			headX = bodyX;	//same x position
		} else if (headPos == 2){
			headY = bodyY-1;	//putting head below
			headX = bodyX;	//same x position
		} else if (headPos == 1){	//head placement is east
			headY = bodyY;
			headX = bodyX+1;	//moving one spot over to the right
		} else if (headPos == 3){	//head placement is west
			headY = bodyY;
			headX = bodyX-1;	//moving one spot over to the left
		}
		//drawing red square for the head of robot
		for(int i = -200+((400/gridSize)*headX); i < -200+((400/gridSize)*headX)+400/gridSize; i++){
			for(int j = -200+((400/gridSize)*headY); j < -200+((400/gridSize)*headY)+400/gridSize; j++){
				glColor3f(1,0,0);
				glVertex2f(i,j);
			}
		}
	glEnd();
	glFlush();		//flush buffer
}


void setPixel(int x, int y)
{
	points.push_back(point2D((float)x,(float)y));	//adding points to vector
}

void drawPixel(int x1, int y1, int x2, int y2)
{
	int x0 = x1; int y0 = y1;
	//float x0 = x1; float y0 = y1;
	int x = x0;
	int y = y0;
	
	//cout << "start";
	setPixel(x, y); //draw initial point
	while (x < x2) {	//increases x value
		x++;
		setPixel(x, y);
	}
	cout << "x = " << x << " y = " << y << "\n";
	while (y < y2) {	//increases y value
		y++;
		setPixel(x, y);
	}
}

void menuProc(int value){
	if(value == 1){	//if right is clicked from menu
		//move the body right as long as body is not at the very right edge or body is two spaces away from edge and head is at edge
		if(!(bodyX == gridSize-1 || (bodyX == gridSize-2 && headPos == 1))){	
			bodyX = bodyX+1;
		}
		printf("Right was clicked\n");
	}
	if(value == 2){	//left menu
		//if body is not at the very left edge or two spaces away with head at the very left edge, move left
		if (!(bodyX == 0 || (bodyX == 1 && headPos == 3))){
		bodyX = bodyX-1;
		}
		printf("Left was clicked\n");
	}
	if(value == 3){	//up menu
		//if body is not at the very top edge, or two spaces away with head at very top, move up
		if(!(bodyY == gridSize-1 || (bodyY == gridSize-2 && headPos == 0))){
		bodyY = bodyY+1;
		}
		printf("Up was clicked\n");
	}
	if(value == 4){	//down menu
		//if body is not at the very bottom of grid, or two spaces away with head at very bottom, move down
		if(!(bodyY == 0 || (bodyY == 1 && headPos == 2))){
		bodyY = bodyY-1;
		}
		printf("Down was clicked\n");
	}
	if(value == 5){	//turning right menu
		headPos += 1;	//changing the head position between 0-3 (North 0,East 1,South 2,West 3)
		if(headPos > 3){	//head position won't move past three
			headPos = 0;	//thus, change position back to 0 to keep it turning
		} 
		if(bodyY == gridSize-1 && headPos == 0){	//body is at very top of grid, head wants to move to be above
			headPos = 3;	//resets back to left side (aka west)
		}
		if(bodyY == 0 && headPos == 2){	//body is at bottom, head wants to move below
			headPos = 1;	//resets head back to east
		}
		if(bodyX == gridSize-1 && headPos == 1){	//body is at right edge, head wants to move east from north
			headPos = 0;	//puts it back to north, cannot move
		}
		if(bodyX == 0 && headPos == 3){	//body is at left edge, head wants to move west from south
			headPos = 2;	//resets head back to south
		}
		//printf("%i\n", headPos);
		printf("Turn Right was clicked\n");
	}
	if(value == 6){	//left menu, same idea as right menu but opposite
		headPos -= 1;
		if(headPos < 0){
			headPos = 3;
		}
		if(bodyY == gridSize-1 && headPos == 0){
			headPos = 1;
		}
		if(bodyY == 0 && headPos == 2){
			headPos = 3;
		}
		if(bodyX == gridSize-1 && headPos == 1){
			headPos = 2;
		}
		if(bodyX == 0 && headPos == 3){
			headPos = 0;
		}
		//printf("%i\n", headPos);
		printf("Turn Left was clicked\n");
	}
	if(value == 7){	//changing the grid size depending on the menu clicked, sets the gridsize value to the corresponding size
		gridSize = 10;
		printf("10x10 was clicked\n");
	}
	if(value == 8){
		gridSize = 20;
		printf("20x20 was clicked\n");
	}
	if(value == 9){
		gridSize = 30;
		printf("30x30 was clicked\n");
	}
	if(value == 10){
		gridSize = 40;
		printf("40x40 was clicked\n");
	}
	if(value == 11){
		gridSize = 50;
		printf("50x50 was clicked\n");
	}
	if(value == 12){
		bodyX = origX;
		bodyY = origY;
		headPos = 0;
		printf("Reset was clicked\n");
	}

	glutPostRedisplay();
}

void createMenu(){
	//glutAddMenuEntry("Randomize", 12);
	int subMenu_id_Move = glutCreateMenu(menuProc);	//sub-menu for directions
	glutAddMenuEntry("Right", 1);
	glutAddMenuEntry("Left", 2);
	glutAddMenuEntry("Up", 3);
	glutAddMenuEntry("Down", 4);
	
	int subMenu_id_Turn = glutCreateMenu(menuProc);	//sub-menu for turning
	glutAddMenuEntry("Turn Right", 5);
	glutAddMenuEntry("Turn Left", 6);

	int subMenu_id_Grid = glutCreateMenu(menuProc);	//sub-menu for grid sizes
	glutAddMenuEntry("10x10", 7);
	glutAddMenuEntry("20x20", 8);
	glutAddMenuEntry("30x30", 9);
	glutAddMenuEntry("40x40", 10);
	glutAddMenuEntry("50x50", 11);

	int main_id = glutCreateMenu(menuProc);	//creating the menus for moving, turning and grid sizes
	glutAddMenuEntry("Reset", 12);
	glutAddSubMenu("Move", subMenu_id_Move);
	glutAddSubMenu("Turn", subMenu_id_Turn);
	glutAddSubMenu("Grid Size", subMenu_id_Grid);
	glutAttachMenu(GLUT_LEFT_BUTTON);
}

/* main function - program entry point */
int main(int argc, char** argv)	
{
	drawPixel(0,-100,0,100);
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(400,400);
	glutInitWindowPosition(200,200);
	glutCreateWindow("square");	//creates the window
	gluOrtho2D(-200,200,-200,200);
	glutDisplayFunc(display);	//registers "display" as the display callback function
	createMenu();
	randomXY();
	glutKeyboardFunc(keyboard);
	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}