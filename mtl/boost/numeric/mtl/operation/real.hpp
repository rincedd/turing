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

#ifndef MTL_REAL_INCLUDE
#define MTL_REAL_INCLUDE

#include <complex>
#include <boost/numeric/mtl/interface/vpt.hpp>

namespace mtl {

namespace sfunctor {

    template <typename Value>
    struct real
    {
	typedef Value result_type;

	static inline Value apply(const Value& v)
	{
	    return v;
	}
    };

    template <typename Value>
    struct real<std::complex<Value> >
    {
	typedef std::complex<Value> result_type;

	static inline result_type apply(const std::complex<Value>& v)
	{
	    return std::real(v);
	}
    };
}

/// real part of scalars (including non-complex)
template <typename Value>
inline typename sfunctor::real<Value>::result_type real(const Value& v)
{	
	vampir_trace<3> tracer;
    return sfunctor::real<Value>::apply(v);
}


} // namespace mtl

#endif // MTL_REAL_INCLUDE
