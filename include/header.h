#pragma once

#include "codebase.h"
#include <ctime>
#include <cstdlib>
#include <random>
using namespace cb;

class Asteroide{
	public:
		Vec3 translate = Vec3(0,0,0);
		Vec3 rotate = Vec3(0,0,0);
		float rotate_grados = 0;
		Vec3 scale = Vec3(1,1,1);
        float rotation_speed = 10;
        float velocidad = 5;
		float angulo_guino = 0;
		float angulo_cabeceo = 0;
		Asteroide(Vec3 t = Vec3(0,0,0), Vec3 r = Vec3(0,0,0), float g = 0, Vec3 s = Vec3(1,1,1)): translate(t), rotate(r), rotate_grados(g), scale(s) {}
};

class Bullet{
	public:
		Vec3 position = Vec3(0,0,0);
		float angulo_guino = 0;
		float angulo_cabeceo = 0;
		float distance_traveled = 0;
		Bullet(Vec3 pos, float ag, float ac): position(pos), angulo_guino(ag), angulo_cabeceo(ac), distance_traveled(0) {}
};

float deg_to_rad(float degrees){
	return degrees * PI / 180;
}

float getRandomFloat(float min, float max) {					//Extraido y modificado de https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    random_device rd;   
    mt19937 gen(rd());   
    uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

void mi_esfera(int resolucion, float radio) {
    glBegin(GL_TRIANGLES);

    for (int lat = 0; lat < resolucion; ++lat) {
        for (int lon = 0; lon < resolucion * 2; ++lon) {
            float theta1 = 2.0f * M_PI * static_cast<float>(lon) / (2.0f * resolucion);
            float theta2 = 2.0f * M_PI * static_cast<float>(lon + 1) / (2.0f * resolucion);
            float phi1 = M_PI * static_cast<float>(lat) / resolucion;
            float phi2 = M_PI * static_cast<float>(lat + 1) / resolucion;

            float x1 = radio * sin(phi1) * cos(theta1);
            float y1 = radio * sin(phi1) * sin(theta1);
            float z1 = radio * cos(phi1);

            float x2 = radio * sin(phi1) * cos(theta2);
            float y2 = radio * sin(phi1) * sin(theta2);
            float z2 = radio * cos(phi1);

            float x3 = radio * sin(phi2) * cos(theta1);
            float y3 = radio * sin(phi2) * sin(theta1);
            float z3 = radio * cos(phi2);

            float x4 = radio * sin(phi2) * cos(theta2);
            float y4 = radio * sin(phi2) * sin(theta2);
            float z4 = radio * cos(phi2);

            // Normales
            float nx1 = x1 / radio;
            float ny1 = y1 / radio;
            float nz1 = z1 / radio;

            float nx2 = x2 / radio;
            float ny2 = y2 / radio;
            float nz2 = z2 / radio;

            float nx3 = x3 / radio;
            float ny3 = y3 / radio;
            float nz3 = z3 / radio;

            float nx4 = x4 / radio;
            float ny4 = y4 / radio;
            float nz4 = z4 / radio;

            // Coordenadas de textura
            float s1 = static_cast<float>(lon) / (2.0f * resolucion);
            float t1 = static_cast<float>(lat) / resolucion;

            float s2 = static_cast<float>(lon + 1) / (2.0f * resolucion);
            float t2 = static_cast<float>(lat) / resolucion;

            float s3 = static_cast<float>(lon) / (2.0f * resolucion);
            float t3 = static_cast<float>(lat + 1) / resolucion;

            float s4 = static_cast<float>(lon + 1) / (2.0f * resolucion);
            float t4 = static_cast<float>(lat + 1) / resolucion;

            // Triangulo 1
            glNormal3f(nx1, ny1, nz1);
            glTexCoord2f(s1, 1 - t1);
			glVertex3f(x1, y1, z1);
            glNormal3f(nx2, ny2, nz2);
            glTexCoord2f(s2, 1 - t2);
			glVertex3f(x2, y2, z2);
            glNormal3f(nx3, ny3, nz3);
            glTexCoord2f(s3, 1 - t3);
			glVertex3f(x3, y3, z3);

            // Triangulo 2
            glNormal3f(nx2, ny2, nz2);
            glTexCoord2f(s2, 1 - t2);
            glVertex3f(x2, y2, z2);
            glNormal3f(nx4, ny4, nz4);
            glTexCoord2f(s4, 1 - t4);
            glVertex3f(x4, y4, z4);
            glNormal3f(nx3, ny3, nz3);
            glTexCoord2f(s3, 1 - t3);
            glVertex3f(x3, y3, z3);
        }
    }

    glEnd();
}

void dibujar_asteroide(){

	glPushMatrix();

		glScalef(0.75,1,0.5);

		GLfloat met11[3] = {-1,-1,0};
		GLfloat met12[3] = {2,-2,0};
		GLfloat met13[3] = {2,-3,2};
		GLfloat met14[3] = {-1,-2,2};

		quad(met11, met12, met13, met14, 20,20);

		GLfloat met21[3] = {-1,-1,0};
		GLfloat met22[3] = {-1,1,0};
		GLfloat met23[3] = {1,1,0};
		GLfloat met24[3] = {2,-2,0};

		quad(met21, met22, met23, met24, 20,20);

		GLfloat met31[3] = {2,-2,0};
		GLfloat met32[3] = {1,1,0};
		GLfloat met33[3] = {2.5,1,1};
		GLfloat met34[3] = {3.5,-2,1};

		quad(met31, met32, met33, met34, 20,20);

		GLfloat met41[3] = {-1,-2,2};
		GLfloat met42[3] = {-1,0.5,2};
		GLfloat met43[3] = {-1,1,0};
		GLfloat met44[3] = {-1,-1,0};

		quad(met41, met42, met43, met44, 20,20);

		GLfloat met51[3] = {-1,1,0};
		GLfloat met52[3] = {-1,2,1.5};
		GLfloat met53[3] = {2,2,1.5};
		GLfloat met54[3] = {1,1,0};

		quad(met51, met52, met53, met54, 20,20);

		GLfloat met61[3] = {1,1,0};
		GLfloat met62[3] = {2,2,1.5};
		GLfloat met63[3] = {2.5,1,1};
		GLfloat met64[3] = {2.5,1,1};

		quad(met61, met62, met63, met64, 20,20);

		GLfloat met71[3] = {2,-2,0};
		GLfloat met72[3] = {3.5,-2,1};
		GLfloat met73[3] = {2,-3,2};
		GLfloat met74[3] = {2,-3,2};

		quad(met71, met72, met73, met74, 20,20);

		GLfloat met81[3] = {-1,1,0};
		GLfloat met82[3] = {-1,0.5,2};
		GLfloat met83[3] = {-1,2,1.5};
		GLfloat met84[3] = {-1,2,1.5};

		quad(met81, met82, met83, met84, 20,20);

		GLfloat met91[3] = {-1,-2,2};
		GLfloat met92[3] = {2,-3,2};
		GLfloat met93[3] = {2,-1,5};
		GLfloat met94[3] = {(-133.0f/78.0f),(1.0f/6.0f),5};

		quad(met91, met92, met93, met94, 20,20);

		GLfloat met10_1[3] = {2,-3,2};
		GLfloat met10_2[3] = {3.5,-2,1};
		GLfloat met10_3[3] = {2,-1,5};
		GLfloat met10_4[3] = {2,-1,5};

		quad(met10_1, met10_2, met10_3, met10_4, 20,20);

		GLfloat met11_1[3] = {2.5,1,1};
		GLfloat met11_2[3] = {1.5,0.5,5};
		GLfloat met11_3[3] = {2,-1,5};
		GLfloat met11_4[3] = {3.5,-2,1};

		quad(met11_1, met11_2, met11_3, met11_4, 20,20);

		GLfloat met12_1[3] = {2.5,1,1};
		GLfloat met12_2[3] = {2,2,1.5};
		GLfloat met12_3[3] = {1.5,0.5,5};
		GLfloat met12_4[3] = {1.5,0.5,5};

		quad(met12_1, met12_2, met12_3, met12_4, 20,20);

		GLfloat met13_1[3] = {-1,2,1.5};
		GLfloat met13_2[3] = {-1.5,0.5,5};
		GLfloat met13_3[3] = {1.5,0.5,5};
		GLfloat met13_4[3] = {2,2,1.5};

		quad(met13_1, met13_2, met13_3, met13_4, 20,20);

		GLfloat met14_1[3] = {-1,2,1.5};
		GLfloat met14_2[3] = {-1,0.5,2};
		GLfloat met14_3[3] = {-1.5,0.5,5};
		GLfloat met14_4[3] = {-1.5,0.5,5};

		quad(met14_1, met14_2, met14_3, met14_4, 20,20);

		GLfloat met15_1[3] = {-1,0.5,2};
		GLfloat met15_2[3] = {-1,-2,2};
		GLfloat met15_3[3] = {(-133.0f/78.0f),(1.0f/6.0f),5};
		GLfloat met15_4[3] = {-1.5,0.5,5};

		quad(met15_1, met15_2, met15_3, met15_4, 20,20);

		GLfloat met16_1[3] = {(-133.0f/78.0f),(1.0f/6.0f),5};
		GLfloat met16_2[3] = {2,-1,5};
		GLfloat met16_3[3] = {1.5,0.5,5};
		GLfloat met16_4[3] = {-1.5,0.5,5};

		quad(met16_1, met16_2, met16_3, met16_4, 20,20);

	glPopMatrix();

}