#define PROYECTO "Trabajo Final"
#define PI 3.141592
#define TASA_REFRESCO 60

#include <iostream>	
#include "include/codebase.h"
#include "include/header.h"

using namespace std;
using namespace cb;

static float VELOCIDAD_MAX = 16.0;
float velocidad_nave = 0.0;
float camara_pos_x = 0;
float camara_pos_y = 0;
float camara_pos_z = 2;
float camara_lookat_x = 0;
float camara_lookat_y = 0;
float camara_lookat_z = 0;
float angulo_cabeceo = 90;
float angulo_guino = 0;
int raton_y = 0;
int raton_x = 0;
bool luces = true;
bool cockpit = true;
bool mapa = true;
static GLuint texturas[15];
Asteroide asteroides[3];
float rotacion_tierra = 0;

void init_luces(){

	GLfloat Al0[]={0.1,0.1,0.1,1.0};				// Color ambiental de la luz
	GLfloat Dl0[]={0.7,0.7,0.7,1.0};						// Color difuso de la luz			
	GLfloat Sl0[]={0.7,0.7,0.7,1.0};						// Color especular de la luz
	glLightfv(GL_LIGHT0, GL_AMBIENT, Al0);			// Caracteristicas de LIGHT0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);
	glEnable(GL_LIGHT0);

	GLfloat Al2[]={0.4,0.4,0.3,1.0};				// Color ambiental de la luz
	GLfloat Dl2[]={1, 0.937, 0.85,1.0};				// Color difuso de la luz			
	GLfloat Sl2[]={1, 0.937, 0.85,1.0};				// Color especular de la luz
	glLightfv(GL_LIGHT1, GL_AMBIENT, Al2);			// Caracteristicas de LIGHT1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_AMBIENT, Al2);			// Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);
	glEnable(GL_LIGHT2);	

    GLfloat mat_diffuse[] = {0.5,0.5,0.5,1.0};          //Kd
    GLfloat mat_specular[] = {0.8,0.8,0.8,1.0};         //Ks    
    GLfloat mat_shininess[] = {100.0};                  //n
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

}

