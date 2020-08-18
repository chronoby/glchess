#include "TextureLoader.h"
#include "ObjStack.h"
#include "defs.h"
#include "ObjLoader.h"
#include "utils.h"
#include "Movement.h"
#include "Mouse.h"

#pragma warning (disable: 4996)

void display();
void keyboardInterrupt(unsigned char keyPressed, int x, int y);
void mouseInterrupt(int button, int state, int x, int y);
void motion(int x, int y);
void movePiece(int passVal);

void initialize();
void initLight();
void initBoard();

void drawAll(GLenum);
void drawOne(shape_ptr p, GLenum mode);
int pickFunction(int button, int state, int x, int y);
int processHits(GLint signedHits, GLuint buffer[]);

int findLight();
void objGenerator();

Movement mover;
ObjStack objects;
int pickedObject = 0;
int positionFilled[64];
int objID = 0;
bool seeded = 0;
int seedStart = 0;
GLuint texture;
bool inMotion = false;
bool editMode = false;
int* moveVector;
int texArray[64];
int lightnames[8] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };

ObjLoader piecebase("resource/piecebase.obj");
ObjLoader pawn("resource/pawn.obj");
ObjLoader rook("resource/rook.obj");
ObjLoader knight("resource/knight.obj");
ObjLoader bishop("resource/bishop.obj");
ObjLoader queen("resource/queen.obj");
ObjLoader king("resource/king.obj");

int CAMERA_MODE = 0;
int OBJECT_MODE = 0;
int PIECE_MODE = 0;
int COLOR_MODE = 0;

string LIGHT_MODE = ZERO;

bool lighton[8] = { true, false, false, false, false, false, false, false };

GLfloat lightp[8][4] = {
	{ 0, 1, 0, 0},
	{ 0,-1, 0, 0},
	{ 0, 0, 1, 0},
	{1, 3,-3, 16},
	{ 3, 5, -1, 12},
	{-1, 1, -2, 28},
	{ 1, 0, 1, 0},
	{-1, 0, 1, 0}
};

GLfloat lightc[][3][3] = {
	{{0.6, 0.6, 0.6}, {0.7, 0.7, 0.7}, {0.1, 0.1, 0.1}},
	{{0.5, 0.5, 0.5}, {0.3, 0.3, 0.5}, {0.1, 0.1, 0.1}},
	{{0.5, 0.5, 0.5}, {0.3, 0.5, 0.3}, {0.1, 0.1, 0.1}},
	{{0.5, 0.5, 0.5}, {0.3, 0.5, 0.5}, {0.1, 0.1, 0.1}},
	{{0.5, 0.5, 0.5}, {0.5, 0.3, 0.3}, {0.1, 0.1, 0.1}},
	{{0.5, 0.5, 0.5}, {0.5, 0.3, 0.5}, {0.1, 0.1, 0.1}},
	{{0.5, 0.5, 0.5}, {0.5, 0.5, 0.3}, {0.1, 0.1, 0.1}},
	{{0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.1, 0.1, 0.1}},
};

Mouse mouse;

enum Mode
{
	CAMERA, OBJECT, PIECE, COLOR, LIGHT
};

Mode mode = CAMERA;

Vector spin(17.8f, 27.4f, 0.0f);
Vector camPosition(0.0f, 0.0f, 15.0f);
Vector lookAt(0.63f, -2.2f, 0.0f);
Vector vup(0.0f, 1.0f, 0.0f);
Vector vside(1.0f, 0.0f, 0.0f);

Vector vpn() { return (lookAt - camPosition).Normal(); }


int main(int argc, char** argv)
{
	srand((unsigned)time(0));
	for (int i = 0; i < 64; i++) texArray[i] = rand() % 3;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	initialize();
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1, 0.1, 0.1, 1);
	glutMainLoop();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	initLight();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(VIEWANGLE, ASPECT, NEAR, FAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPosition.x, camPosition.y, camPosition.z, lookAt.x, lookAt.y, lookAt.z, vup.x, vup.y, vup.z);
	glTranslatef(mouse.MouseUpdate.Translation.x, mouse.MouseUpdate.Translation.y, mouse.MouseUpdate.Translation.z);
	spin = spin + mouse.MouseUpdate.Rotation;
	mouse.ClearUpdate();
	glRotatef(spin.x, 0, 1, 0);
	glRotatef(spin.y, 1, 0, 0);
	drawAll(GL_RENDER);
	glutSwapBuffers();
}

