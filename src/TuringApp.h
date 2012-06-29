/**
 * @file TuringApp.h
 * @date 04.06.2012
 */

#ifndef TURINGAPP_H_
#define TURINGAPP_H_

#include "TuringOptions.h"

class TuringApp
{
public:
	TuringApp();
	void parseCommandLine(int argc, char** argv);
	int exec();
	virtual ~TuringApp();
private:
	TuringOptions opts_;
};

#endif /* TURINGAPP_H_ */
