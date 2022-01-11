
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h

#include "loadShaders.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
 

using namespace std;
  
//////////////////////////////////////
// identificatori 
GLuint
  VaoId,
  VboId,
  EboId,
  ColorBufferId,
  ProgramId,
  myMatrixLocation,
  matrUmbraLocation,
  viewLocation,
  projLocation,
  matrRotlLocation,
  codColLocation,
  depthLocation;

 GLuint texture;

int codCol;
float PI=3.141592;

// matrice utilizate
glm::mat4 myMatrix, matrRot; 

// elemente pentru matricea de vizualizare
float Refx=-30.0f, Refy=0.0f, Refz=0.0f;
float alpha = PI/8, beta = - PI / 2, dist=400.0f;
float Obsx, Obsy, Obsz;

float Vx=0.0, Vy=0.0, Vz=1.0;
glm::mat4 view;

// elemente pentru matricea de proiectie
float width=800, height=600, xwmin=-800.f, xwmax=800, ywmin=-600, ywmax=600, znear=0.1, zfar=1, fov=45;
glm::mat4 projection;
 
// sursa de lumina
float xL = 200.f, yL = -300.f, zL = 300.f;

// matricea umbrei
float matrUmbra[4][4];

// obiecte si pozitii
float xs[10] = { 0, 0, 0, 0, 0 };
float ys[10] = { 0, 0, 0, 0, 0 };
float zs[10] = { 0, 0, 0, 0, 0 };

int curObject = 0;
int nrObjects = 4;
bool focused = false;

//cursor
int cursorXs[] = {-170, -80, 30, 130};


void displayMatrix ( )
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
		cout <<  myMatrix[ii][jj] << "  " ;
		cout << endl;
	};

};

void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key) {
		case 'a' :
            if (curObject > 0)
                curObject--;
			break;
		case 'd' :
            if (curObject < nrObjects - 1)
                curObject++;
			break;
		case 'w' :
            if (zs[curObject] < 100)
                zs[curObject] += 10;
			break;
		case 's' :
            if (zs[curObject] > 0)
                zs[curObject] -= 10;
			break;
        case '6':
            beta += 0.01;
            break;
        case '4':
            beta -= 0.01;
            break;
        case '5':
            alpha -= 0.01;
            break;
        case '8':
            alpha += 0.01;
            break;

	}
if (key == 27)
exit(0);
}
void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT :
            if (xs[curObject] > -100)
                xs[curObject] -= 10;
			break;
		case GLUT_KEY_RIGHT :
            if (xs[curObject] < 100)
                xs[curObject] += 10;
			break;
		case GLUT_KEY_UP :
            if (ys[curObject] < 100)
                ys[curObject] += 10;
			break;
		case GLUT_KEY_DOWN :
            if (ys[curObject] > -100)
                ys[curObject] -= 10;
			break;
	}
}

