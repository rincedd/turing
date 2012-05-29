/**
 * @file Stepper.h
 * @date 29.05.2012
 */

#ifndef STEPPER_H_
#define STEPPER_H_

namespace ode
{

class ODE;

/**
 * 
 *
 */
class Stepper
{
public:
	Stepper() {}
	virtual ~Stepper() {}
	virtual double step(double h, ODE& rhs) = 0;
};

} /* namespace ode */
#endif /* STEPPER_H_ */
