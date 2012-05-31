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

#ifndef MTL_VECTOR_MAP_VIEW_INCLUDE
#define MTL_VECTOR_MAP_VIEW_INCLUDE

#include <boost/shared_ptr.hpp>
#include <boost/numeric/mtl/concept/collection.hpp>
#include <boost/numeric/mtl/utility/category.hpp>
#include <boost/numeric/mtl/utility/range_generator.hpp>
#include <boost/numeric/mtl/utility/property_map.hpp>
#include <boost/numeric/mtl/utility/copy_expression_const_ref_container.hpp>
#include <boost/numeric/mtl/operation/sfunctor.hpp>
#include <boost/numeric/mtl/operation/tfunctor.hpp>
#include <boost/numeric/mtl/operation/conj.hpp>
#include <boost/numeric/mtl/vector/vec_expr.hpp>


namespace mtl { namespace vector { namespace detail {
    // Forward declaration for friend declaration
    template <typename, typename> struct map_value;
}}}

namespace mtl { namespace vector {

template <typename Functor, typename Vector> 
struct map_view 
  : public vec_expr< map_view<Functor, Vector> >
{
    typedef map_view                                   self;
    typedef vec_expr< self >                           expr_base;
    typedef Vector                                     other;

    typedef typename Functor::result_type              value_type;
    typedef typename Functor::result_type              const_reference;
    typedef typename Collection<Vector>::size_type     size_type;

    map_view (const Functor& functor, const other& ref) 
      : expr_base(*this), functor(functor), ref(ref) 
    {
	ref.delay_assign();
    }
    
    map_view (const Functor& functor, boost::shared_ptr<Vector> p) 
      : expr_base(*this), functor(functor), my_copy(p), ref(*p)
    {
	ref.delay_assign();
    }

    friend size_type inline num_rows(const self& v) { return num_rows(v.ref); }
    friend size_type inline num_cols(const self& v) { return num_cols(v.ref); }

    size_type stride() const { 	return ref.stride(); }
    const_reference operator() (size_type i) const { return functor(ref(i)); }
    const_reference operator[] (size_type i) const { return functor(ref[i]); }
    void delay_assign() const {}
    
    template <typename, typename> friend struct detail::map_value;

  protected:
    boost::shared_ptr<Vector>           my_copy;
  public:
    Functor           functor;
    // ref is a const& if Vector is a true vector and a copy if it is an expression
    typename mtl::traits::copy_expression_const_ref_container<Vector>::type ref;
};


template <typename Functor, typename Vector> 
inline std::size_t size(const map_view<Functor, Vector>& v)
{     return mtl::vector::size(v.ref); }

// ================
// Free functions
// ================


    namespace detail {

	template <typename Functor, typename Vector> 
	struct map_value
	{
	    typedef typename Vector::key_type                      key_type;
	    typedef typename vector::map_view<Functor, Vector>::value_type value_type;
    	
	    map_value(vector::map_view<Functor, Vector> const& map_vector) 
		: map_vector(map_vector), its_value(map_vector.ref) 
	    {}

	    value_type operator() (key_type const& key) const
	    {
		return map_vector.functor(its_value(key));
	    }

	  protected:
	    vector::map_view<Functor, Vector> const&   map_vector;
	    typename ::mtl::traits::const_value<Vector>::type its_value;
        };

    } // detail

}} // namespace mtl::vector



namespace mtl { namespace traits {

    // ================
    // Property maps
    // ================

    template <typename Functor, typename Vector> 
    struct index<vector::map_view<Functor, Vector> >
	: public index<Vector>
    {};

    template <typename Functor, typename Vector> 
    struct const_value<vector::map_view<Functor, Vector> >
    {
	typedef vector::detail::map_value<Functor, Vector>  type;
    };


    // ================
    // Range generators
    // ================

    // Use range_generator of original vector
    template <typename Tag, typename Functor, typename Vector> 
    struct range_generator<Tag, vector::map_view<Functor, Vector> >
	: public range_generator<Tag, Vector>
    {};

}} // mtl::traits

namespace mtl { namespace vector {

template <typename Scaling, typename Vector>
struct scaled_view
    : public map_view<tfunctor::scale<Scaling, typename Vector::value_type>, Vector>
{
    typedef tfunctor::scale<Scaling, typename Vector::value_type>  functor_type;
    typedef map_view<functor_type, Vector>                         base;

    explicit scaled_view(const Scaling& scaling, const Vector& vector)
      : base(functor_type(scaling), vector)
    {}
    
    explicit scaled_view(const Scaling& scaling, boost::shared_ptr<Vector> p)
      : base(functor_type(scaling), p)
    {}
};

// added by Hui Li
template <typename Vector, typename RScaling>
struct rscaled_view
  : public map_view<tfunctor::rscale<typename Vector::value_type, RScaling>, Vector>
{
    typedef tfunctor::rscale<typename Vector::value_type, RScaling>  functor_type;
    typedef map_view<functor_type, Vector>                          base;
	
    explicit rscaled_view(const Vector& vector, const RScaling& rscaling)
      : base(functor_type(rscaling), vector)
    {}
	
    explicit rscaled_view(boost::shared_ptr<Vector> p, const RScaling& rscaling)
      : base(functor_type(rscaling), p)
    {}
};
	

// added by Hui Li
template <typename Vector, typename Divisor>
struct divide_by_view
  : public map_view<tfunctor::divide_by<typename Vector::value_type, Divisor>, Vector>
{
    typedef tfunctor::divide_by<typename Vector::value_type, Divisor>  functor_type;
    typedef map_view<functor_type, Vector>                             base;
	
    explicit divide_by_view(const Vector& vector, const Divisor& div)
      : base(functor_type(div), vector)
    {}
	
    explicit divide_by_view(boost::shared_ptr<Vector> p, const Divisor& div)
      : base(functor_type(div), p)
    {}
};
	

template <typename Vector>
struct conj_view
  : public map_view<mtl::sfunctor::conj<typename Vector::value_type>, Vector>
{
    typedef mtl::sfunctor::conj<typename Vector::value_type>            functor_type;
    typedef map_view<functor_type, Vector>                         base;

    explicit conj_view(const Vector& vector)
      : base(functor_type(), vector)
    {}
    
    explicit conj_view(boost::shared_ptr<Vector> p)
      : base(functor_type(), p)
    {}
};

template <typename Vector>
struct negate_view
  : public map_view<mtl::sfunctor::negate<typename Vector::value_type>, Vector>
{
    typedef mtl::sfunctor::negate<typename Vector::value_type>            functor_type;
    typedef map_view<functor_type, Vector>                         base;

    explicit negate_view(const Vector& vector)
      : base(functor_type(), vector)
    {}
    
    explicit negate_view(boost::shared_ptr<Vector> p)
      : base(functor_type(), p)
    {}
};



}} // namespace mtl::vector


#endif // MTL_VECTOR_MAP_VIEW_INCLUDE
