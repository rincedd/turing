/**
 * @file TuringModel.h
 * @date 29.05.2012
 */

#ifndef TURINGMODEL_H_
#define TURINGMODEL_H_

#include <largenet2/measures/spectrum.h>
#include <boost/function.hpp>

#include "../ode/ODE.h"

namespace largenet
{
class Graph;
}

class TuringModel: public ode::ODE
{
public:
	typedef boost::function<double(double u, double v)> coupling_function_t;
	struct Params
	{
		double activator_d; ///< diffusion constant of activator species
		double diff_ratio_inhibitor_activator; ///< diffusion constant ratio between inhibitor and activator species
	};

public:
	TuringModel(largenet::Graph& g, Params p,
			coupling_function_t activator_coupling,
			coupling_function_t inhibitor_coupling);
	virtual ~TuringModel();

	size_type dim() const
	{
		return 2 * graph_.numberOfNodes();
	}

	void recomputeLaplacian();
	void initialize(const ode::dvector& activator,
			const ode::dvector& inhibitor);

	void operator()(const double x, ode::dvector& y, ode::dvector& dydx);

	ode::dvector activator() const;
	ode::dvector inhibitor() const;
	const ode::dvector& concentrations() const;
	ode::dvector& concentrations();

private:
	largenet::Graph& graph_;
	Params par_;
	coupling_function_t activator_coupling_, inhibitor_coupling_;
	ode::dvector concentrations_; //< activator and inhibitor concentrations
	largenet::measures::sparse_dmatrix_t laplacian_;
};

inline void TuringModel::recomputeLaplacian()
{
	laplacian_ = largenet::measures::laplacian(graph_);
}

#endif /* TURINGMODEL_H_ */