void CreateVBO(void)
{
    // varfurile 
    GLfloat Vertices[] = {

        // coordonate                    // culori			// normale
        // varfuri tabletop
       -350.0f,  -150.0f, 0.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f,
        350.0f,  -150.0f, 0.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f,
        350.0f,  250.0f,  0.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f,
       -350.0f,  250.0f,  0.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f,
       // varfuri cub == object #0
       -200.0f,  -30.0f,   0.0f, 1.0f,    1.0f, 0.3f, 0.0f,   -1.0f, -1.0f, -1.0f, //  4
       -140.0f,  -30.0f,   0.0f, 1.0f,    1.0f, 0.3f, 0.0f,    1.0f, -1.0f, -1.0f, //  5
       -140.0f,   30.0f,   0.0f, 1.0f,    1.0f, 0.3f, 0.0f,    1.0f,  1.0f, -1.0f, //  6
       -200.0f,   30.0f,   0.0f, 1.0f,    1.0f, 0.3f, 0.0f,   -1.0f,  1.0f, -1.0f, //  7
       -200.0f,  -30.0f,  60.0f, 1.0f,    1.0f, 0.3f, 0.0f,   -1.0f, -1.0f,  1.0f, //  8
       -140.0f,  -30.0f,  60.0f, 1.0f,    1.0f, 0.3f, 0.0f,    1.0f, -1.0f,  1.0f, //  9
       -140.0f,   30.0f,  60.0f, 1.0f,    1.0f, 0.3f, 0.0f,    1.0f,  1.0f,  1.0f, // 10
       -200.0f,   30.0f,  60.0f, 1.0f,    1.0f, 0.3f, 0.0f,   -1.0f,  1.0f,  1.0f, // 11

       //varfuri piramida == object #1
       -100.0f,  -30.0f,   0.0f, 1.0f,    0.3f, 0.3f, 1.0f,   -1.0f, -1.0f, -1.0f, // 12
        -60.0f,  -30.0f,   0.0f, 1.0f,    0.3f, 0.3f, 1.0f,    1.0f, -1.0f, -1.0f, // 13
        -60.0f,   30.0f,   0.0f, 1.0f,    0.3f, 0.3f, 1.0f,    1.0f,  1.0f, -1.0f, // 14
       -100.0f,   30.0f,   0.0f, 1.0f,    0.3f, 0.3f, 1.0f,   -1.0f,  1.0f, -1.0f, // 15
        -80.0f,   0.0f,   60.0f, 1.0f,    0.3f, 0.3f, 1.0f,    0.0f,  0.0f,  1.0f, // 16

       //varfuri trunchi de piramida == object #2
          0.0f,  -30.0f,   0.0f, 1.0f,    0.0f, 1.0f, 0.7f,   -1.0f, -1.0f, -1.0f, // 17
         60.0f,  -30.0f,   0.0f, 1.0f,    0.0f, 1.0f, 0.7f,    1.0f, -1.0f, -1.0f, // 18
         60.0f,   30.0f,   0.0f, 1.0f,    0.0f, 1.0f, 0.7f,    1.0f,  1.0f, -1.0f, // 19
          0.0f,   30.0f,   0.0f, 1.0f,    0.0f, 1.0f, 0.7f,   -1.0f,  1.0f, -1.0f, // 20
         15.0f,  -15.0f,  40.0f, 1.0f,    0.0f, 1.0f, 0.7f,   -1.0f, -1.0f,  1.0f, // 21
         45.0f,  -15.0f,  40.0f, 1.0f,    0.0f, 1.0f, 0.7f,    1.0f, -1.0f,  1.0f, // 22
         45.0f,   15.0f,  40.0f, 1.0f,    0.0f, 1.0f, 0.7f,    1.0f,  1.0f,  1.0f, // 23
         15.0f,   15.0f,  40.0f, 1.0f,    0.0f, 1.0f, 0.7f,   -1.0f,  1.0f,  1.0f, // 24

        //varfuri paralelipiped == object #3
        100.0f,  -30.0f,    0.0f, 1.0f,    0.5f, 1.0f, 0.3f,   -1.0f, -1.0f, -1.0f, // 25
        160.0f,  -30.0f,    0.0f, 1.0f,    0.5f, 1.0f, 0.3f,    1.0f, -1.0f, -1.0f, // 26
        160.0f,   30.0f,    0.0f, 1.0f,    0.5f, 1.0f, 0.3f,    1.0f,  1.0f, -1.0f, // 27
        100.0f,   30.0f,    0.0f, 1.0f,    0.5f, 1.0f, 0.3f,   -1.0f,  1.0f, -1.0f, // 28
        100.0f,  -30.0f,  100.0f, 1.0f,    0.5f, 1.0f, 0.3f,   -1.0f, -1.0f,  1.0f, // 29
        160.0f,  -30.0f,  100.0f, 1.0f,    0.5f, 1.0f, 0.3f,    1.0f, -1.0f,  1.0f, // 30
        160.0f,   30.0f,  100.0f, 1.0f,    0.5f, 1.0f, 0.3f,    1.0f,  1.0f,  1.0f, // 31
        100.0f,   30.0f,  100.0f, 1.0f,    0.5f, 1.0f, 0.3f,   -1.0f,  1.0f,  1.0f, // 32

        //varfuri cursor
        -5.0f,   -50.0f,   1.0f, 1.0f,    0.0f, 1.0f, 0.3f,   1.0f, 1.0f, 1.0f, // 33
         5.0f,   -50.0f,   1.0f, 1.0f,    0.0f, 1.0f, 0.3f,   1.0f, 1.0f, 1.0f, // 34
         2.5f,   -35.0f,   1.0f, 1.0f,    0.0f, 1.0f, 0.3f,   1.0f, 1.0f, 1.0f, // 35
         
        // varfuri picior masa
       -350.0f,  -150.0f, -300.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f, // 36
       -350.0f,  -130.0f, -300.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f, // 37
       -330.0f,  -130.0f, -300.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f, // 38
       -330.0f,  -150.0f, -300.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f, // 39
       -350.0f,  -150.0f,    0.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f, // 40
       -350.0f,  -130.0f,    0.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f, // 41
       -330.0f,  -130.0f,    0.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f, // 42
       -330.0f,  -150.0f,    0.0f, 1.0f,   0.729f, 0.549f, 0.388f,  0.0f, 0.0f, 1.0f, // 43

        // varfuri picior masa pentru muchii/contur
       -350.0f,  -150.0f, -300.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 44
       -350.0f,  -130.0f, -300.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 45
       -330.0f,  -130.0f, -300.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 46
       -330.0f,  -150.0f, -300.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 47
       -350.0f,  -150.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 48
       -350.0f,  -130.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 49
       -330.0f,  -130.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 50
       -330.0f,  -150.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 51

       // varfuri tabletop pentru contur
       -350.0f,  -530.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 52
        350.0f,  -530.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 53
        350.0f,  -130.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 54
       -350.0f,  -130.0f,    0.0f, 1.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 55
    };

    // indicii pentru varfuri
    GLubyte Indices[] = {

       // fetele tabletop
       1, 2, 0,   2, 0, 3,
       
       // fete cub
       4,  7,  5,   7,  6,  5,
       4,  5,  9,   4,  9,  8,
       5,  6,  9,   6, 10,  9,
       6,  7, 10,   7, 11, 10,
       7,  4,  8,   7,  8, 11,
       8,  9, 10,   8, 10, 11,

       // fete piramida
        12, 13, 16,
        13, 14, 16,
        14, 15, 16,
        15, 12, 16,

       // fete trunchi de piramida
       17, 20, 19,  17, 19, 18,
       17, 18, 22,  17, 22, 21,
       18, 19, 23,  18, 23, 22,
       19, 20, 24,  19, 24, 23, 
       20, 17, 21,  20, 21, 24,
       21, 22, 23,  21, 23, 24,

       // fete paralelipiped
       25, 28, 27,  25, 27, 26,
       25, 26, 30,  25, 30, 29,
       26, 27, 31,  26, 31, 30,
       27, 28, 32,  27, 32, 31,
       28, 25, 29,  28, 29, 32,
       29, 30, 31,  29, 31, 32,

       // cursor
       33, 34, 35,

       //fete picior masa
       36, 39, 38,  36, 38, 37,
       36, 37, 41,  36, 41, 40,
       37, 38, 42,  37, 42, 41,
       38, 39, 43,  38, 43, 42,
       39, 36, 40,  39, 40, 43,
       40, 41, 42,  40, 42, 43,
       //muchii picior
       44, 45,
       45, 46,
       46, 47,
       47, 44,
       44, 48,
       45, 49, 
       46, 50, 
       47, 51,

       //contur masa
       52, 53,
       53, 54,
       54, 55,
       55, 52
    };

    // se creeaza un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
    glGenVertexArrays(1, &VaoId);
    // se creeaza un buffer nou (atribute)
    glGenBuffers(1, &VboId);
    // se creeaza un buffer nou (indici)
    glGenBuffers(1, &EboId);

    // legarea VAO 
    glBindVertexArray(VaoId);

    // legarea buffer-ului "Array"
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // punctele sunt "copiate" in bufferul curent
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // legarea buffer-ului "Element" (indicii)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    // indicii sunt "copiati" in bufferul curent
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // se activeaza lucrul cu atribute; atributul 0 = pozitie
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);

    // se activeaza lucrul cu atribute; atributul 1 = culoare
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

    // se activeaza lucrul cu atribute; atributul 2 = normale
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));

}
void DestroyVBO(void)
{
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &VboId);
    glDeleteBuffers(1, &EboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
}

 
void CreateShaders(void)
{
  ProgramId=LoadShaders("11_02_Shader.vert", "11_02_Shader.frag");
  glUseProgram(ProgramId);
}
 
