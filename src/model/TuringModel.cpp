#include "TuringModel.h"
#include <largenet2.h>
#include <cassert>
#include <boost/numeric/ublas/vector_proxy.hpp>

using namespace largenet;

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
	boost::numeric::ublas::subrange(concentrations_, 0,
			concentrations_.size() / 2) = activator;
	boost::numeric::ublas::subrange(concentrations_, concentrations_.size() / 2,
			concentrations_.size()) = activator;
}

ode::dvector TuringModel::activator() const
{
	return boost::numeric::ublas::subrange(concentrations_, 0, dim() / 2);
}

ode::dvector TuringModel::inhibitor() const
{
	return boost::numeric::ublas::subrange(concentrations_, dim() / 2,
			concentrations_.size());
}

const ode::dvector& TuringModel::concentrations() const
{
	return concentrations_;
}

ode::dvector& TuringModel::concentrations()
{
	return concentrations_;
}

void TuringModel::operator ()(const double x, ode::dvector& y,
		ode::dvector& dydx)
{
	size_t K = y.size() / 2;
	assert(K == graph_.numberOfNodes());
	assert(y.size() == dim());
	assert(dydx.size() == dim());
	for (size_t i = 0; i < K; ++i)
	{
		double activator_diffusion = 0;
		double inhibitor_diffusion = 0;
		for (size_t j = 0; j < K; ++j)
		{
			activator_diffusion -= laplacian_(i, j) * y[j];
			inhibitor_diffusion -= laplacian_(i, j) * y[j + K];
		}
		dydx[i] = activator_coupling_(y[i], y[i + K])
				+ par_.activator_d * activator_diffusion;
		dydx[i + K] = inhibitor_coupling_(y[i], y[i + K])
				+ par_.diff_ratio_inhibitor_activator * par_.activator_d
						* inhibitor_diffusion;
	}
}
