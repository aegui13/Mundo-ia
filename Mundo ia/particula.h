#pragma once
#include <iostream>
using namespace std;
class particula
{
public:int id=0;
public:string nombre;
public:double carga = 0;
public:double masa = 0;
public:double radio = 0;

public:double p[3] = { 0, 0, 0 };

public:double dv[3] = { 0, 0, 0 };

particula() {}

particula(double px, double py, double pz, double carga, double masa, double radio, string nombre, int id) {
	p[0] = px;
	p[1] = py;
	p[2] = pz;
	this->carga = carga;
	this->masa = masa;
	this->radio = radio;
	this->nombre = nombre;
	this->id = id;
}
};

