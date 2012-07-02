#include "TuringModel.h"
#include "EdgeWeights.h"
#include "DiffusionMatrix.h"
#include <cmath>
#include <largenet2/measures/spectrum.h>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/operation.hpp>

using namespace largenet;

TuringModel::TuringModel(Graph& g, EdgeWeights& w, Params p, Coupling coupling) :
		par_(p), concentrations_(2 * g.numberOfNodes(), 0.0), coupling_(
				coupling), diff_matrix_(g, w, par_.activator_d)
{
}

TuringModel::~TuringModel()
{
}

void TuringModel::refresh(const EdgeWeights& w)
{
	diff_matrix_.recompute(w);
}

const TuringModel::state_type& TuringModel::concentrations() const
{
	return concentrations_;
}

TuringModel::state_type& TuringModel::concentrations()
{
	return concentrations_;
}


void TuringModel::operator ()(const state_type& y, state_type& dydx,
		const time_type x)
{
	size_t K = dim() / 2;
	for (size_t i = 0; i < K; ++i)
	{
		dydx[i] = coupling_.activatorCoupling(y[i], y[i + K]);
		dydx[i + K] = coupling_.inhibitorCoupling(y[i], y[i + K]);
	}
	bnu::axpy_prod(diff_matrix_.get(), y, dydx, false);
}
