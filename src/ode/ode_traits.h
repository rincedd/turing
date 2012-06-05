/**
 * @file ode_traits.h
 * @date 05.06.2012
 */

#ifndef ODE_TRAITS_H_
#define ODE_TRAITS_H_

namespace ode
{

template<class ODE>
struct ode_traits
{
	typedef typename ODE::time_type time_type;
	typedef typename ODE::state_type state_type;
};

}

#endif /* ODE_TRAITS_H_ */
