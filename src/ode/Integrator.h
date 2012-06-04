/**
 * @file Integrator.h
 * @date 04.06.2012
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <boost/numeric/odeint.hpp>
#include <vector>

namespace bno = boost::numeric::odeint;

namespace ode
{

namespace detail
{

template<class _ODE, class _Logger>
class Loggers
{
private:
	typedef typename std::vector<_Logger> logger_v;
public:
	void operator()(const typename _ODE::state_type& state,
			const typename _ODE::time_type t)
	{
		for (typename logger_v::iterator it = loggers_.begin(); it != loggers_.end();
				++it)
		{
			(*it)->log(state, t);
		}
	}
	void registerLogger(_Logger o)
	{
		loggers_.push_back(o);
	}
private:
	logger_v loggers_;
};

}

/**
 * 
 *
 */
template<class _ODE, class _Logger>
class Integrator
{
public:
	typedef typename _ODE::time_type time_type;
	typedef typename _ODE::state_type state_type;
public:
	Integrator(_ODE& system, state_type& state, double atol, double rtol) :
			sys_(system), state_(state), atol_(atol), rtol_(rtol), loggers_()
	{
	}

	virtual ~Integrator()
	{
	}

	void registerLogger(_Logger* o)
	{
		loggers_.registerLogger(o);
	}

	size_t integrate(const double t0, const double t1, const double dt)
	{
		typedef bno::runge_kutta_dopri5<state_vector_t> error_stepper_t;
		typedef bno::result_of::make_controlled<error_stepper_t>::type stepper_t;
		stepper_t stepper = bno::make_controlled<error_stepper_t>(atol_, rtol_);
		return bno::integrate_adaptive(stepper, sys_, state_, t0, t1, dt,
				loggers_);
	}

private:
	_ODE& sys_;
	state_type& state_;
	double atol_, rtol_;
	detail::Loggers<_ODE, _Logger> loggers_;
};

}
#endif /* INTEGRATOR_H_ */
