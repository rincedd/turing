/**
 * @file TuringApp.h
 * @date 04.06.2012
 */

#ifndef TURINGAPP_H_
#define TURINGAPP_H_

#include "TuringOptions.h"
#include "myrng1.3/myrng.h"
#include "OStreamCollection.h"
#include <string>
#include <largenet2.h>

class TuringModel;
class EdgeWeights;

class TuringApp
{
public:
	TuringApp();
	void parseCommandLine(int argc, char** argv);
	void setName(std::string name) { name_ = name; }
	std::string name() const { return name_; }
	int exec();
	virtual ~TuringApp();
protected:
	std::string makeFilename(std::string tag) const;
private:
	void setup();
	void initWeights();
	void initConcentrations();

	std::string name_;
	myrng::WELL1024a rng_;
	TuringOptions opts_;
	largenet::Graph graph_;
	EdgeWeights* weights_;
	TuringModel* model_;
	OStreamCollection streams_;
};

#endif /* TURINGAPP_H_ */
