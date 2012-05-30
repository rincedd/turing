/**
 * @file ODE.h
 * @date 29.05.2012
 */

#ifndef ODE_H_
#define ODE_H_

#include <cstddef>

namespace ode
{

/**
 * 
 *
 */
template<class State, class Time>
class ODE
{
public:
	typedef State state_type;
	typedef Time time_type;
public:
	ODE()
	{
	}
	virtual ~ODE()
	{
	}
	virtual size_t dim() const = 0;
	/**
	 * Right-hand side of the ODE, in odeint signature
	 * @param[in] y current state vector
	 * @param[out] dydx computed change in state vector
	 * @param[in] x current value of independent variable
	 */
	virtual void operator()(const state_type& y, state_type& dydx, const time_type x) = 0;
};

} /* namespace ode */
#endif /* ODE_H_ */