void DestroyShaders(void)
{
  glDeleteProgram(ProgramId);
}
 
void Initialize(void)
{

    myMatrix = glm::mat4(1.0f);
	matrRot=glm::rotate(glm::mat4(1.0f), PI/8, glm::vec3(0.0, 0.0, 1.0));
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // culoarea de fond a ecranului
    CreateShaders();
	
}
void RenderFunction(void)
{
  
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   //pozitia observatorului
   Obsx = Refx + dist * cos(alpha) * cos(beta);
   Obsy = Refy + dist * cos(alpha) * sin(beta);
   Obsz = Refz + dist * sin(alpha);
   
    // reperul de vizualizare
	glm::vec3 Obs = glm::vec3 (Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
    glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert =  glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
  	view = glm::lookAt(Obs, PctRef, Vert);
	 	 
    projection = glm::infinitePerspective(fov, GLfloat(width)/GLfloat(height), znear);
    myMatrix = glm::mat4(1.0f);
    // matricea pentru umbra
    float D = -0.5f;
    matrUmbra[0][0] = zL+D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
    matrUmbra[1][0] = 0; matrUmbra[1][1] = zL+D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
    matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
    matrUmbra[3][0] = -D*xL; matrUmbra[3][1] = -D*yL; matrUmbra[3][2] = -D*zL; matrUmbra[3][3] = zL;
 
  CreateVBO();
 
 // variabile uniforme pentru shaderul de varfuri
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
  glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE,  &matrUmbra[0][0]);
  viewLocation = glGetUniformLocation(ProgramId, "view");
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE,  &view[0][0]);
  projLocation = glGetUniformLocation(ProgramId, "projection");
  glUniformMatrix4fv(projLocation, 1, GL_FALSE,  &projection[0][0]);
 
  // Variabile uniforme pentru iluminare
  GLint lightColorLoc  = glGetUniformLocation(ProgramId, "lightColor");
  GLint lightPosLoc    = glGetUniformLocation(ProgramId, "lightPos");
  GLint viewPosLoc     = glGetUniformLocation(ProgramId, "viewPos");
  GLint codColLocation = glGetUniformLocation(ProgramId, "codCol");
  glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
  glUniform3f(lightPosLoc, xL, yL, zL);
  glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);

  // desenare ground
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::mat4(1.0f);
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
  
  //desenare cub
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xs[0], ys[0], zs[0]));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));

  //desenare piramida
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xs[1], ys[1], zs[1]));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (void*)(42));

  //desenare trunchi de piramida
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xs[2], ys[2], zs[2]));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(54));

  //desenare paralelipiped
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xs[3], ys[3], zs[3]));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(90));

  //desenare cursor
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cursorXs[curObject] + xs[curObject], ys[curObject], 0));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (void*)(126));

  glLineWidth(3);

      //desenare primul picior al mesei
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::mat4(1.0f);
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(129));
  glDrawElements(GL_LINES, 16, GL_UNSIGNED_BYTE, (void*)(165));

  //desenare al doilea picior al mesei
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(680, 0, 0));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(129));
  glDrawElements(GL_LINES, 16, GL_UNSIGNED_BYTE, (void*)(165));

  //desenare al treilea picior al mesei
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(680, 380, 0));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(129));
  glDrawElements(GL_LINES, 16, GL_UNSIGNED_BYTE, (void*)(165));

  //desenare al patrulea picior al mesei
  
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 380, 0));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(129));
  glDrawElements(GL_LINES, 16, GL_UNSIGNED_BYTE, (void*)(175));


  // desenare contur masa
  /*
  codCol = 0;
  glUniform1i(codColLocation, codCol);*/
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(200, -380, 0));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(191));


  /*
  // desenare con
  myMatrix= glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 150.0));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, (void*)(42));

  //new
  glm::mat4 matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 50.f, 0.0f));
  myMatrix = glm::mat4(1.0f) * matrTransl;
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)(60));
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(66));
  */

  // desenare umbra cub
  codCol = 1;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xs[0], ys[0], zs[0]));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));

  //desenare umbra piramida
  codCol = 1;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xs[1], ys[1], zs[1]));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (void*)(42));

  //desenare umbra trunchi de piramida
  codCol = 1;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xs[2], ys[2], zs[2]));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(54));

  //desenare umbra paralelipiped
  codCol = 1;
  glUniform1i(codColLocation, codCol);
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(xs[3], ys[3], zs[3]));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(90));

  /*
  // desenare umbra con
  myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 150.0));
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, (void*)(42));

  //new
  myMatrix = glm::mat4(1.0f) * matrTransl;
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(66));
  */
  glutSwapBuffers(); 
  glFlush ( );
}
void Cleanup(void)
{
  DestroyShaders();
  DestroyVBO();
}

int main(int argc, char* argv[])
{
 
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
  glutInitWindowPosition (100,100); 
  glutInitWindowSize(1200,900); 
  glutCreateWindow("3DScene - Proiect 2"); 
  glewInit(); 
  Initialize( );
  glutIdleFunc(RenderFunction);
  glutDisplayFunc(RenderFunction);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
 

  glutCloseFunc(Cleanup);
  glutMainLoop();
 
}

