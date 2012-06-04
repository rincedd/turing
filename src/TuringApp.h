/**
 * @file TuringApp.h
 * @date 04.06.2012
 */

#ifndef TURINGAPP_H_
#define TURINGAPP_H_

#include "TuringOptions.h"
#include "myrng1.3/myrng.h"
#include <largenet2.h>

class TuringModel;

/**
 * 
 *
 */
class TuringApp
{
public:
	TuringApp();
	void parseCommandLine(int argc, char** argv);
	int exec();
	virtual ~TuringApp();
private:
	void setup();
	void initConcentrations();

	myrng::WELL1024a rng_;
	TuringOptions opts_;
	largenet::Graph graph_;
	TuringModel* model_;
};

#endif /* TURINGAPP_H_ */