void init_texturas(){

	glGenTextures(10, texturas);
	glBindTexture(GL_TEXTURE_2D, texturas[0]);
	loadImageFile((char*)"resources/superficie.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, texturas[1]);
	loadImageFile((char*)"resources/plataforma.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, texturas[2]);
	loadImageFile((char*)"resources/meteor.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, texturas[3]);
	loadImageFile((char*)"resources/earth.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, texturas[6]);
	loadImageFile((char*)"resources/jupiter.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, texturas[4]);
	loadImageFile((char*)"resources/sky.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, texturas[5]);
	loadImageFile((char*)"resources/cockpit.png");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, texturas[7]);
	loadImageFile((char*)"resources/minimap.png");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

	glBindTexture(GL_TEXTURE_2D, texturas[8]);
	loadImageFile((char*)"resources/cursor.png");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

	glBindTexture(GL_TEXTURE_2D, texturas[9]);
	loadImageFile((char*)"resources/pin.png");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);					

	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_TEXTURE_2D);
}

void init_asteroides(){

	for(int i = 0; i < 3; i++){
		asteroides[i] = Asteroide();
		asteroides[i].translate.x = getRandomFloat(-50,-25);
		asteroides[i].translate.y = getRandomFloat(-50,50);
		asteroides[i].translate.z = getRandomFloat(0,50);

		float scale = getRandomFloat(0.25,3);
		asteroides[i].scale.x = scale;
		asteroides[i].scale.y = scale;
		asteroides[i].scale.z = scale;

		asteroides[i].rotation_speed = getRandomFloat(25,100);
		Vec3 rotation = Vec3(getRandomFloat(0,10), getRandomFloat(0,10), getRandomFloat(0,10));
		asteroides[i].rotate = rotation.normalize();
		asteroides[i].rotate_grados = getRandomFloat(0,360);
		asteroides[i].velocidad = getRandomFloat(6,20);
	}
}

void init(){
    cout << glGetString(GL_VERSION) << endl;
	cout << "Presiona la tecla 'm' para activar o desactivar el mapa" << endl;
	
	init_luces();
	init_texturas();
	init_asteroides();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void set_posicion_luces(){

	GLfloat posicion[] = {0.0, 0.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, posicion);

	GLfloat posicion2[]={camara_lookat_x + (-cos(deg_to_rad(angulo_guino)) * 3), camara_lookat_y + sin(deg_to_rad(angulo_guino)) * 3, camara_lookat_z, 1.0};
	GLfloat dir_central[]={sin(deg_to_rad(angulo_guino)) * sin(deg_to_rad(angulo_cabeceo)), cos(deg_to_rad(angulo_guino)) * sin(deg_to_rad(angulo_cabeceo)), cos(deg_to_rad(angulo_cabeceo))};
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_central);
	glLightfv(GL_LIGHT1,GL_POSITION,posicion2);		// Luz focal 

	GLfloat posicion3[]={camara_lookat_x + cos(deg_to_rad(angulo_guino)) * 3, camara_lookat_y + (-sin(deg_to_rad(angulo_guino)) * 3), camara_lookat_z, 1.0};
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_central);
	glLightfv(GL_LIGHT2,GL_POSITION,posicion3);		// Luz focal 

}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    gluLookAt(camara_pos_x,camara_pos_y,camara_pos_z,camara_lookat_x,camara_lookat_y,camara_lookat_z,0,0,1);
	set_posicion_luces();

	glBindTexture(GL_TEXTURE_2D, texturas[4]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPushMatrix();

		mi_esfera(50, 100);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[0]);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GLfloat point1[3] = {100.0, 100.0, 0.0};
		GLfloat point2[3] = {-100.0, 100.0, 0.0};
		GLfloat point3[3] = {-100.0, -100.0, 0.0};
		GLfloat point4[3] = {100.0, -100.0, 0.0};

		quad(point1, point2, point3, point4, 500, 500);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[2]);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for(int i = 0; i < 3; i++){

		glPushMatrix();

			glTranslatef(asteroides[i].translate.x, asteroides[i].translate.y, asteroides[i].translate.z);
			glRotatef(asteroides[i].rotate_grados, asteroides[i].rotate.x, asteroides[i].rotate.y, asteroides[i].rotate.z);
			glScalef(asteroides[i].scale.x, asteroides[i].scale.x, asteroides[i].scale.x);
			dibujar_asteroide();

		glPopMatrix();

	}

	glBindTexture(GL_TEXTURE_2D, texturas[1]);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glPushMatrix();						// PLATAFORMA	
		
		glNormal3f(0,0,1);
		glScalef(3,3,1);

		glBegin(GL_POLYGON);

			glTexCoord2f(0.75,1);
			glVertex3f(1,2,0.5);
			glTexCoord2f(1,0.75);
			glVertex3f(2,1,0.5);
			glTexCoord2f(1,0.25);
			glVertex3f(2,-1,0.5);
			glTexCoord2f(0.75,0);
			glVertex3f(1,-2,0.5);
			glTexCoord2f(0.25,0);
			glVertex3f(-1,-2,0.5);
			glTexCoord2f(0,0.25);
			glVertex3f(-2,-1,0.5);
			glTexCoord2f(0,0.75);
			glVertex3f(-2,1,0.5);
			glTexCoord2f(0.25,1);
			glVertex3f(-1,2,0.5);

		glEnd();

		GLfloat plat31[3] = {1,-2,0.5};
		GLfloat plat32[3] = {-1,-2,0.5};
		GLfloat plat33[3] = {-1,-2,0};
		GLfloat plat34[3] = {1,-2,0};

		glRotatef(180,0,0,1);

		quad(plat31, plat32, plat33, plat34, 20,20);

		glRotatef(90,0,0,1);

		quad(plat31, plat32, plat33, plat34, 20,20);

		glRotatef(90,0,0,1);

		quad(plat31, plat32, plat33, plat34, 20,20);

		glRotatef(90,0,0,1);

		quad(plat31, plat32, plat33, plat34, 20,20);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[3]);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glPushMatrix();

		glTranslatef(35,0,25);
		glRotatef(23.5,0,1,0);
		glRotatef(rotacion_tierra, 0,0,1);
		mi_esfera(20,3.0f);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[6]);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

	glPushMatrix();

		glTranslatef(-50,-25,60);
		glRotatef(rotacion_tierra * 0.75, 0,0,1);
		mi_esfera(20,15.0f);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[5]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

	if(cockpit){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		glPushMatrix();

			glTranslatef(camara_lookat_x, camara_lookat_y, camara_lookat_z);
			glRotatef(-angulo_guino, 0,0,1);
			glRotatef(-(angulo_cabeceo - 90),1,0,0);
			glScalef(0.2,0.2,0.2);

			GLfloat nave11[3] = {-3,0,-2};
			GLfloat nave12[3] = {3,0,-2};
			GLfloat nave13[3] = {3,0,2};
			GLfloat nave14[3] = {-3,0,2};

			quad(nave11, nave12, nave13, nave14, 30,30);

		glPopMatrix();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}

	glPushMatrix();

		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		char* text = (char*)"LUCES: ON";
		if(luces){
			text = (char*)"LUCES: ON";
		}
		else{
			text = (char*)"LUCES: OFF";
		}

		texto(viewport[2] / 2 - 50, viewport[3] - 30, text, BLANCO, GLUT_BITMAP_HELVETICA_18, false);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		if(mapa){
			glBindTexture(GL_TEXTURE_2D, texturas[7]);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);

			glBegin(GL_POLYGON);

				glTexCoord2f(0,0);
				glVertex2i(viewport[2] - 200,0);
				glTexCoord2f(0,1);
				glVertex2i(viewport[2] - 200,200);
				glTexCoord2f(1,1);
				glVertex2i(viewport[2],200);
				glTexCoord2f(1,0);
				glVertex2i(viewport[2],0);

			glEnd();

			glBindTexture(GL_TEXTURE_2D, texturas[8]);

			glPushMatrix();

				glTranslatef(viewport[2] - 100 + camara_pos_x, camara_pos_y + 100, 0);
				glRotatef(-angulo_guino, 0,0,1);

				glBegin(GL_POLYGON);

					glTexCoord2f(0,0);
					glVertex2i(-10,-10);
					glTexCoord2f(0,1);
					glVertex2i(-10,10);
					glTexCoord2f(1,1);
					glVertex2i(10,10);
					glTexCoord2f(1,0);
					glVertex2i(10,-10);

				glEnd();

			glPopMatrix();

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

		glBindTexture(GL_TEXTURE_2D, texturas[9]);

		if(cockpit){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);

			glPushMatrix();

				glTranslatef(viewport[2] / 2 - viewport[3] / 2.9, viewport[3] / 11,0);
				glScalef(viewport[3] / 250, viewport[3] / 250 ,1);
				glRotatef(100,0,0,1);
				glRotatef(-velocidad_nave * 10,0,0,1);

				glBegin(GL_POLYGON);

					glTexCoord2f(0,0);
					glVertex2i(-10,-3);
					glTexCoord2f(0,1);
					glVertex2i(-10,17);
					glTexCoord2f(1,1);
					glVertex2i(10,17);
					glTexCoord2f(1,0);
					glVertex2i(10,-3);

				glEnd();

			glPopMatrix();

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

    glutSwapBuffers();
}

