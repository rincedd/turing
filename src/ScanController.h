/*
 * ScanController.h
 *
 *  Created on: 29.06.2012
 *      Author: gerd
 */

#ifndef SCANCONTROLLER_H_
#define SCANCONTROLLER_H_

#include "Controller.h"
#include "TuringOptions.h"
#include "model/TuringModel.h"
#include "ode/ode_traits.h"
#include "myrng/myrng.h"
#include "OStreamCollection.h"
#include <largenet2.h>

class EdgeWeights;

class ScanController: public Controller
{
public:
	ScanController(TuringOptions& opts);
	virtual ~ScanController();
	int exec();
	void writeInfo(std::ostream& strm) const;

private:
	void setup();
	void loadNetwork();
	TuringModel* createModel(ode::ode_traits<TuringModel>::state_type& concentrations);
	void initConcentrations();
	void updateTopology(double scan_par);

private:
	myrng::WELL1024a rng_;
	TuringOptions& opts_;
	largenet::Graph graph_;
	EdgeWeights* weights_, *orig_weights_;
	TuringModel* model_;
	OStreamCollection streams_;
};

#endif /* SCANCONTROLLER_H_ */
