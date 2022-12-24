#include "main.h"
#include "particula.h"
#include "proton.h"
#include "neutron.h"
#include "electron.h"
#include <iostream>
#include <thread>
#include <GLFW/glfw3.h>
#include <cmath>
#include <math.h>

#define M_PI       3.14159265358979323846
#define F_electro (9 * pow(10, 9))
#define F_gravedad (6.674 * pow(10, -11))
#define VLUZ       300000000

using namespace std;
#define ancho  900
#define alto  900
#define cant  150//4
#define cant_thread  30//1
double pos_pant[4] = { 0,0,0,0 };
int pausa = 0;
double v_imagen = pow(5,19/*1*/);
int ampl=-15  ;//17
int ampl_part = -22;//18
int num = 0;
double bordes = 6 * pow(7, (ampl - 1));
double r_xpos, r_ypos;
int turno = 0;
double t=0;

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void pintaPixel(double x, double y, string name, double radio);
void pintaLinea(double x1, double y1, double x2, double y2);
void Pantalla_Normal(particula particulas[]);
void Pantalla_Simulador(particula particulas[]);
void Pantalla_Simulador_Concreto();
void Pantalla_Simulador_Concreto(particula particulas[]);
void escribir_v_imagen();
void escribir_vel(particula particulas[]);
void escribir_pos(particula particulas[]);
void escribir_todo(particula particulas[]);
void escribir_todo_1(particula particulas);
void escribir_pantalla();
void iniciar_thread(particula particulas[], thread threads[]);
void relaciones_thread(particula particulas[], thread threads[]);
void relaciones2(particula particulas[],int i,int m);
void relaciones(particula particulas[]);
double gravedad(double masa1, double masa2, double distancia);
double electromagnetismo(double carga1, double carga2, double distancia);
double I_N_fuerte(double distancia);
double I_N_debil(string nombre1, string nombre2, double distancia);
void velocidad(particula *part1, particula *part2, double fuerza, double distancia);
void posicion(particula *part, particula particulas[]);