void reshape(GLint w, GLint h){
	float ra = (float)w / h;
	glViewport(0, 0, w, h);

	// Elegir la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(40, ra, 0.1, 201);
}

void update(){
    static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);
	float tiempo_transcurrido = (hora_actual - hora_anterior) / 1000.0f;

	camara_pos_y += (velocidad_nave * tiempo_transcurrido) * sin(deg_to_rad(angulo_cabeceo)) * cos(deg_to_rad(angulo_guino));	
	camara_lookat_y = sin(deg_to_rad(angulo_cabeceo)) * cos(deg_to_rad(angulo_guino)) + camara_pos_y;
	camara_pos_x += (velocidad_nave * tiempo_transcurrido) * sin(deg_to_rad(angulo_guino)) * sin(deg_to_rad(angulo_cabeceo));
	camara_lookat_x = sin(deg_to_rad(angulo_cabeceo)) * sin(deg_to_rad(angulo_guino)) + camara_pos_x;
	camara_pos_z += (velocidad_nave * tiempo_transcurrido) * cos(deg_to_rad(angulo_cabeceo));
	camara_lookat_z = cos(deg_to_rad(angulo_cabeceo)) + camara_pos_z;

	for(int i = 0; i < 2; i++){
		asteroides[i].translate.x += tiempo_transcurrido * asteroides[i].velocidad;
		asteroides[i].rotate_grados += tiempo_transcurrido * asteroides[i].rotation_speed;
		if(asteroides[i].rotate_grados >= 360)
			asteroides[i].rotate_grados = 0;

		if(asteroides[i].translate.x >= 100)
			asteroides[i].translate.x = -100;
	}

	asteroides[2].translate.y -= tiempo_transcurrido * asteroides[2].velocidad;
	asteroides[2].rotate_grados += tiempo_transcurrido * asteroides[2].rotation_speed;
	if(asteroides[2].rotate_grados >= 360)
		asteroides[2].rotate_grados = 0;

	if(asteroides[2].translate.y <= -100)
		asteroides[2].translate.y = 100;

	rotacion_tierra += tiempo_transcurrido * 10;

	hora_anterior = hora_actual;

	glutPostRedisplay();
}

