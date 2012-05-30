/**
 * @file ODE.h
 * @date 29.05.2012
 */

#ifndef ODE_H_
#define ODE_H_

#include <boost/numeric/ublas/vector.hpp>

namespace ode
{

typedef boost::numeric::ublas::vector<double> dvector;

/**
 * 
 *
 */
class ODE
{
public:
	typedef dvector::size_type size_type;
public:
	ODE() {}
	virtual ~ODE() {}
	virtual size_type dim() const = 0;
	/**
	 * Right-hand side of the ODE, in odeint signature
	 * @param[in] y current state vector
	 * @param[out] dydx computed change in state vector
	 * @param[in] x current value of independent variable
	 */
	virtual void operator()(const dvector& y, dvector& dydx, const double x) = 0;
};

} /* namespace ode */
#endif /* ODE_H_ */
