/**
 * @file ublas_bindings.h
 * @date 31.05.2012
 */

#ifndef UBLAS_BINDINGS_H_
#define UBLAS_BINDINGS_H_

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/odeint.hpp>
#include <cmath>
#include <cassert>

typedef boost::numeric::ublas::vector<double> dvector_t;

inline dvector_t operator/(const dvector_t& a, const dvector_t& b)
{
	assert(a.size() == b.size());
	dvector_t result(a);
	for (dvector_t::size_type i = 0; i < a.size(); ++i)
	{
		result[i] /= b[i];
	}
	return result;
}

inline dvector_t abs(const dvector_t& v)
{
	dvector_t result(v);
	for (dvector_t::size_type i = 0; i < v.size(); ++i)
	{
		result[i] = abs(result[i]);
	}
	return result;
}

namespace boost
{
namespace numeric
{
namespace odeint
{

template<>
struct is_resizeable<dvector_t>
{
	typedef boost::true_type type;
	const static bool value = type::value;
};

template<>
struct vector_space_reduce<dvector_t>
{
	template<class Value, class Op>
	Value operator()(const dvector_t &v, Op op, Value init)
	{
		for (dvector_t::size_type i = 0; i < v.size(); ++i)
		{
			init = op(init, v[i]);
		}
		return init;
	}
};

}
}
}

#endif /* UBLAS_BINDINGS_H_ */
