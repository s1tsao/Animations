////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Spinning Cube"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;

Joint* test;
Skinning* testSkin;
int run(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	
	test = new Joint("test.skel");
	testSkin = new Skinning("test.skin", test);
   


	
	
	glutMainLoop();
	return 0;
}


////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );



	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	//Cube.Update();
	test->Update();
	testSkin->Update();

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
	test->Reset();
	//Cube.Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	//Cube.Draw();
	test->Draw();
	testSkin->Draw(0);
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////


int joint_select = 0;
int deg = 0;
std::string skin;
std::string skel;
void Tester::Keyboard(int key,int x,int y) {

	FILE* file1;
    FILE* file2;
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'a':
			joint_select--;
			break;
		case 'd':
			joint_select++;
			break;
		case 'w':
			switch (deg) {
				case 0:
					testSkin->rotate_x(joint_select, 0.1);
					break;
				case 1:
					testSkin->rotate_y(joint_select, 0.1);
					break;
				case 2:
					testSkin->rotate_z(joint_select, 0.1);
					break;
			}
			break;
		case 's':
			switch (deg) {
			case 0:
				testSkin->rotate_x(joint_select, -0.1);
				break;
			case 1:
				testSkin->rotate_y(joint_select, -0.1);
				break;
			case 2:
				testSkin->rotate_z(joint_select, -0.1);
				break;
			}
			break;
		case 'x':
		    deg = 0;
			break;
		case 'y':
			deg = 1;
			break;
		case'z':
			deg = 2;
			break;
		case'l':
			std::cout << "enter file to load: ";
			skel = "";
			skin = "";
			std::cin >> skel;
			skin = skel;
			skel += ".skel";
			skin += ".skin";
			file1 = fopen(skel.c_str(), "r");
			file2 = fopen(skin.c_str(), "r");
			if ( file1 != NULL && file2 != NULL ) {
				test = new Joint((char*)skel.c_str());
				testSkin = new Skinning((char*)skin.c_str(), test);;
				std::cout << "file:"<<skel<<" "<<skin<<" loaded" << std::endl;
			}
			else {
				std::cerr << "no such files: "<<skel<<" "<<skin << std::endl;;
			}
			break;
		case 'r':
			Reset();
			break;
	}
	std::cout << "Joint: " << testSkin->skel.at(joint_select%testSkin->skel.size())->name << std::endl
		<< "axis:  " << ('x' + deg) << std::endl;
	std::cout << "degree: ";
		switch (deg) {
		case 0:
			std::cout << testSkin->skel.at(joint_select%testSkin->skel.size())->rotx.value << std::endl;
			break;
		case 1:
			std::cout << testSkin->skel.at(joint_select%testSkin->skel.size())->roty.value << std::endl;
			break;
		case 2:
			std::cout << testSkin->skel.at(joint_select%testSkin->skel.size())->rotz.value << std::endl;
			break;
		}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
