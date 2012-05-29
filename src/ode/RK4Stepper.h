/**
 * @file RK4Stepper.h
 * @date 29.05.2012
 */

#ifndef RK4STEPPER_H_
#define RK4STEPPER_H_

#include "Stepper.h"

namespace ode
{

/**
 * 
 *
 */
class RK4Stepper : public Stepper
{
public:
	RK4Stepper(double& x, dvector& y, dvector& dydx);
	virtual ~RK4Stepper();
	virtual double step(double h, ODE& rhs);
private:
	double& x_;
	dvector& y_;
	dvector& dydx_;
};

} /* namespace ode */
#endif /* RK4STEPPER_H_ */
