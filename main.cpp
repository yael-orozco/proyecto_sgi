#define PROYECTO "Trabajo Final"
#define TASA_REFRESCO 60
#define ASTEROIDES 5

#include <iostream>	
#include <vector>
#include "include/codebase.h"
#include "include/header.h"

using namespace std;
using namespace cb;

static float VELOCIDAD_MAX = 16.0;
float velocidad_nave = 0.0;
Vec3 camara_pos = Vec3(0,0,5);
Vec3 camara_lookat = Vec3();
Vec3 nave_lookat = Vec3();
float angulo_cabeceo = 90;
float angulo_guino = 0;
float ag_aux = 0;
float ac_aux = 0;
int raton_y = 0;
int raton_x = 0;
bool luces = true;
bool cockpit = true;
bool mapa = true;
bool passiveMotion = true;
static GLuint texturas[15];
Asteroide asteroides[ASTEROIDES];
float rotacion_tierra = 0;
vector<Bullet> bullets;

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

}

void init_texturas(){

	glGenTextures(15, texturas);
	glBindTexture(GL_TEXTURE_2D, texturas[0]);
	loadImageFile((char*)"resources/superficie.png");

	glBindTexture(GL_TEXTURE_2D, texturas[1]);  
	loadImageFile((char*)"resources/plataforma.jpg");

	glBindTexture(GL_TEXTURE_2D, texturas[2]);
	loadImageFile((char*)"resources/meteor.jpg");

	glBindTexture(GL_TEXTURE_2D, texturas[3]);
	loadImageFile((char*)"resources/earth.jpg");

	glBindTexture(GL_TEXTURE_2D, texturas[6]);
	loadImageFile((char*)"resources/jupiter.jpg");

	glBindTexture(GL_TEXTURE_2D, texturas[4]);
	loadImageFile((char*)"resources/sky.jpg");

	glBindTexture(GL_TEXTURE_2D, texturas[5]);
	loadImageFile((char*)"resources/cockpit.png");

	glBindTexture(GL_TEXTURE_2D, texturas[7]);
	loadImageFile((char*)"resources/minimap.png");

	glBindTexture(GL_TEXTURE_2D, texturas[8]);
	loadImageFile((char*)"resources/cursor.png");

	glBindTexture(GL_TEXTURE_2D, texturas[9]);
	loadImageFile((char*)"resources/pin.png");	

	glBindTexture(GL_TEXTURE_2D, texturas[10]);
	loadImageFile((char*)"resources/gold.jpg");

	glBindTexture(GL_TEXTURE_2D, texturas[11]);
	loadImageFile((char*)"resources/mexico.jpg");		

	glBindTexture(GL_TEXTURE_2D, texturas[12]);
	loadImageFile((char*)"resources/wall.png");

	glEnable(GL_TEXTURE_2D);
}

void init_asteroides(){

	for(int i = 0; i < ASTEROIDES; i++){
		asteroides[i] = Asteroide();
		asteroides[i].translate.x = getRandomFloat(-200,-25);
		asteroides[i].translate.y = getRandomFloat(-200,200);
		asteroides[i].translate.z = getRandomFloat(15,75);

		float scale = getRandomFloat(0.25,3);
		asteroides[i].scale.x = scale;
		asteroides[i].scale.y = scale;
		asteroides[i].scale.z = scale;

		asteroides[i].rotation_speed = getRandomFloat(25,100);
		Vec3 rotation = Vec3(getRandomFloat(0,10), getRandomFloat(0,10), getRandomFloat(0,10));
		asteroides[i].rotate = rotation.normalize();
		asteroides[i].rotate_grados = getRandomFloat(0,360);
		asteroides[i].velocidad = getRandomFloat(6,20);

		asteroides[i].angulo_cabeceo = getRandomFloat(75,105);
		asteroides[i].angulo_guino = getRandomFloat(0,360);
	}
}

