#include "TuringModel.h"
#include <cmath>
#include <largenet2/measures/spectrum.h>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/operation.hpp>

using namespace largenet;

TuringModel::TuringModel(Graph& g, Params p,
		coupling_function_t activator_coupling,
		coupling_function_t inhibitor_coupling) :
		graph_(g), par_(p), concentrations_(2 * g.numberOfNodes(), 0.0), activator_coupling_(
				activator_coupling), inhibitor_coupling_(inhibitor_coupling), laplacian_()
{
	recomputeLaplacian();
}

TuringModel::~TuringModel()
{
}

void TuringModel::recomputeLaplacian()
{
	laplacian_matrix_t temp = -measures::laplacian(graph_);
	if ((laplacian_.size1() != 2 * temp.size1())
			|| (laplacian_.size2() != 2 * temp.size2()))
	{
		laplacian_.resize(2 * temp.size1(), 2 * temp.size2(), false);
		laplacian_.reserve(2 * temp.nnz(), false);
	}
	bnu::subrange(laplacian_, 0, temp.size1(), 0, temp.size2()).assign(temp);
	bnu::subrange(laplacian_, temp.size1(), laplacian_.size1(), temp.size2(),
			laplacian_.size2()).assign(temp) *=
			par_.diff_ratio_inhibitor_activator;
	laplacian_ *= par_.activator_d;
}

const TuringModel::state_type& TuringModel::concentrations() const
{
	return concentrations_;
}

TuringModel::state_type& TuringModel::concentrations()
{
	return concentrations_;
}

double TuringModel::patternAmplitude() const
{
	double mean_act = meanActivatorConcentration(), mean_inh =
			meanInhibitorConcentration();
	size_t K = dim() / 2;
	double a = 0;
	for (size_t i = 0; i < K; ++i)
	{
		a += (concentrations_[i] - mean_act) * (concentrations_[i] - mean_act)
				+ (concentrations_[i + K] - mean_inh)
						* (concentrations_[i + K] - mean_inh);
	}
	return sqrt(a);
}

double TuringModel::meanActivatorConcentration() const
{
	return bnu::sum(
			bnu::subrange(concentrations_, 0, concentrations_.size() / 2))
			/ (0.5 * concentrations_.size());
}

double TuringModel::meanInhibitorConcentration() const
{
	return bnu::sum(
			bnu::subrange(concentrations_, concentrations_.size() / 2,
					concentrations_.size())) / (0.5 * concentrations_.size());
}

void TuringModel::operator ()(const state_type& y, state_type& dydx,
		const time_type x)
{
	size_t K = dim() / 2;
	for (size_t i = 0; i < K; ++i)
	{
		dydx[i] = activator_coupling_(y[i], y[i + K]);
		dydx[i + K] = inhibitor_coupling_(y[i], y[i + K]);
	}
	bnu::axpy_prod(laplacian_, y, dydx, false);
}
