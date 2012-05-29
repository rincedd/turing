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
	ODE() {}
	virtual ~ODE() {}
	virtual void operator()(const double x, dvector& y, dvector& dydx) = 0;
};

} /* namespace ode */
#endif /* ODE_H_ */
