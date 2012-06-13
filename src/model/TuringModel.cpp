#include "TuringModel.h"
#include "EdgeWeights.h"
#include "DiffusionMatrix.h"
#include <cmath>
#include <largenet2/measures/spectrum.h>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/operation.hpp>

using namespace largenet;

TuringModel::TuringModel(Graph& g, Params p, Coupling coupling) :
		graph_(g), par_(p), concentrations_(2 * g.numberOfNodes(), 0.0), coupling_(
				coupling), weights_(0), diff_matrix_(0)
{
	weights_ = new EdgeWeights(graph_.numberOfEdges(), graph_.numberOfNodes());
	BOOST_FOREACH(const Edge& e, graph_.edges())
	{
		weights_->setWeight(e, par_.diff_ratio_inhibitor_activator);
	}
	diff_matrix_ = new DiffusionMatrix(graph_, *weights_, par_.activator_d);
}

TuringModel::~TuringModel()
{
	delete diff_matrix_;
	delete weights_;
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
		dydx[i] = coupling_.activatorCoupling(y[i], y[i + K]);
		dydx[i + K] = coupling_.inhibitorCoupling(y[i], y[i + K]);
	}
	bnu::axpy_prod(diff_matrix_->get(), y, dydx, false);
}
