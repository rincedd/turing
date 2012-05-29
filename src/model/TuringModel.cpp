#include "TuringModel.h"
#include <largenet2.h>
#include <cassert>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <cmath>

using namespace largenet;
namespace bnu = boost::numeric::ublas;

TuringModel::TuringModel(Graph& g, Params p,
		coupling_function_t activator_coupling,
		coupling_function_t inhibitor_coupling) :
		graph_(g), par_(p), concentrations_(2 * g.numberOfNodes()), activator_coupling_(
				activator_coupling), inhibitor_coupling_(inhibitor_coupling), laplacian_(
				measures::laplacian(g))
{
	concentrations_ *= 0;
}

TuringModel::~TuringModel()
{
}

void TuringModel::initialize(const ode::dvector& activator,
		const ode::dvector& inhibitor)
{
	assert(activator.size() == graph_.numberOfNodes());
	assert(inhibitor.size() == graph_.numberOfNodes());
	bnu::subrange(concentrations_, 0, concentrations_.size() / 2) = activator;
	bnu::subrange(concentrations_, concentrations_.size() / 2,
			concentrations_.size()) = activator;
}

ode::dvector TuringModel::activator() const
{
	return bnu::subrange(concentrations_, 0, dim() / 2);
}

ode::dvector TuringModel::inhibitor() const
{
	return bnu::subrange(concentrations_, dim() / 2, concentrations_.size());
}

const ode::dvector& TuringModel::concentrations() const
{
	return concentrations_;
}

ode::dvector& TuringModel::concentrations()
{
	return concentrations_;
}

double TuringModel::patternAmplitude() const
{
	double mean_act = 0, mean_inh = 0;
	size_type K = dim() / 2;
	for (size_type i = 0; i < K; ++i)
	{
		mean_act += concentrations_[i];
		mean_inh += concentrations_[i + K];
	}
	mean_act /= K;
	mean_inh /= K;
	double a = 0;
	for (size_type i = 0; i < K; ++i)
	{
		a += (concentrations_[i] - mean_act) * (concentrations_[i] - mean_act)
				+ (concentrations_[i + K] - mean_inh)
						* (concentrations_[i + K] - mean_inh);
	}
	return sqrt(a);
}

void TuringModel::operator ()(const double x, ode::dvector& y,
		ode::dvector& dydx)
{
	size_t K = y.size() / 2;
	for (size_t i = 0; i < K; ++i)
	{
		bnu::matrix_row<largenet::measures::sparse_dmatrix_t> lr(laplacian_, i);
		double activator_diffusion = bnu::inner_prod(lr, bnu::subrange(y, 0, K));
		double inhibitor_diffusion = bnu::inner_prod(lr, bnu::subrange(y, K, y.size()));
//		double activator_diffusion = 0;
//		double inhibitor_diffusion = 0;
//		for (size_t j = 0; j < K; ++j)
//		{
//			activator_diffusion -= laplacian_(i, j) * y[j];
//			inhibitor_diffusion -= laplacian_(i, j) * y[j + K];
//		}
		dydx[i] = activator_coupling_(y[i], y[i + K])
				+ par_.activator_d * activator_diffusion;
		dydx[i + K] = inhibitor_coupling_(y[i], y[i + K])
				+ par_.diff_ratio_inhibitor_activator * par_.activator_d
						* inhibitor_diffusion;
	}
}
