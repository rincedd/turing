/**
 * @file RK4Stepper.cpp
 * @date 29.05.2012
 */

#include "RK4Stepper.h"

namespace ode
{

RK4Stepper::RK4Stepper(double& x, dvector& y, dvector& dydx) :
		x_(x), y_(y), dydx_(dydx)
{
}

RK4Stepper::~RK4Stepper()
{
}

double RK4Stepper::step(double h, ODE& rhs)
{
	double h2 = 0.5 * h, h6 = h / 6.0;
	double xh = x_ + h2;
	dvector yt(y_.size()), dym(y_.size()), dyt(y_.size());
	rhs(y_, dydx_, x_);
	yt = y_ + h2 * dydx_;
	rhs(yt, dyt, xh);
	yt = y_ + h2 * dyt;
	rhs(yt, dym, xh);
	yt = y_ + h * dym;
	dym += dyt;
	rhs(yt, dyt, x_ + h);
	y_ = y_ + h6 * (dydx_ + dyt + 2.0 * dym);
	return h;
}

} /* namespace ode */
