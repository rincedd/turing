/**
 * @file TuringModel.h
 * @date 29.05.2012
 */

#ifndef TURINGMODEL_H_
#define TURINGMODEL_H_

#include <largenet2.h>
#include <boost/function.hpp>

#include "types.h"
#include "../ode/ODE.h"

#include <boost/numeric/ublas/matrix_sparse.hpp>

class TuringModel: public ode::ODE<state_vector_t, time_value_t>
{
public:
	typedef boost::function<double(double, double)> coupling_function_t;
	typedef boost::numeric::ublas::compressed_matrix<double> laplacian_matrix_t;
	struct Params
	{
		double activator_d; ///< diffusion constant of activator species
		double diff_ratio_inhibitor_activator; ///< diffusion constant ratio between inhibitor and activator species
	};

	struct Coupling
	{
		coupling_function_t activatorCoupling;
		coupling_function_t inhibitorCoupling;
	};
public:
	TuringModel(largenet::Graph& g, Params p, Coupling coupling);
	virtual ~TuringModel();

	size_t dim() const
	{
		return 2 * graph_.numberOfNodes();
	}

	void recomputeLaplacian();

	void operator()(const state_type& y, state_type& dydx, const time_type x);

	const state_type& concentrations() const;
	state_type& concentrations();

	const largenet::Graph& graph() const { return graph_; }

	double patternAmplitude() const;
	double meanActivatorConcentration() const;
	double meanInhibitorConcentration() const;

private:
	largenet::Graph& graph_;
	Params par_;
	Coupling coupling_;
	state_type concentrations_; //< activator and inhibitor concentrations
	laplacian_matrix_t laplacian_;
};

#endif /* TURINGMODEL_H_ */