void onTimer(int tiempo){
	glutTimerFunc(tiempo, onTimer, tiempo);
	update();
}

// Atender el movimiento del raton
void onPassiveMotion(int x, int y){

	if(raton_y == 0){
		raton_y = y;
	}
	if(raton_x == 0){
		raton_x = x;
	}

	float delta_y = static_cast<float>(raton_y - y);
	raton_y = y;
	float delta_x = static_cast<float>(raton_x - x);
	raton_x = x;

	angulo_cabeceo -= delta_y * 0.2;
	angulo_guino -= delta_x * 0.2;

	if(angulo_cabeceo > 180.0){
		angulo_cabeceo = 180.0;
	}
	if(angulo_cabeceo < 0.0){
		angulo_cabeceo = 0.0;
	}
	if(angulo_guino > 360.0){
		angulo_guino = 0.0;
	}
	if(angulo_guino < -360.0){
		angulo_guino = 0.0;
	}
}

void onKey(unsigned char key, int x, int y){
	if(key == 'a' && velocidad_nave < VELOCIDAD_MAX){
		velocidad_nave += 2;
	}
	if(key == 'z' && velocidad_nave > 0.0){
		velocidad_nave -= 2;
	}

	if(key == 'l' && luces){
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		luces = !luces;
	}
	else if(key == 'l' && !luces){
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		luces = !luces;
	}

	if(key == 'c'){
		cockpit = !cockpit;
	}
	if(key == 'm'){
		if(mapa){
			cout << "Mapa desactivado" << endl;
		}
		else{
			cout << "Mapa activado" << endl;
		}
		mapa = !mapa;
	}
}

int main(int argc, char** argv){
	FreeImage_Initialise();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(100, 100);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(int(1000 / TASA_REFRESCO), onTimer, int(1000 / TASA_REFRESCO));
	glutPassiveMotionFunc(onPassiveMotion);
	glutKeyboardFunc(onKey);

	init();
	update();
	glutMainLoop();
	FreeImage_DeInitialise();
}