void initLight()
{
	for (int i = 0; i < 8; i++)
	{
		if (lighton[i] == true)
		{
			glEnable(lightnames[i]);
			glLightfv(lightnames[i], GL_POSITION, lightp[i]);
			glLightfv(lightnames[i], GL_SPECULAR, lightc[i][0]);
			glLightfv(lightnames[i], GL_DIFFUSE, lightc[i][1]);
			glLightfv(lightnames[i], GL_AMBIENT, lightc[i][2]);
		}
		else glDisable(lightnames[i]);
	}
}

void initialize()
{
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Chess");
	glutKeyboardFunc(keyboardInterrupt);
	glutMouseFunc(mouseInterrupt);
	glutMotionFunc(motion);
	glutDisplayFunc(display);

	LoadBitmap("resource/chessBoardBlack.bmp");
	LoadBitmap("resource/chessBoardWhite.bmp");
	// BOARD
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			shape_ptr p = new shape;
			p->type = BOARDPIECE;
			if ((i + j) % 2 == 0)
			{
				p->ambimat[0] = 0.35; p->ambimat[1] = 0.35; p->ambimat[2] = 0.35;
				p->specmat[0] = 0.9;  p->specmat[1] = 0.9;  p->specmat[2] = 0.9;
				p->diffmat[0] = 0.7;  p->diffmat[1] = 0.7;  p->diffmat[2] = 0.7;
				p->texID = 1;
			}
			else
			{
				p->ambimat[0] = 0.1;  p->ambimat[1] = 0.1;  p->ambimat[2] = 0.1;
				p->specmat[0] = 0.8;  p->specmat[1] = 0.8;  p->specmat[2] = 0.8;
				p->diffmat[0] = 0.7;  p->diffmat[1] = 0.7;  p->diffmat[2] = 0.7;
				p->texID = 2;
			}
			p->shininess = 4;
			p->position[0] = i - 3.5; p->position[1] = 0.0; p->position[2] = j - 3.5;
			p->rotation[0] = 0;	p->rotation[1] = 0; p->rotation[2] = 0;
			p->scale[0] = 1; p->scale[1] = 0.2; p->scale[2] = 1;
			p->ID = objID;
			objID++;
			objects.push(p);
		}
	}
}

void drawAll(GLenum mode)
{
	shape_ptr p = objects.getTop();
	for (int i = 0; i < objects.length(); i++)
	{
		drawOne(p, mode);
		p = p->next;
	}
}

void drawOne(shape_ptr drawShape, GLenum mode)
{
	glPushMatrix();
	int id = drawShape->type;

	GLfloat transX = drawShape->position[0];
	GLfloat transY = drawShape->position[1];
	GLfloat transZ = drawShape->position[2];

	GLfloat rotatX = drawShape->rotation[0];
	GLfloat rotatY = drawShape->rotation[1];
	GLfloat rotatZ = drawShape->rotation[2];

	GLfloat scaleX = drawShape->scale[0];
	GLfloat scaleY = drawShape->scale[1];
	GLfloat scaleZ = drawShape->scale[2];

	GLfloat* spec = &drawShape->specmat[0];
	GLfloat* diff = &drawShape->diffmat[0];
	GLfloat* ambi = &drawShape->ambimat[0];
	GLfloat shiny = drawShape->shininess;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambi);
	glMaterialf(GL_FRONT, GL_SHININESS, shiny);

	glTranslatef(transX, transY, transZ);
	glScalef(scaleX, scaleY, scaleZ);
	glRotatef(rotatX, 1, 0, 0);
	glRotatef(rotatY, 0, 1, 0);
	glRotatef(rotatZ, 0, 0, 1);

	if (mode == GL_SELECT) glLoadName(drawShape->ID);

	if (id == BOARDPIECE)
	{
		glEnable(GL_TEXTURE_2D);
		if (drawShape->texID == 1) glBindTexture(GL_TEXTURE_2D, 1);
		else if (drawShape->texID == 2) glBindTexture(GL_TEXTURE_2D, 2);
		fieldSquare(texArray[drawShape->ID]);
		glDisable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	}
	if (id == BOARDBASE)
	{
		boardBase();
	}
	if (id == PAWN)
	{
		drawPiece(piecebase);
		drawPiece(pawn);
	}
	if (id == BISHOP)
	{
		drawPiece(piecebase);
		drawPiece(bishop);
	}
	if (id == KING)
	{
		drawPiece(piecebase);
		drawPiece(king);
	}
	if (id == QUEEN)
	{
		drawPiece(piecebase);
		drawPiece(queen);
	}
	if (id == ROOK)
	{
		drawPiece(piecebase);
		drawPiece(rook);
	}
	if (id == KNIGHT)
	{
		drawPiece(piecebase);
		drawPiece(knight);
	}
	glPopMatrix();
}

