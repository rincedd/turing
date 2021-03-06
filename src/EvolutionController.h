#ifndef EVOLUTIONCONTROLLER_H_
#define EVOLUTIONCONTROLLER_H_

#include "Controller.h"
#include "TuringOptions.h"
#include "model/TuringModel.h"
#include "ode/ode_traits.h"
#include "myrng/myrng.h"
#include "OStreamCollection.h"
#include <string>
#include <largenet2.h>

class EdgeWeights;

class EvolutionController: public Controller
{
public:
	EvolutionController(TuringOptions& opts);
	int exec();
	virtual ~EvolutionController();
	void writeInfo(std::ostream& strm) const;
private:
	void setup();
	void initWeights();
	void initConcentrations();

	void updateTopology();

	myrng::WELL1024a rng_;
	TuringOptions& opts_;
	largenet::Graph graph_;
	EdgeWeights* weights_;
	TuringModel* model_;
	OStreamCollection streams_;
};

#endif /* EVOLUTIONCONTROLLER_H_ */