int main(int argc,char **argv){

	proton p;
	electron e;
	neutron n;
	particula particulas[cant];

	int opcion = 0;

	for (int i = 0; i < cant/3; i++) {
		double x = 0; double y = 0; double z = 0;
		int separacion = 11;
		bool hacer = false;
		if (hacer) {
			if (i % 4 == 0) { x = pow(7, ampl_part + separacion); y = pow(7, ampl_part + separacion); z = pow(7, ampl_part + separacion); }
			else if (i % 4 == 1) { x = -pow(7, ampl_part + separacion); y = pow(7, ampl_part + separacion); z = pow(7, ampl_part + separacion); }
			else if (i % 4 == 2) { x = -pow(7, ampl_part + separacion); y = -pow(7, ampl_part + separacion); z = pow(7, ampl_part + separacion); }
			else if (i % 4 == 3) { x = pow(7, ampl_part + separacion); y = -pow(7, ampl_part + separacion); z = pow(7, ampl_part + separacion); }
		}
		particulas[i * 3 + 0] = particula(rand() * pow(7, ampl_part)+x, rand() * pow(7, ampl_part)+y, 0, p.carga, p.masa, p.radio, p.nombre, p.id);
		particulas[i*3+1]= particula(rand() * pow(7, ampl_part)+x, rand() * pow(7, ampl_part)+y, 0, e.carga, e.masa, e.radio,e.nombre,e.id);
		particulas[i * 3 + 2] = particula(rand() * pow(7, ampl_part)+x, rand() * pow(7, ampl_part)+y, 0, n.carga, n.masa, n.radio,n.nombre,n.id);
	}
	
	cout << "Escribe Opcion\n1 Pantalla Normal\n2 Pantalla Simulador\n3 Pantalla Simulador Casos Concretos\n";
	cin >> opcion;
	if (opcion == 1) {
		Pantalla_Normal(particulas);
	}
	else if (opcion == 3) {
		Pantalla_Simulador_Concreto();
	}
	else {
		cout << "Escribe Opcion entre 1 y "<<cant<<"\n";
		cin >> opcion;
		opcion--;
		Pantalla_Simulador(particulas);
	}

	glfwTerminate();
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (num >= cant || num<0) {
		num = 0;
	}
	else {
		num += (int)yoffset;
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (num == cant) {
			num = 0;
			pos_pant[0] = 0;
			pos_pant[1] = 0;
			pos_pant[2] = 0;
			pos_pant[3] = 0;
		}
		else {
			num++;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		num = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		pos_pant[0] = 0;
		pos_pant[1] = 0;
		pos_pant[2] = 0;
		pos_pant[3] = 0;
	}
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	r_xpos = xpos;
	r_ypos = ypos;
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		pausa++;
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		pos_pant[0] = 0;
		pos_pant[1] = 0;
		pos_pant[2] = 0;
		pos_pant[3] = 0;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		ampl+= 1;
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		ampl+= -1;
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		if (bordes > 0) {
			bordes = 0;
		}
		else {
			bordes = 6*pow(7, (ampl-1));
		}
	}
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
		v_imagen = v_imagen/5;
	}
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
		v_imagen = v_imagen*5;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		pos_pant[2] -= pow(7, (ampl - 4));
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		pos_pant[3] += pow(7, (ampl - 4));
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		pos_pant[2] += pow(7, (ampl - 4));
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		pos_pant[3] += -pow(7, (ampl - 4));
	}
	//escribir_pantalla();
}
void pintaPixel(double x, double y, string name, double radio) {

	GLdouble p0x = (((x)-pos_pant[0]) /pow(7,ampl));
	GLdouble p0y = (((y)-pos_pant[1]) / pow(7, ampl));
	
	if((2*radio / pow(7, ampl+2))>=(double)1/alto){
		glBegin(GL_QUADS);
		if (name=="Neutron") {
			glColor3f(0, 0, 0);
		}
		else if (name=="Proton") {
			glColor3f(1, 0, 0);
		}
		else {
			glColor3f(0, 1, 0);
		}
		glVertex2d(p0x+ (radio / pow(7, ampl)), p0y+ (radio / pow(7, ampl)));
		glVertex2d(p0x- (radio / pow(7, ampl)), p0y+ (radio / pow(7, ampl)));
		glVertex2d(p0x- (radio / pow(7, ampl)), p0y- (radio / pow(7, ampl)));
		glVertex2d(p0x+ (radio / pow(7, ampl)), p0y- (radio / pow(7, ampl)));
		glEnd();
	}
	else {
		if (name == "Neutron") {
			glPointSize(8);
		}
		else if (name == "Proton") {
			glPointSize(10);
		}
		else {
			glPointSize(7);
		}
		glBegin(GL_POINTS);
		if (name == "Neutron") {
			glColor3f(0, 0, 0);
		}
		else if (name == "Proton") {
			glColor3f(1, 0, 0);
		}
		else {
			glColor3f(0, 1, 0);
		}
		glVertex2d(p0x, p0y);
		glEnd();
	}

}
void pintaLinea(double x1, double y1, double x2, double y2) {

	GLdouble p0x = (((x1)-pos_pant[0]) / pow(7, ampl));
	GLdouble p0y = (((y1)-pos_pant[1]) / pow(7, ampl));
	GLdouble p1x = (((x2)-pos_pant[0]) / pow(7, ampl));
	GLdouble p1y = (((y2)-pos_pant[1]) / pow(7, ampl));

	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2d(p0x, p0y);
	glVertex2d(p1x, p1y);
	glEnd();

}
void Pantalla_Normal(particula particulas[]) {
	int opcion = 0;
	thread threads[cant/cant_thread];
	while (opcion == 0) {
		cout << "0.0 Escribir todo\n0.n Escribir particula n\nn Ejecutar n veces\n-n Simulacion Especifica\n-1 Simulacion\n";
		cin >> opcion;
		if (opcion == 0) {
			cin >> opcion;
			if (opcion == 0) {
				escribir_todo(particulas);
			}
			else if (opcion > 0 && opcion <= cant) {
				escribir_todo_1(particulas[opcion]);
			}
			opcion = 0;
		}
		else if (opcion > 0) {
			for (int i = 0; i < opcion; i++) {
				if (cant >= 30) {
					iniciar_thread(particulas, threads);
					relaciones_thread(particulas, threads);
				}
				else {
					relaciones(particulas);
				}
				turno++;
				t = t + 1/v_imagen;
				if (turno % 10000 == 0) {
					cout << "Turno " << turno << " | Tiempo " << t << "\n";
				}
			}
			opcion = 0;
		}
		else if(opcion==-20) {
			opcion = 1;
		}
		else if(opcion==-1) {
			Pantalla_Simulador(particulas);
		}
		else {
			Pantalla_Simulador_Concreto(particulas);
		}

		cout << "Termina " << turno << "\n";
	}
}
void Pantalla_Simulador(particula particulas[]) {
	GLFWwindow* window;
	

	if (!glfwInit()) {
		return;
	}
	window = glfwCreateWindow(ancho, alto, "Hola", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 10, 100);
	int width = ancho;
	int height = alto;
	thread threads[cant/ cant_thread];
	
	while (!glfwWindowShouldClose(window)) {
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwGetFramebufferSize(window, &width, &height);
		
		if (pausa % 2 != 0) {
			if (cant >= 30) {
				iniciar_thread(particulas, threads);
				relaciones_thread(particulas, threads);
			}
			else {
				relaciones(particulas);
			}
			turno++;
			t = t + 1/v_imagen;
			if (turno % 10000 == 0) {
				cout << "Turno " << turno <<" | Tiempo "<<t<<"\n";
			}
		}

		if (num > 0 && num <= cant) {
			pos_pant[0] = particulas[num-1].p[0];
			pos_pant[1] = particulas[num-1].p[1];
		}

		if (bordes > 0) {
			pintaLinea(bordes, bordes, bordes, -bordes);
			pintaLinea(bordes, -bordes, -bordes, -bordes);
			pintaLinea(-bordes, -bordes, -bordes, bordes);
			pintaLinea(-bordes, bordes, bordes, bordes);
		}
		for (int i = 0; i < cant; i++) {
			pintaPixel(particulas[i].p[0], particulas[i].p[1], particulas[i].nombre, particulas[i].radio);
		}
		glfwSwapBuffers(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwPollEvents();
		
		pos_pant[0] += pos_pant[2];
		pos_pant[1] += pos_pant[3];
	}
}
void Pantalla_Simulador_Concreto() {
	proton p;
	neutron n;
	electron e;
	
	particula particulas[cant];
	particulas[0] = particula(0, 0, 0, p.carga, p.masa, p.radio, p.nombre,p.id);
	for (int i = 1; i < cant; i++) {
		particulas[i] = particula(rand() * pow(10, -18), rand() * pow(10, -18), 0, n.carga, n.masa, n.radio, n.nombre, n.id);
	}
	int turno = 0;
	thread threads[cant / cant_thread];
	cout << "n veces\n";
	int opcion = 0;
	cin >> opcion;
	while (opcion > 0) {
		if (cant >= 30) {
			iniciar_thread(particulas, threads);
			relaciones_thread(particulas, threads);
		}
		else {
			relaciones(particulas);
		}
		turno++;
		t = t + 1/v_imagen;
		if (turno % 100000 == 0) {
			cout << "Turno " << turno << " | Tiempo " << t << "\n";
		}
		opcion--;
	}

	GLFWwindow* window;

	if (!glfwInit()) {
		return;
	}
	window = glfwCreateWindow(ancho, alto, "Hola", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 10, 100);
	int width = ancho;
	int height = alto;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwGetFramebufferSize(window, &width, &height);

		if (pausa % 2 != 0) {
			particulas[0].p[0] = (r_xpos + pos_pant[0]-(ancho/2))/450 * pow(7, ampl);
			particulas[0].p[1] = -(r_ypos + pos_pant[1]-(alto/2))/450 * pow(7, ampl);

			if (cant >= 300) {
				iniciar_thread(particulas, threads);
				relaciones_thread(particulas, threads);
			}
			else {
				relaciones(particulas);
			}
			turno++;
			if (turno % 10000 == 0) {
				cout << "Termina " << turno << "\n";
			}
		}

		if (bordes > 0) {
			pintaLinea(bordes, bordes, bordes, -bordes);
			pintaLinea(bordes, -bordes, -bordes, -bordes);
			pintaLinea(-bordes, -bordes, -bordes, bordes);
			pintaLinea(-bordes, bordes, bordes, bordes);
		}

		if (num > 0 && num <= cant) {
			pos_pant[0] = particulas[num - 1].p[0];
			pos_pant[1] = particulas[num - 1].p[1];
		}

		for (int i = 0; i < cant; i++) {
			pintaPixel(particulas[i].p[0], particulas[i].p[1], particulas[i].nombre, particulas[i].radio);
		}
		glfwSwapBuffers(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwPollEvents();

		pos_pant[0] += pos_pant[2];
		pos_pant[1] += pos_pant[3];
	}
}
void Pantalla_Simulador_Concreto(particula particulas[]) {
	proton p;
	neutron n;
	electron e;

	GLFWwindow* window;

	if (!glfwInit()) {
		return;
	}
	window = glfwCreateWindow(ancho, alto, "Hola", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 10, 100);
	int width = ancho;
	int height = alto;
	thread threads[cant/ cant_thread];
	while (!glfwWindowShouldClose(window)) {
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwGetFramebufferSize(window, &width, &height);

		if (pausa % 2 != 0) {
			escribir_todo(particulas);
			if (cant >= 30) {
				iniciar_thread(particulas, threads);
				relaciones_thread(particulas, threads);
			}
			else {
				relaciones(particulas);
			}
			turno++;
			t = t + 1/v_imagen;
			if (turno % 1000000 == 0) {
				cout << "Turno " << turno << " | Tiempo " << t << "\n";
			}

		}

		if (bordes > 0) {
			pintaLinea(bordes, bordes, bordes, -bordes);
			pintaLinea(bordes, -bordes, -bordes, -bordes);
			pintaLinea(-bordes, -bordes, -bordes, bordes);
			pintaLinea(-bordes, bordes, bordes, bordes);
		}

		if (num > 0 && num <= cant) {
			pos_pant[0] = particulas[num - 1].p[0];
			pos_pant[1] = particulas[num - 1].p[1];
		}

		for (int i = 0; i < cant; i++) {
			pintaPixel(particulas[i].p[0], particulas[i].p[1], particulas[i].nombre, particulas[i].radio);
		}

		glfwSwapBuffers(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwPollEvents();

		pos_pant[0] += pos_pant[2];
		pos_pant[1] += pos_pant[3];
	}
}
void escribir_v_imagen() {
	int extra = 0;
	for (extra = 1; pow(5,extra) < v_imagen;extra++) {
	}
	cout << extra << endl;
}
void escribir_pos(particula particulas[]) {
	for (int i = 0; i < cant; i++) {
		cout << particulas[i].p[0] << "/" << particulas[i].p[1] << "/" << particulas[i].p[2] << "\n";
	}
}
void escribir_vel(particula particulas[]) {
	for (int i = 0; i < cant; i++) {
		cout << particulas[i].dv[0] << "/" << particulas[i].dv[1] << "/" << particulas[i].dv[2] << "\n";
	}
}
void escribir_todo(particula particulas[]) {
	for (int i = 0; i < cant; i++) {
		cout << "------------------------------\n";
		cout << "Nombre " << particulas[i].nombre << "\n\n";
		cout << "Posicion " << particulas[i].p[0] << "/" << particulas[i].p[1] << "/" << particulas[i].p[2] << "\n";
		cout << "Velocidad " << particulas[i].dv[0] << "/" << particulas[i].dv[1] << "/" << particulas[i].dv[2] << "\n";
		cout << "Carga " << particulas[i].carga << "\n";
		cout << "Masa " << particulas[i].masa << "\n";
		cout << "Radio " << particulas[i].radio << "\n";
		cout << "------------------------------\n";
	}
}
void escribir_todo_1(particula particulas) {
	cout << "------------------------------\n";
	cout << "Nombre " << particulas.nombre << "\n\n";
	cout<<"posicion "<< particulas.p[0] << "/" << particulas.p[1] << "/" << particulas.p[2] << "\n";
	cout << "velocidad " << particulas.dv[0] << "/" << particulas.dv[1] << "/" << particulas.dv[2] << "\n";
	cout << "carga " << particulas.carga << "\n";
	cout << "masa " << particulas.masa << "\n";
	cout << "radio " << particulas.radio << "\n";
	cout << "------------------------------\n";
}
void escribir_pantalla() {
	cout << v_imagen << "\n" << ampl << "\n";
}
void iniciar_thread(particula particulas[], thread threads[]) {
	for (int i = 0; i < (cant/ cant_thread); i++) {
		threads[i] = thread(relaciones2, particulas, i* cant_thread,(i+1)* cant_thread);
	}
}
void relaciones_thread(particula particulas[], thread threads[]) {
	for (int i = 0; i < (cant/ cant_thread); i++) {
		threads[i].join();
	}
}
void relaciones2(particula particulas[],int i,int m) {
	for (; i < m; i++) {
		for (int j = (i + 1); j < cant; j++) {
			if (i != j) {
				double distancia = sqrt(pow(particulas[j].p[0] - particulas[i].p[0], 2) + pow(particulas[j].p[1] - particulas[i].p[1], 2) + pow(particulas[j].p[2] - particulas[i].p[2], 2));
				if (particulas[i].radio + particulas[j].radio >= distancia) {
					double vec[3];
					vec[0] = particulas[i].p[0] - particulas[j].p[0];
					vec[1] = particulas[i].p[1] - particulas[j].p[1];
					vec[2] = particulas[i].p[2] - particulas[j].p[2];

					for (int t = 0; t < 3; t++) {
						double extra = particulas[j].dv[t];
						particulas[j].dv[t] = 2 * particulas[i].masa * particulas[i].dv[t] / (particulas[i].masa + particulas[j].masa) + (particulas[j].masa - particulas[i].masa) / (particulas[i].masa + particulas[j].masa) * particulas[j].dv[t];
						particulas[i].dv[t] = 2 * particulas[j].masa * extra / (particulas[j].masa + particulas[i].masa) + (particulas[i].masa - particulas[j].masa) / (particulas[j].masa + particulas[i].masa) * particulas[i].dv[t];

					}

					double d = -(vec[0] * particulas[j].dv[0] + vec[1] * particulas[j].dv[1] + vec[2] * particulas[j].dv[2]);
					d = -d / (pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));
					double d2 = -(vec[0] * particulas[i].dv[0] + vec[1] * particulas[i].dv[1] + vec[2] * particulas[i].dv[2]);
					d2 = -d2 / (pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));

					for (int t = 0; t < 3; t++) {
						particulas[i].dv[t] = (vec[t] * d2) + ((vec[t] * d2) - particulas[i].dv[t]);
						particulas[j].dv[t] = (vec[t] * d) + ((vec[t] * d) - particulas[j].dv[t]);
						if ((particulas[i].id == 1 && particulas[j].id == 2) || (particulas[i].id == 2 && particulas[j].id == 1)) {
							particulas[i].dv[t] = particulas[i].dv[t] / v_imagen;
							particulas[j].dv[t] = particulas[j].dv[t] / v_imagen;
						}
					}

					double separar = (particulas[i].radio + particulas[j].radio - distancia) / (vec[0] + vec[1] + vec[2]);
					particulas[i].p[0] = particulas[i].p[0] + vec[0] * separar / 2;
					particulas[i].p[1] = particulas[i].p[1] + vec[1] * separar / 2;
					particulas[i].p[2] = particulas[i].p[2] + vec[2] * separar / 2;
					particulas[j].p[0] = particulas[j].p[0] - vec[0] * separar / 2;
					particulas[j].p[1] = particulas[j].p[1] - vec[1] * separar / 2;
					particulas[j].p[2] = particulas[j].p[2] - vec[2] * separar / 2;
					distancia = particulas[i].radio + particulas[j].radio;
				}
				else {
					velocidad(&particulas[i], &particulas[j], gravedad(particulas[i].masa, particulas[j].masa, distancia), distancia);
					velocidad(&particulas[j], &particulas[i], gravedad(particulas[i].masa, particulas[j].masa, distancia), distancia);
					velocidad(&particulas[i], &particulas[j], electromagnetismo(particulas[i].carga, particulas[j].carga, distancia), distancia);
					velocidad(&particulas[j], &particulas[i], electromagnetismo(particulas[j].carga, particulas[i].carga, distancia), distancia);
					if ((particulas[i].id == 1 && particulas[j].id == 2) || (particulas[i].id == 2 && particulas[j].id == 1)) {
						double fuerza_fuerte = I_N_fuerte(distancia);
						velocidad(&particulas[i], &particulas[j], fuerza_fuerte, distancia);
						velocidad(&particulas[j], &particulas[i], fuerza_fuerte, distancia);
					}
					velocidad(&particulas[j], &particulas[i], I_N_debil(particulas[i].nombre, particulas[j].nombre, distancia), distancia);
					velocidad(&particulas[j], &particulas[i], I_N_debil(particulas[j].nombre, particulas[i].nombre, distancia), distancia);
				}
			}
		}
		for (int i = 0; i < cant; i++) {
			posicion(&particulas[i], particulas);
		}
	}
}
void relaciones(particula particulas[]) {
	for (int i = 0; i < (cant - 1); i++) {
		for (int j = (i + 1); j < cant; j++) {
			if (i != j) {
				double distancia = sqrt(pow(particulas[j].p[0] - particulas[i].p[0], 2) + pow(particulas[j].p[1] - particulas[i].p[1], 2) + pow(particulas[j].p[2] - particulas[i].p[2], 2));
				if (particulas[i].radio + particulas[j].radio >= distancia) {
					double vec[3];
					vec[0] = particulas[i].p[0] - particulas[j].p[0];
					vec[1] = particulas[i].p[1] - particulas[j].p[1];
					vec[2] = particulas[i].p[2] - particulas[j].p[2];

					for (int t = 0; t < 3; t++) {
						double extra = particulas[j].dv[t];
						particulas[j].dv[t] = 2 * particulas[i].masa * particulas[i].dv[t] / (particulas[i].masa + particulas[j].masa) + (particulas[j].masa - particulas[i].masa) / (particulas[i].masa + particulas[j].masa) * particulas[j].dv[t];
						particulas[i].dv[t] = 2 * particulas[j].masa * extra / (particulas[j].masa + particulas[i].masa) + (particulas[i].masa - particulas[j].masa) / (particulas[j].masa + particulas[i].masa) * particulas[i].dv[t];

					}

					double d = -(vec[0] * particulas[j].dv[0] + vec[1] * particulas[j].dv[1] + vec[2] * particulas[j].dv[2]);
					d = -d / (pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));
					double d2 = -(vec[0] * particulas[i].dv[0] + vec[1] * particulas[i].dv[1] + vec[2] * particulas[i].dv[2]);
					d2 = -d2 / (pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));

					for (int t = 0; t < 3; t++) {
						particulas[i].dv[t] = (vec[t] * d2) + ((vec[t] * d2) - particulas[i].dv[t]);
						particulas[j].dv[t] = (vec[t] * d) + ((vec[t] * d) - particulas[j].dv[t]);
						if ((particulas[i].id == 1 && particulas[j].id == 2) || (particulas[i].id == 2 && particulas[j].id == 1)) {
							particulas[i].dv[t] = particulas[i].dv[t] / v_imagen;
							particulas[j].dv[t] = particulas[j].dv[t] / v_imagen;
						}
					}

					double separar = (particulas[i].radio + particulas[j].radio - distancia) / (vec[0] + vec[1] + vec[2]);
					particulas[i].p[0] = particulas[i].p[0] + vec[0] * separar / 2;
					particulas[i].p[1] = particulas[i].p[1] + vec[1] * separar / 2;
					particulas[i].p[2] = particulas[i].p[2] + vec[2] * separar / 2;
					particulas[j].p[0] = particulas[j].p[0] - vec[0] * separar / 2;
					particulas[j].p[1] = particulas[j].p[1] - vec[1] * separar / 2;
					particulas[j].p[2] = particulas[j].p[2] - vec[2] * separar / 2;
					distancia = particulas[i].radio + particulas[j].radio;
				}
				else {
					velocidad(&particulas[i], &particulas[j], gravedad(particulas[i].masa, particulas[j].masa, distancia), distancia);
					velocidad(&particulas[j], &particulas[i], gravedad(particulas[i].masa, particulas[j].masa, distancia), distancia);
					velocidad(&particulas[i], &particulas[j], electromagnetismo(particulas[i].carga, particulas[j].carga, distancia), distancia);
					velocidad(&particulas[j], &particulas[i], electromagnetismo(particulas[j].carga, particulas[i].carga, distancia), distancia);
					if ((particulas[i].id == 1 && particulas[j].id == 2) || (particulas[i].id == 2 && particulas[j].id == 1)) {
						double fuerza_fuerte = I_N_fuerte(distancia);
						velocidad(&particulas[i], &particulas[j], fuerza_fuerte, distancia);
						velocidad(&particulas[j], &particulas[i], fuerza_fuerte, distancia);
					}
					velocidad(&particulas[j], &particulas[i], I_N_debil(particulas[i].nombre, particulas[j].nombre, distancia), distancia);
					velocidad(&particulas[j], &particulas[i], I_N_debil(particulas[j].nombre, particulas[i].nombre, distancia), distancia);
				}
			}
		}
	}
	for(int i = 0; i < cant; i++) {
		posicion(&particulas[i], particulas);
	}
	return;
}
double gravedad(double masa1, double masa2, double distancia) {

	double resultado = F_gravedad * masa1 * masa2 / distancia / distancia;
	return resultado;

}
double electromagnetismo(double carga1, double carga2, double distancia) {

	double resultado = F_electro * carga1 * -carga2 / distancia / distancia;
	return resultado;
}
double I_N_fuerte(double distancia) {
	proton p;
	if (distancia <= (pow(10, -10))/*(5 * pow(10, -10))*/) {
		double res = 3* F_electro *p.carga*p.carga/ pow(distancia,2);
		return res;
	}
	return 0;

}
double I_N_debil(string nombre1, string nombre2, double distancia) {

	double resultado=0;
	return resultado;

}
void velocidad(particula *part1,particula *part2, double fuerza,double distancia){
	double coso;
	double coso2;
	for (int i = 0; i < 3; i++) {
		coso = (*part2).p[i] - (*part1).p[i] / distancia * fuerza / (*part1).masa;
		coso = modf(coso / VLUZ,&coso2)*VLUZ;
		(*part1).dv[i] += coso / v_imagen;
	}
	
	
}
void posicion(particula *part, particula particulas[]) {
	if (bordes > 0) {
		if ((*part).p[0]+((*part).dv[0]/v_imagen) >= bordes || (*part).p[0]+((*part).dv[0] / v_imagen) <= (-bordes)) {
			(*part).dv[0] = -(*part).dv[0];
		}else if ((*part).p[1] + ((*part).dv[1] / v_imagen) >= bordes || (*part).p[1] + ((*part).dv[1] / v_imagen) <= (-bordes)) {
			(*part).dv[1] = -(*part).dv[1];
		}else if ((*part).p[2] + ((*part).dv[2] / v_imagen) >= bordes || (*part).p[2] + ((*part).dv[2] / v_imagen) <= (-bordes)) {
			(*part).dv[2] = -(*part).dv[2];
		}
		else {
			(*part).p[0] += (*part).dv[0] / v_imagen;
			(*part).p[1] += (*part).dv[1] / v_imagen;
			(*part).p[2] += (*part).dv[2] / v_imagen;
		}
	}
	else {
		(*part).p[0] += (*part).dv[0] / v_imagen;
		(*part).p[1] += (*part).dv[1] / v_imagen;
		(*part).p[2] += (*part).dv[2] / v_imagen;
	}
}