void keyboardInterrupt(unsigned char keyPressed, int x, int y)
{
	if (keyPressed == 'E' || keyPressed == 'e') editMode = !editMode;
	if (keyPressed == 'J' || keyPressed == 'j') COLOR_MODE = AMBIENT;
	if (keyPressed == 'K' || keyPressed == 'k') COLOR_MODE = DIFFUSE;
	if (keyPressed == 'L' || keyPressed == 'l') COLOR_MODE = SPECULAR;
	if (keyPressed == 'q' || keyPressed == 'Q') objGenerator();
	if (keyPressed == 'p' || keyPressed == 'P') screenShot();
	if (keyPressed == 'w')
	{
		if (editMode)
		{
			shape_ptr obj = objects.find(pickedObject);
			obj->scale[1] += .01;
		}
		else
		{ 
			Vector increment = vpn() * INC;
			lookAt = lookAt + increment;
			camPosition = camPosition + increment;
		}
	}
	if (keyPressed == 'a')
	{
		if (editMode)
		{
			shape_ptr obj = objects.find(pickedObject);
			obj->scale[0] -= .01;
		}
		else
		{
			Vector left = (vpn() | vup) * -INC;
			lookAt = lookAt + left;
			camPosition = camPosition + left;
		}
	}
	if (keyPressed == 'd')
	{
		if (editMode)
		{
			shape_ptr obj = objects.find(pickedObject);
			obj->scale[0] += .01;
		}
		else
		{
			Vector right = (vpn() | vup) * INC;
			lookAt = lookAt + right;
			camPosition = camPosition + right;
		}
	}
	if (keyPressed == 's')
	{
		if (editMode)
		{
			shape_ptr obj = objects.find(pickedObject);
			obj->scale[1] -= .01;
		}
		else
		{ 
			Vector increment = vpn() * INC;
			lookAt = lookAt - increment;
			camPosition = camPosition - increment;
		}
	}
	if (keyPressed == 'r')
	{
		if (editMode)
		{
			shape_ptr obj = objects.find(pickedObject);
			obj->scale[2] -= .01;
		}
		else
		{
			Vector up = (vpn() | vside) * -INC;
			lookAt = lookAt + up;
			camPosition = camPosition + up;
		}
	}
	if (keyPressed == 't')
	{
		if (editMode)
		{
			shape_ptr obj = objects.find(pickedObject);
			obj->scale[2] += .01;
		}
		else
		{
			Vector down = (vpn() | vside) * INC;
			lookAt = lookAt + down;
			camPosition = camPosition + down;
		}
	}
	if (keyPressed == 'z')
	{
		if (PIECE_MODE == PAWN && OBJECT_MODE == REDPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = REDPIECE;
			PIECE_MODE = PAWN;
		}
	}
	if (keyPressed == 'Z')
	{
		if (PIECE_MODE == PAWN && OBJECT_MODE == GREENPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = GREENPIECE;
			PIECE_MODE = PAWN;
		}
	}
	if (keyPressed == 'x')
	{
		if (PIECE_MODE == ROOK && OBJECT_MODE == REDPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = REDPIECE;
			PIECE_MODE = ROOK;
		}
	}
	if (keyPressed == 'X')
	{
		if (PIECE_MODE == ROOK && OBJECT_MODE == GREENPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = GREENPIECE;
			PIECE_MODE = ROOK;
		}
	}
	if (keyPressed == 'c')
	{
		if (PIECE_MODE == KNIGHT && OBJECT_MODE == REDPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = REDPIECE;
			PIECE_MODE = KNIGHT;
		}
	}
	if (keyPressed == 'C')
	{
		if (PIECE_MODE == KNIGHT && OBJECT_MODE == GREENPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = GREENPIECE;
			PIECE_MODE = KNIGHT;
		}
	}
	if (keyPressed == 'v')
	{
		if (PIECE_MODE == BISHOP && OBJECT_MODE == REDPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = REDPIECE;
			PIECE_MODE = BISHOP;
		}
	}
	if (keyPressed == 'V')
	{
		if (PIECE_MODE == BISHOP && OBJECT_MODE == GREENPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = GREENPIECE;
			PIECE_MODE = BISHOP;
		}
	}
	if (keyPressed == 'n')
	{
		if (PIECE_MODE == KING && OBJECT_MODE == REDPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = REDPIECE;
			PIECE_MODE = KING;
		}
	}
	if (keyPressed == 'N')
	{
		if (PIECE_MODE == KING && OBJECT_MODE == GREENPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = GREENPIECE;
			PIECE_MODE = KING;
		}
	}
	if (keyPressed == 'b')
	{
		if (PIECE_MODE == QUEEN && OBJECT_MODE == REDPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = REDPIECE;
			PIECE_MODE = QUEEN;
		}
	}
	if (keyPressed == 'B')
	{
		if (PIECE_MODE == QUEEN && OBJECT_MODE == GREENPIECE)
		{
			OBJECT_MODE = 0;
			PIECE_MODE = 0;
		}
		else
		{
			OBJECT_MODE = GREENPIECE;
			PIECE_MODE = QUEEN;
		}
	}
	if (keyPressed == ' ') initBoard();
	if ((keyPressed == 'u' || keyPressed == 'U' || keyPressed == 'i' || keyPressed == 'I' || keyPressed == 'o' || keyPressed == 'O') && COLOR_MODE != NONE)
	{
		int whichLight = findLight();
		cout << "Current Light under control is " << whichLight << endl;
		cout << "Current LightMode under control is " << COLOR_MODE << endl;
		switch (keyPressed)
		{
			case 'u':
				lightc[whichLight][COLOR_MODE - 30][0] = lightc[whichLight][COLOR_MODE - 30][0] + 0.05;
				if (lightc[whichLight][COLOR_MODE - 30][0] > 1) lightc[whichLight][COLOR_MODE - 30][0] = 1;
				break;
			case 'U':
				lightc[whichLight][COLOR_MODE - 30][0] = lightc[whichLight][COLOR_MODE - 30][0] - 0.05;
				if (lightc[whichLight][COLOR_MODE - 30][0] < 0) lightc[whichLight][COLOR_MODE - 30][0] = 0;
				break;
			case 'i':
				lightc[whichLight][COLOR_MODE - 30][1] = lightc[whichLight][COLOR_MODE - 30][1] + 0.05;
				if (lightc[whichLight][COLOR_MODE - 30][1] > 1) lightc[whichLight][COLOR_MODE - 30][2] = 1;
				break;
			case 'I':
				lightc[whichLight][COLOR_MODE - 30][1] = lightc[whichLight][COLOR_MODE - 30][1] - 0.05;
				if (lightc[whichLight][COLOR_MODE - 30][1] < 0) lightc[whichLight][COLOR_MODE - 30][1] = 0;
				break;
			case 'o':
				lightc[whichLight][COLOR_MODE - 30][2] = lightc[whichLight][COLOR_MODE - 30][2] + 0.05;
				if (lightc[whichLight][COLOR_MODE - 30][2] > 1) lightc[whichLight][COLOR_MODE - 30][2] = 1;
				break;
			case 'O':
				lightc[whichLight][COLOR_MODE - 30][2] = lightc[whichLight][COLOR_MODE - 30][2] - 0.05;
				if (lightc[whichLight][COLOR_MODE - 30][2] < 0) lightc[whichLight][COLOR_MODE - 30][2] = 0;
				break;
		}
	}
	if (keyPressed == 'W' || keyPressed == 'A' || keyPressed == 'S' || keyPressed == 'D' || keyPressed == 'R' || keyPressed == 'T')
	{
		int whichLight = findLight();
		switch (keyPressed)
		{
		case 'A': lightp[whichLight][0] += 0.1f; break;
		case 'D': lightp[whichLight][0] -= 0.1f; break;
		case 'W': lightp[whichLight][1] += 0.1f; break;
		case 'S': lightp[whichLight][1] -= 0.1f; break;
		case 'R': lightp[whichLight][2] += 0.1f; break;
		case 'T': lightp[whichLight][2] -= 0.1f; break;
		}
	}
	switch (keyPressed)
	{
		case '1': lighton[1] = !lighton[1];
			LIGHT_MODE = ONE;
			break;
		case '2': lighton[2] = !lighton[2];
			LIGHT_MODE = TWO;
			break;
		case '3': lighton[3] = !lighton[3];
			LIGHT_MODE = THREE;
			break;
		case '4': lighton[4] = !lighton[4];
			LIGHT_MODE = FOUR;
			break;
		case '5': lighton[5] = !lighton[5];
			LIGHT_MODE = FIVE;
			break;
		case '6': lighton[6] = !lighton[6];
			LIGHT_MODE = SIX;
			break;
		case '7': lighton[7] = !lighton[7];
			LIGHT_MODE = SEVEN;
			break;
		case '0': lighton[0] = !lighton[0];
			LIGHT_MODE = ZERO;
			break;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutPostRedisplay();
}

void mouseInterrupt(int button, int state, int x, int y)
{
	std::cout << "CURRENT OBJID IS " << objID << endl;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		CAMERA_MODE = ROTATE_MODE;
		mouse.Down(RIGHT);
		mouse.SetPosition(x, y);
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		CAMERA_MODE = 0;
		mouse.Up(RIGHT);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		shape_ptr obj = NULL;
		int pickedType = 0;
		pickedObject = pickFunction(button, state, x, y);
		if (pickedObject != 999)
		{
			obj = objects.find(pickedObject);
			pickedType = obj->type;
		}

		if (pickedType != BOARDPIECE && pickedType != BOARDBASE && pickedObject != 999) // objectID != 0-64
		{
			mover.setObj(obj);
			mover.setStart();
			mover.on(true);
		}
		if (pickedType == BOARDPIECE)
		{
			if (mover.on())
			{
				if (positionFilled[pickedObject] == 0)
				{
					mover.setFinish(pickedObject);
					moveVector = mover.getDifference();
					cout << "x: " << moveVector[0] << " y: " << moveVector[1] << endl;
					inMotion = true;
					mover.on(false);
					glutTimerFunc(10, movePiece, (int)(30 * sqrt(moveVector[0] * moveVector[0] + moveVector[1] * moveVector[1])));
					mover.piece->location = pickedObject;
				}
			}
			else if (OBJECT_MODE == REDPIECE)
			{
				positionFilled[pickedObject] = objID;
				switch (PIECE_MODE)
				{
				case PAWN:
					addWhite(objects, pickedObject, objID, PAWN);
					break;
				case ROOK:
					addWhite(objects, pickedObject, objID, ROOK);
					break;
				case KNIGHT:
					addWhite(objects, pickedObject, objID, KNIGHT);
					break;
				case BISHOP:
					addWhite(objects, pickedObject, objID, BISHOP);
					break;
				case KING:
					addWhite(objects, pickedObject, objID, KING);
					break;
				case QUEEN:
					addWhite(objects, pickedObject, objID, QUEEN);
					break;
				}

			}
			else if (OBJECT_MODE == GREENPIECE)
			{
				positionFilled[pickedObject] = objID;
				switch (PIECE_MODE)
				{
				case PAWN:
					addBlack(objects, pickedObject, objID, PAWN);
					break;
				case ROOK:
					addBlack(objects, pickedObject, objID, ROOK);
					break;
				case KNIGHT:
					addBlack(objects, pickedObject, objID, KNIGHT);
					break;
				case BISHOP:
					addBlack(objects, pickedObject, objID, BISHOP);
					break;
				case KING:
					addBlack(objects, pickedObject, objID, KING);
					break;
				case QUEEN:
					addBlack(objects, pickedObject, objID, QUEEN);
					break;
				}

			}
			else
			{
				CAMERA_MODE = TRANSLATE_MODE;
				mouse.Down(LEFT);
				mouse.SetPosition(x, y);
			}

			glutPostRedisplay();
		}
		else if (pickedType == PAWN || pickedType == ROOK || pickedType == KNIGHT
			|| pickedType == BISHOP || pickedType == QUEEN || pickedType == KING)
		{
			if (OBJECT_MODE == REDPIECE)
			{
				addWhite(objects, pickedObject, objID, pickedType);
				for (int i = 0; i < 64; i++) if (positionFilled[i] == pickedObject) positionFilled[i] = 0;
				mover.on(false);
			}
			else if (OBJECT_MODE == GREENPIECE)
			{
				addBlack(objects, pickedObject, objID, pickedType);
				for (int i = 0; i < 64; i++) if (positionFilled[i] == pickedObject) positionFilled[i] = 0;
				mover.on(false);
			}
			else
			{
				CAMERA_MODE = TRANSLATE_MODE;
				mouse.Down(LEFT);
				mouse.SetPosition(x, y);
			}
			glutPostRedisplay();
		}
		else if (pickedType != 0)
		{
			CAMERA_MODE = TRANSLATE_MODE;
			mouse.Down(LEFT);
			mouse.SetPosition(x, y);
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (CAMERA_MODE == TRANSLATE_MODE) 
			CAMERA_MODE = NONE;
		mouse.Up(LEFT);
	}
	glutPostRedisplay();
}

void movePiece(int passVal)
{
	float increment = 30 * sqrt(moveVector[0] * moveVector[0] + moveVector[1] * moveVector[1]);
	if (passVal >= increment * (2.0f / 3.0f))
		mover.piece->position[1] += 2.0f / (increment / 3.0f);
	else if (passVal < increment / 3.0f)
		mover.piece->position[1] -= 2.0f / (increment / 3.0f);
	else
	{
		mover.piece->position[0] -= (float)moveVector[0] / (increment / 3.0f);
		mover.piece->position[2] -= (float)moveVector[1] / (increment / 3.0f);
	}
	glutPostRedisplay();
	passVal--;
	if (passVal > 0) glutTimerFunc(10, movePiece, passVal);
	else
	{
		mover.piece->position[1] = 0.2f;
		mover.piece->position[0] = -3.5 + mover.piece->location / 8;
		mover.piece->position[2] = -3.5 + mover.piece->location % 8;
		inMotion = false;
	}
}

void initBoard()
{
	if (!seeded)
	{
		//cout << "TRY INIT" << endl;
		seedStart = objID;
		addBlack(objects, 0, objID, ROOK);
		addBlack(objects, 1, objID, KNIGHT);
		addBlack(objects, 2, objID, BISHOP);
		addBlack(objects, 3, objID, KING);
		addBlack(objects, 4, objID, QUEEN);
		addBlack(objects, 5, objID, BISHOP);
		addBlack(objects, 6, objID, KNIGHT);
		addBlack(objects, 7, objID, ROOK);
		for (int i = 8; i < 16; i++)
			addBlack(objects, i, objID, PAWN);
		for (int i = 48; i < 56; i++) 
			addWhite(objects, i, objID, PAWN);
		addWhite(objects, 56, objID, ROOK);
		addWhite(objects, 57, objID, KNIGHT);
		addWhite(objects, 58, objID, BISHOP);
		addWhite(objects, 59, objID, KING);
		addWhite(objects, 60, objID, QUEEN);
		addWhite(objects, 61, objID, BISHOP);
		addWhite(objects, 62, objID, KNIGHT);
		addWhite(objects, 63, objID, ROOK);
		seeded = true;
	}
	else
	{
		for (int i = 0; i < 32; i++)
			objects.erase(seedStart + i);
		seeded = false;
	}
}

void motion(int x, int y)
{
	mouse.SetPosition(x, y);
	if (mouse.IsActive() && mouse.HasMoved())
	{
		mouse.Update();
		glutPostRedisplay();
	}
}

int pickFunction(int button, int state, int x, int y)
{
	GLuint selectBuf[MAX_NAME_BUFFER];
	GLint hits;
	GLint viewport[4];
	int hitObject = 0;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(MAX_NAME_BUFFER, selectBuf);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.0, 1.0, viewport);
	gluPerspective(VIEWANGLE, ASPECT, NEAR, FAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPosition.x, camPosition.y, camPosition.z, lookAt.x, lookAt.y, lookAt.z, vup.x, vup.y, vup.z);
	glTranslatef(mouse.MouseUpdate.Translation.x, mouse.MouseUpdate.Translation.y, mouse.MouseUpdate.Translation.z);
	spin = spin + mouse.MouseUpdate.Rotation;
	glRotatef(spin.x, 0, 1, 0);
	glRotatef(spin.y, 1, 0, 0);
	glRenderMode(GL_SELECT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glInitNames();
	glPushName(0);
	drawAll(GL_SELECT);
	hits = glRenderMode(GL_RENDER);
	hitObject = processHits(hits, selectBuf);
	//cout << "hitObject: " << hitObject << endl;

	glPopMatrix();
	if (!inMotion) 
		return hitObject;
	else return 999;
}

int processHits(GLint signedHits, GLuint buffer[])
{
	unsigned int i, j, hits;
	GLuint names, *ptr, minZ, *ptrNames, numberOfNames;
	ptrNames = 0;
	numberOfNames = 0;
	hits = signedHits;

	printf("hits = %d\n", hits);
	ptr = (GLuint *)buffer;
	minZ = 0xffffffff;

	for (i = 0; i < hits; i++)
	{
		names = *ptr;
		ptr++;
		if (*ptr < minZ)
		{
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr + 2;
		}

		ptr += names + 2;
	}
	printf("The closest object is ");
	ptr = ptrNames;
	int what = 0;
	for (j = 0; j < numberOfNames; j++, ptr++)
	{
		what = *ptr;
		printf("%d\n", what);
	}
	if (numberOfNames > 0) return what;
	else return 999;
}

int findLight()
{
	int lightNum = 0;
	if (LIGHT_MODE == ONE) lightNum = 1;
	else if (LIGHT_MODE == TWO) lightNum = 2;
	else if (LIGHT_MODE == THREE) lightNum = 3;
	else if (LIGHT_MODE == FOUR) lightNum = 4;
	else if (LIGHT_MODE == FIVE) lightNum = 5;
	else if (LIGHT_MODE == SIX) lightNum = 6;
	else if (LIGHT_MODE == SEVEN) lightNum = 7;

	return lightNum;
}

void objGenerator()
{
	if (pickedObject == 999)
		return;
	shape_ptr obj = objects.find(pickedObject);
	ObjLoader *shapeHead;
	ObjLoader *shapeBase = &piecebase;
	ofstream objOutput;
	float* vertices1 = shapeBase->GetVertices();
	int* quads1 = shapeBase->GetQuads();
	int* triangles1 = shapeBase->GetTriangles();
	float scale[3] = { obj->scale[0], obj->scale[1], obj->scale[2] };



	switch (obj->type)
	{
	case PAWN:	shapeHead = &pawn;	break;
	case BISHOP:shapeHead = &bishop; break;
	case KING:	shapeHead = &king;	break;
	case QUEEN:	shapeHead = &queen; break;
	case KNIGHT:shapeHead = &knight; break;
	case ROOK:	shapeHead = &rook;	break;
	default:
		cout << "No Corresponding Shape Supported for NOW" << endl;
		shapeHead = NULL;
	}

	if (shapeHead == NULL)
		return;
	float* vertices2 = shapeHead->GetVertices();
	int* quads2 = shapeHead->GetQuads();
	int* triangles2 = shapeHead->GetTriangles();

	int vertOffset = shapeBase->VertCount;
	objOutput.open("result/res.obj");
	for (int i = 0; i < shapeBase->VertCount; i++)
		objOutput << "v " << vertices1[i * 3] * scale[0] << " " << vertices1[i * 3 + 1] * scale[1] << " " << vertices1[i * 3 + 2] * scale[2] << endl;
	for (int i = 0; i < shapeHead->VertCount; i++)
		objOutput << "v " << vertices2[i * 3] * scale[0] << " " << vertices2[i * 3 + 1] * scale[1] << " " << vertices2[i * 3 + 2] * scale[2] << endl;
	for (int i = 0; i < shapeBase->TriangleCount; i++)
		objOutput << "f " << triangles1[i * 3] + 1 << " " << triangles1[i * 3 + 1] + 1 << " " << triangles1[i * 3 + 2] + 1 << endl;
	for (int i = 0; i < shapeHead->TriangleCount; i++)
		objOutput << "f " << triangles2[i * 3] + 1 + vertOffset << " " << triangles2[i * 3 + 1] + 1 + vertOffset << " " << triangles2[i * 3 + 2] + 1 + vertOffset << endl;
	for (int i = 0; i < shapeBase->QuadCount; i++)
		objOutput << "f " << quads1[i * 4] + 1 << " " << quads1[i * 4 + 1] + 1 << " " << quads1[i * 4 + 2] + 1 << " " << quads1[i * 4 + 3] + 1 << endl;
	for (int i = 0; i < shapeHead->QuadCount; i++)
		objOutput << "f " << quads2[i * 4] + 1 + vertOffset << " " << quads2[i * 4 + 1] + 1 + vertOffset << " " << quads2[i * 4 + 2] + 1 + vertOffset << " " << quads2[i * 4 + 3] + 1 + vertOffset << endl;

	objOutput.close();
}
