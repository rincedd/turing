// Software License for MTL
// 
// Copyright (c) 2007 The Trustees of Indiana University. 
//               2008 Dresden University of Technology and the Trustees of Indiana University.
//               2010 SimuNova UG (haftungsbeschränkt), www.simunova.com. 
// All rights reserved.
// Authors: Peter Gottschling and Andrew Lumsdaine
// 
// This file is part of the Matrix Template Library
// 
// See also license.mtl.txt in the distribution.

#ifndef MTL_IMAG_INCLUDE
#define MTL_IMAG_INCLUDE

#include <complex>
#include <boost/numeric/linear_algebra/identity.hpp>

namespace mtl {

namespace sfunctor {

    template <typename Value>
    struct imag
    {
	typedef Value result_type;

	static inline Value apply(const Value& v)
	{
	    using math::zero;
	    return zero(v);
	}
    };

    template <typename Value>
    struct imag<std::complex<Value> >
    {
	typedef std::complex<Value> result_type;

	static inline std::complex<Value> apply(const std::complex<Value>& v)
	{
	    return std::imag(v);
	}
    };

}

/// imaginary part of scalars (including non-complex)
template <typename Value>
inline typename sfunctor::imag<Value>::result_type imag(const Value& v)
{
    return sfunctor::imag<Value>::apply(v);
}



} // namespace mtl

#endif // MTL_IMAG_INCLUDE
