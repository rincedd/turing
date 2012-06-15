/**
 * @file TuringApp.h
 * @date 04.06.2012
 */

#ifndef TURINGAPP_H_
#define TURINGAPP_H_

#include "TuringOptions.h"
#include "model/TuringModel.h"
#include "ode/ode_traits.h"
#include "myrng1.3/myrng.h"
#include "loggers/Loggers.h"
#include "OStreamCollection.h"
#include <string>
#include <largenet2.h>

class EdgeWeights;

class TuringApp
{
public:
	typedef Loggers<ode::ode_traits<TuringModel>::state_type,
			ode::ode_traits<TuringModel>::time_type> loggers_t;
public:
	TuringApp();
	void parseCommandLine(int argc, char** argv);
	void setName(std::string name)
	{
		name_ = name;
	}
	std::string name() const
	{
		return name_;
	}
	int exec();
	virtual ~TuringApp();
protected:
	std::string makeFilename(std::string tag) const;
private:
	void setup();
	void initWeights();
	void initConcentrations();

	void integrate(ode::ode_traits<TuringModel>::time_type from,
			ode::ode_traits<TuringModel>::time_type to,
			ode::ode_traits<TuringModel>::time_type dt, loggers_t& loggers);

	std::string name_;
	myrng::WELL1024a rng_;
	TuringOptions opts_;
	largenet::Graph graph_;
	EdgeWeights* weights_;
	TuringModel* model_;
	OStreamCollection streams_;
};

#endif /* TURINGAPP_H_ */