void init(){
    cout << glGetString(GL_VERSION) << endl;
	cout << "Presiona la tecla 'm' para activar o desactivar el mapa" << endl;
	cout << "Presiona la tecla 'd' para disparar" << endl;
	cout << "Para mover la cabeza mantén presionado algún botón del ratón en lo que lo mueves" << endl;
	
	init_luces();
	init_texturas();
	init_asteroides();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void set_posicion_luces(){

	GLfloat posicion[] = {0.0, 0.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, posicion);

	GLfloat posicion2[]={camara_lookat.x + (-cos(deg_to_rad(ag_aux)) * 5), camara_lookat.y + sin(deg_to_rad(ag_aux)) * 5, camara_lookat.z, 1.0};
	GLfloat dir_central[]={sin(deg_to_rad(ag_aux)) * sin(deg_to_rad(ac_aux)), cos(deg_to_rad(ag_aux)) * sin(deg_to_rad(ac_aux)), cos(deg_to_rad(ac_aux))};
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_central);
	glLightfv(GL_LIGHT1,GL_POSITION,posicion2);		// Luz focal 

	GLfloat posicion3[]={camara_lookat.x + cos(deg_to_rad(ag_aux)) * 5, camara_lookat.y + (-sin(deg_to_rad(ag_aux)) * 5), camara_lookat.z, 1.0};
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_central);
	glLightfv(GL_LIGHT2,GL_POSITION,posicion3);		// Luz focal 

}

