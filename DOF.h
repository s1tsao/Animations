#pragma once
class DOF
{
public:
	DOF(double, double,  double);
	~DOF();
	void setValue(double );
	double getValue();
	void SetMinMax(double, double );
	void rot(double);
	double value = 0;
	double Min =  -10000;
	double Max = 10000;
};

