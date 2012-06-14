/**
 * @file DiffusionMatrix.cpp
 * @date 13.06.2012
 */

#include <boost/bind.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <largenet2/measures/spectrum.h>

#include "DiffusionMatrix.h"
#include "EdgeWeights.h"

using namespace largenet;
namespace bnu = boost::numeric::ublas;

DiffusionMatrix::DiffusionMatrix(const Graph& g, EdgeWeights& w,
		double prefactor) :
		prefactor_(prefactor)
{
	recompute(g, w);
	w.weight_changed.connect(
			boost::bind(&DiffusionMatrix::updateEdgeWeight, this, _1, _2, _3));
}

DiffusionMatrix::~DiffusionMatrix()
{
}

void DiffusionMatrix::recompute(const Graph& g, const EdgeWeights& w)
{
	d_matrix_t lw = -measures::weighted_laplacian(g, w);
	d_matrix_t l = -measures::laplacian(g);

	if ((matrix_.size1() != 2 * l.size1())
			|| (matrix_.size2() != 2 * l.size2()))
	{
		matrix_.resize(2 * l.size1(), 2 * l.size2(), false);
		matrix_.reserve(2 * l.nnz(), false);
	}
	bnu::subrange(matrix_, 0, l.size1(), 0, l.size2()).assign(l);
	bnu::subrange(matrix_, l.size1(), matrix_.size1(), l.size2(),
			matrix_.size2()).assign(lw);
	matrix_ *= prefactor_;
}

void DiffusionMatrix::updateEdgeWeight(const Edge& e, double old_weight,
		double new_weight)
{
	size_t i = e.source()->id(), j = e.target()->id();
	if (i == j)
		return;

	size_t K = matrix_.size1() / 2;

	matrix_(i + K, j + K) = prefactor_ * new_weight;
	matrix_(j + K, i + K) = prefactor_ * new_weight;
}
