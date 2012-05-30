#include "TuringModel.h"
#include <cmath>

using namespace largenet;
namespace bnu = boost::numeric::ublas;

TuringModel::TuringModel(Graph& g, Params p,
		coupling_function_t activator_coupling,
		coupling_function_t inhibitor_coupling) :
		graph_(g), par_(p), concentrations_(2 * g.numberOfNodes(), 0), activator_coupling_(
				activator_coupling), inhibitor_coupling_(inhibitor_coupling), laplacian_(
				measures::laplacian(g))
{
}

TuringModel::~TuringModel()
{
}

void TuringModel::recomputeLaplacian()
{
	laplacian_ = largenet::measures::laplacian(graph_);
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
	double mean_act = 0, mean_inh = 0;
	size_t K = dim() / 2;
	for (size_t i = 0; i < K; ++i)
	{
		mean_act += concentrations_[i];
		mean_inh += concentrations_[i + K];
	}
	mean_act /= K;
	mean_inh /= K;
	double a = 0;
	for (size_t i = 0; i < K; ++i)
	{
		a += (concentrations_[i] - mean_act) * (concentrations_[i] - mean_act)
				+ (concentrations_[i + K] - mean_inh)
						* (concentrations_[i + K] - mean_inh);
	}
	return sqrt(a);
}

void TuringModel::operator ()(const state_type& y, state_type& dydx,
		const time_type x)
{
	size_t K = y.size() / 2;
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