void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    gluLookAt(camara_pos.x,camara_pos.y,camara_pos.z,camara_lookat.x,camara_lookat.y,camara_lookat.z,0,0,1);
	set_posicion_luces();

    GLfloat mat_diffuse[] = {0.5,0.5,0.5,1.0};          //Kd
    GLfloat mat_specular[] = {0.8,0.8,0.8,1.0};         //Ks    
    GLfloat mat_shininess[] = {100.0};                  //n
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glBindTexture(GL_TEXTURE_2D, texturas[4]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	glPushMatrix();

		glRotatef(45,0,1,0);
		mi_esfera(50, 2500);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for(int i = 0; i < ASTEROIDES; i++){

		glPushMatrix();

			glTranslatef(asteroides[i].translate.x, asteroides[i].translate.y, asteroides[i].translate.z);
			glRotatef(asteroides[i].rotate_grados, asteroides[i].rotate.x, asteroides[i].rotate.y, asteroides[i].rotate.z);
			glScalef(asteroides[i].scale.x, asteroides[i].scale.x, asteroides[i].scale.x);
			dibujar_asteroide();

		glPopMatrix();

	}

	glBindTexture(GL_TEXTURE_2D, texturas[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

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

	glPushMatrix();

		glTranslatef(0,50,0);

		GLfloat bandera11[3] = {0.1,-0.1,0};
		GLfloat bandera12[3] = {0.1,0.1,0};
		GLfloat bandera13[3] = {0.1,0.1,20};
		GLfloat bandera14[3] = {0.1,-0.1,20};

		quad(bandera11, bandera12, bandera13, bandera14, 20,20);

		glRotatef(90,0,0,1);

		quad(bandera11, bandera12, bandera13, bandera14, 20,20);

		glRotatef(90,0,0,1);

		quad(bandera11, bandera12, bandera13, bandera14, 20,20);

		glRotatef(90,0,0,1);

		quad(bandera11, bandera12, bandera13, bandera14, 20,20);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[11]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

	glPushMatrix();

		glTranslatef(4.5,50,16.5);

		GLfloat bandera21[3] = {-4.5,0,-3.5};
		GLfloat bandera22[3] = {4.5,0,-3.5};
		GLfloat bandera23[3] = {4.5,0,3.5};
		GLfloat bandera24[3] = {-4.5,0,3.5};

		quad(bandera21, bandera22, bandera23, bandera24, 20,20);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[3]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glPushMatrix();

		glTranslatef(70,0,25);
		glRotatef(23.5,0,1,0);
		glRotatef(rotacion_tierra, 0,0,1);
		mi_esfera(20,3.0f);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[6]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

	glPushMatrix();

		glTranslatef(-100,-50,60);
		glRotatef(rotacion_tierra * 0.75, 0,0,1);
		mi_esfera(20,15.0f);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texturas[10]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    GLfloat mat_emission[] = {0.8,0.8,0.8};
	GLfloat mat_diffuse2[] = {0.7,0.7,0.7,1.0};         //Kd
    GLfloat mat_specular2[] = {1,1,1,1.0};         		//Ks    
    GLfloat mat_shininess2[] = {50.0};                  //n
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);  
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);  
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);  
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);

	for(int i = 0; i < bullets.size(); i++){
		glPushMatrix();

			glTranslatef(bullets.at(i).position.x, bullets.at(i).position.y, bullets.at(i).position.z);
			mi_esfera(10, 0.05);

		glPopMatrix();
	}

	GLfloat mat_emission2[] = {0,0,0};
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission2); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glBindTexture(GL_TEXTURE_2D, texturas[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glPushMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GLfloat point1[3] = {200.0, 200.0, 0.0};
		GLfloat point2[3] = {-200.0, 200.0, 0.0};
		GLfloat point3[3] = {-200.0, -200.0, 0.0};
		GLfloat point4[3] = {200.0, -200.0, 0.0};

		quad(point1, point2, point3, point4, 500, 500);

	glPopMatrix();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	if(cockpit){
		glBindTexture(GL_TEXTURE_2D, texturas[5]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		glPushMatrix();

			glTranslatef(camara_pos.x, camara_pos.y, camara_pos.z);

			if(passiveMotion){
				ac_aux = angulo_cabeceo;
				ag_aux = angulo_guino;
			}	

			glRotatef(-ag_aux, 0,0,1);
			glRotatef(-(ac_aux - 90),1,0,0);
			glTranslatef(0,1.5,0);	
			glPushMatrix();

				GLfloat nave1_1[3] = {-3,3,-2};
				GLfloat nave1_2[3] = {3,3,-2};
				GLfloat nave1_3[3] = {3,3,2};
				GLfloat nave1_4[3] = {-3,3,2};

				quad(nave1_1, nave1_2, nave1_3, nave1_4, 20, 20);

				glBindTexture(GL_TEXTURE_2D, texturas[12]);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

				glRotatef(90, 0, 0, 1);
				quad(nave1_1, nave1_2, nave1_3, nave1_4, 20, 20);

				glRotatef(90, 0, 0, 1);
				quad(nave1_1, nave1_2, nave1_3, nave1_4, 20, 20);

				glRotatef(90, 0, 0, 1);
				quad(nave1_1, nave1_2, nave1_3, nave1_4, 20, 20);

			glPopMatrix();

			glBindTexture(GL_TEXTURE_2D, texturas[1]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

			glPushMatrix();

				GLfloat nave2_1[3] = {3,3,-2};
				GLfloat nave2_2[3] = {-3,3,-2};
				GLfloat nave2_3[3] = {-3,-3,-2};
				GLfloat nave2_4[3] = {3,-3,-2};
				
				quad(nave2_1, nave2_2, nave2_3, nave2_4, 20, 20);

				glTranslatef(0, 0, 4);
				quad(nave2_1, nave2_2, nave2_3, nave2_4, 20, 20);

			glPopMatrix();

			glBindTexture(GL_TEXTURE_2D, texturas[9]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

			glPushMatrix();

				glTranslatef(-0.5, 0, -0.6);
				glRotatef(velocidad_nave * 10 - 100,0,1,0);
				glScalef(0.1, 0.1, 0.1);

				GLfloat aguja1[3] = {-0.5,3,-0.1};
				GLfloat aguja2[3] = {0.5,3,-0.1};
				GLfloat aguja3[3] = {0.5,3,0.9};
				GLfloat aguja4[3] = {-0.5,3,0.9};

				quad(aguja1, aguja2, aguja3, aguja4);

			glPopMatrix();

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

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

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

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

			glPushMatrix();

				float pos_y = camara_pos.y / 2 + 100;
				if(pos_y >= 200)
					pos_y = 200;

				float pos_x = viewport[2] - 100 + camara_pos.x / 2;
				if(pos_x >= viewport[2])
					pos_x = viewport[2];
				if(pos_x <= viewport[2] - 200)
					pos_x = viewport[2] - 200;

				glTranslatef(pos_x, pos_y, 0);
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

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

    glutSwapBuffers();
}

void reshape(GLint w, GLint h){
	float ra = (float)w / h;
	glViewport(0, 0, w, h);

	// Elegir la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, ra, 0.1, 5001);
}

void update(){
    static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);
	float tiempo_transcurrido = (hora_actual - hora_anterior) / 1000.0f;

	camara_pos.y += (velocidad_nave * tiempo_transcurrido) * sin(deg_to_rad(angulo_cabeceo)) * cos(deg_to_rad(angulo_guino));	
	camara_lookat.y = sin(deg_to_rad(angulo_cabeceo)) * cos(deg_to_rad(angulo_guino)) + camara_pos.y;
	camara_pos.x += (velocidad_nave * tiempo_transcurrido) * sin(deg_to_rad(angulo_guino)) * sin(deg_to_rad(angulo_cabeceo));
	camara_lookat.x = sin(deg_to_rad(angulo_cabeceo)) * sin(deg_to_rad(angulo_guino)) + camara_pos.x;
	camara_pos.z += (velocidad_nave * tiempo_transcurrido) * cos(deg_to_rad(angulo_cabeceo));
	camara_lookat.z = cos(deg_to_rad(angulo_cabeceo)) + camara_pos.z;

	for(int i = 0; i < ASTEROIDES; i++){
		asteroides[i].translate.y += (asteroides[i].velocidad * tiempo_transcurrido) * sin(deg_to_rad(asteroides[i].angulo_cabeceo)) * cos(deg_to_rad(asteroides[i].angulo_guino));
		asteroides[i].translate.x += (asteroides[i].velocidad * tiempo_transcurrido) * sin(deg_to_rad(asteroides[i].angulo_cabeceo)) * sin(deg_to_rad(asteroides[i].angulo_guino));
		asteroides[i].translate.z += (asteroides[i].velocidad * tiempo_transcurrido) * cos(deg_to_rad(asteroides[i].angulo_cabeceo));
		
		asteroides[i].rotate_grados += tiempo_transcurrido * asteroides[i].rotation_speed;
		if(asteroides[i].rotate_grados >= 360)
			asteroides[i].rotate_grados = 0;
	}

	for(int i = 0; i < bullets.size(); i++){
		bullets.at(i).position.y += (50 * tiempo_transcurrido) * sin(deg_to_rad(bullets.at(i).angulo_cabeceo)) * cos(deg_to_rad(bullets.at(i).angulo_guino));
		bullets.at(i).position.x += (50 * tiempo_transcurrido) * sin(deg_to_rad(bullets.at(i).angulo_cabeceo)) * sin(deg_to_rad(bullets.at(i).angulo_guino));	
		bullets.at(i).position.z += (50 * tiempo_transcurrido) * cos(deg_to_rad(bullets.at(i).angulo_cabeceo));

		bullets.at(i).distance_traveled += 1;

		if(bullets.at(i).distance_traveled >= 500){
			bullets.erase(bullets.begin() + i);
		}
	}

	rotacion_tierra += tiempo_transcurrido * 10;

	hora_anterior = hora_actual;

	glutPostRedisplay();
}

void onTimer(int tiempo){
	glutTimerFunc(tiempo, onTimer, tiempo);
	update();
}

void girar(int x, int y){

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
	if(angulo_cabeceo <= 0.1){
		angulo_cabeceo = 0.1;
	}
	if(angulo_guino > 360.0){
		angulo_guino = 0.0;
	}
	if(angulo_guino < -360.0){
		angulo_guino = 0.0;
	}

}

//Atender el movimiento del raton
void onPassiveMotion(int x, int y){
	angulo_cabeceo = ac_aux;
	angulo_guino = ag_aux;
	passiveMotion = true;
	girar(x, y);

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

	if(key == 'd'){
		Bullet bullet = Bullet(Vec3(camara_lookat.x, camara_lookat.y, camara_lookat.z), angulo_guino, angulo_cabeceo);
		bullets.push_back(bullet);
	}
}

void onMotion(int x, int y){
	passiveMotion = false;
	girar(x, y);
}

int main(int argc, char** argv){
	FreeImage_Initialise();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(100, 100);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(int(1000 / TASA_REFRESCO), onTimer, int(1000 / TASA_REFRESCO));
	glutPassiveMotionFunc(onPassiveMotion);
	glutMotionFunc(onMotion);
	glutKeyboardFunc(onKey);

	init();
	update();
	glutMainLoop();
	FreeImage_DeInitialise();
}