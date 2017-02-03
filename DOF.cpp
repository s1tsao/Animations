#include "stdafx.h"
#include "DOF.h"
#include <iostream>

DOF::DOF(double v, double Min, double Max)
{
	this->value = value;
	this->Min = Min;
	this->Max = Max;
}


DOF::~DOF()
{
}

void DOF::rot(double d) {
	setValue(value += d);
}

void DOF::setValue(double  value) {
	this->value = value;
	
	if (this->value < this->Min) {
		this->value = this->Min;
	}
	if (this->value > this->Max) {
		this->value = this->Max;
	}
	
}
double DOF::getValue() {
	return this->value;
}
void DOF::SetMinMax(double min, double max) {
	this->Min = min;
	this->Max = max;
}