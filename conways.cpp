//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
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

int current[40][40];	//first array
int nextState[40][40];	//second array, temporary one that does all of the updating

int timeIncDec = 50;	//speed variable

bool running = true; //running boolean to determine if it is paused or running

void keyboard(unsigned char key, int xIn, int yIn)	//quit the program
{
	switch (key)	
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;	
	}
}

//storing array into temporary array using two for loops to iterate through each index
void tempArr(int arr1[40][40], int arr2[40][40]){
	for(int i = 0; i < 40; i++){
		for(int j = 0; j < 40; j++){
			arr2[i][j] = arr1[i][j];
		}
	}
}
//generating 0 or 1 for each index of the array, 0 for dead and 1 for alive
void init(){
	srand(time(0));
	for(int i = 0; i < 40; i++){
		for(int j = 0; j < 40; j++){
			current[i][j] = rand() % 2;
		}
	}
}

//drawing the robot
void display(void){
	glClearColor(0.0,0.0,0.0,0.0);	//colour change for background
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPointSize(10.0f);
	glColor3f(1,1,1);
	
	if(running == true){	//if statement is so that the display is not always refreshing, only when it is running
		tempArr(current, nextState);	//storing the array of randomized cells into the temp array
		
		int count = 0; 	//counter
		for(int i = 0; i < 40; i++){	//indexing through the temporary array, counting the number of neighbours on all 8 sides
			for(int j = 0; j < 40; j++){
				count = current[i-1][i+1] + current[i][j+1] + current[i+1][j+1] + current[i+1][j] + current[i+1][j-1] + 
						current[i][j-1] + current[i-1][j-1] + current[i-1][j];

				//checking counter
				if(count < 2 || count > 3){
					nextState[i][j] = 0;	//cell dies
				} else if (count == 3){
					nextState[i][j] = 1;	//cell is brought to life
				} else if (count == 2){
					nextState[i][j] = nextState[i][j];	//cell stays the same
				}
			}
		}
		tempArr(nextState, current);	//updating the current array
	}

	glBegin(GL_POINTS);	//drawing each pixel
		for(int i = 0; i < 40; i++){
			for(int j = 0; j < 40; j++){
				if(current[i][j] == 1){	//if the cell is alive, draw a white pixel
					glColor3f(1,1,1);
					glVertex2f(i-20,j-20);
				} else {
					glColor3f(0,0,0);	//cell is dead, make pixel black
					glVertex2f(i-20,j-20);
				}
			}
		}
	glEnd();
	glFlush();		//flush buffer
}

//function to put pixels onto the grid, only works when the game is not running (aka paused)
void mouse(int btn, int state, int x,int y){	
	printf("MouseFunc coords: %i,%i\n", x, y);		

	if(running == false){	//if the game is paused and the left button is pressed at certain cell
		if (btn == GLUT_LEFT_BUTTON){	
			if(state == GLUT_DOWN){	
				if(current[x/10][40-y/10] == 0){	//make the cell to be alive if it is dead to begin with
					current[x/10][40-y/10] = 1;
				} else {
					current[x/10][40-y/10] = 0;	//make cell dead if it is alive and clicked
				}
			}
		}
	}
	glutPostRedisplay();	//refreshing
}

//timer function to deal with increasing or decreasing speed
void timerFunc(int v) {
	if(!(running == false)){	//as long as the game is running
		glutTimerFunc(timeIncDec, timerFunc, 0);	//smaller the number is, the faster it goes -- higher the number, slower it goes
	}
	glutPostRedisplay();
}

void menuProc(int value){
	if(value == 1){						//if clear grid is clicked, change all cells to be "dead" which will change colour to black
		for(int i = 0; i < 40; i++){
			for(int j = 0; j < 40; j++){
				current[i][j] = 0;
			}
		}
		printf("Clear grid was clicked\n");
	}
	if(value == 2){
		running = true;	//running is set to be true, calls the timer function to run again
		timerFunc(50);
		//timeIncDec = 50;
		printf("Run was clicked\n");
	}
	if(value == 3){
		running = false;	//function is not running, therefore paused
		printf("Pause was clicked\n");
	}
	if(value == 4){
		init();		//calls randomizing function to randomize the grid
		printf("Randomize was clicked\n");
	}
	if(value == 5){
		timeIncDec--;	//the smaller the value of timeIncDec is, the faster it goes
		printf("Fast was clicked\n");
	}
	if(value == 6){
		timeIncDec++;	//the higher the value of timeIncDec is, the slower it goes
		printf("Slow was clicked\n");
	}

	glutPostRedisplay();
}

//creating menu for actions 
void createMenu(){
	int subMenu_id_Speed = glutCreateMenu(menuProc);	//sub-menu for speed
	glutAddMenuEntry("Fast", 5);
	glutAddMenuEntry("Slow", 6);

	int main_id = glutCreateMenu(menuProc);	//creating the menus for clear grid, run, pause, randomize and speed control
	glutAddMenuEntry("Clear Grid", 1);
	glutAddMenuEntry("Run", 2);
	glutAddMenuEntry("Pause", 3);
	glutAddMenuEntry("Randomize", 4);
	glutAddSubMenu("Speed Control", subMenu_id_Speed);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* main function - program entry point */
int main(int argc, char** argv)	
{
	init();
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(400,400);
	glutInitWindowPosition(200,200);
	glutCreateWindow("square");	//creates the window
	gluOrtho2D(-20,20,-20,20);
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, timerFunc, 0);
	createMenu();
	glutMouseFunc(mouse);
	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}