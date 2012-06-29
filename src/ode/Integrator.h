/**
 * @file Integrator.h
 * @date 04.06.2012
 */

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include "ode_traits.h"
#include <boost/numeric/odeint.hpp>
#include <boost/ref.hpp>
#include <vector>

namespace bno = boost::numeric::odeint;

namespace ode
{

namespace detail
{
template<class State, class Time>
class NullLogger
{
public:
	void operator()(const State& state, const Time t) {}
};

}

/**
 * 
 *
 */
template<class _ODE>
class Integrator
{
public:
	typedef typename ode_traits<_ODE>::time_type time_type;
	typedef typename ode_traits<_ODE>::state_type state_type;
public:
	Integrator(_ODE& system, state_type& state, double atol, double rtol) :
			sys_(system), state_(state), atol_(atol), rtol_(rtol)
	{
	}

	template<class Logger>
	size_t integrate(const double t0, const double t1, const double dt,
			Logger* logger)
	{
		typedef bno::runge_kutta_dopri5<state_vector_t> error_stepper_t;
		typedef bno::result_of::make_controlled<error_stepper_t>::type stepper_t;
		stepper_t stepper = bno::make_controlled<error_stepper_t>(atol_, rtol_);
		if (logger != 0)
			return bno::integrate_adaptive(stepper, boost::ref(sys_), state_,
					t0, t1, dt, boost::ref(*logger));
		else
			return bno::integrate_adaptive(stepper, boost::ref(sys_), state_,
					t0, t1, dt);
	}

	size_t integrate(const double t0, const double t1, const double dt)
	{
		return integrate<detail::NullLogger<state_type, time_type> >(t0, t1, dt, 0);
	}

private:
	_ODE& sys_;
	state_type& state_;
	double atol_, rtol_;
};

}
#endif /* INTEGRATOR_H_ */
