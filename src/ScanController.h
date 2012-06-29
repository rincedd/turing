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
#include <largenet2.h>

class EdgeWeights;

class ScanController: public Controller
{
public:
	ScanController(TuringOptions& opts);
	virtual ~ScanController();
	int exec();

private:
	void setup();
	void loadNetwork();
	void createModel(ode::ode_traits<TuringModel>::state_type& concentrations);

private:
	myrng::WELL1024a rng_;
	TuringOptions& opts_;
	largenet::Graph graph_;
	EdgeWeights* weights_;
	TuringModel* model_;
};

#endif /* SCANCONTROLLER_H_ */
