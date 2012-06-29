/**
 * @file TuringApp.cpp
 * @date 04.06.2012
 */

#include "TuringApp.h"
#include "EvolutionController.h"
#include "ScanController.h"
#include <iostream>

TuringApp::TuringApp() :
		opts_()
{
}

TuringApp::~TuringApp()
{
}

void TuringApp::parseCommandLine(int argc, char** argv)
{
	try
	{
		opts_.parseCommandLine(argc, argv);
	} catch (TuringOptions::UsageError& e)
	{
		opts_.printHelpText(std::cout);
		exit(0);
	} catch (TuringOptions::ParsingError& e)
	{
		std::cerr << e.what() << "\n";
		exit(1);
	}
}

int TuringApp::exec()
{
	if (opts_.params().mode == "evolve")
	{
		EvolutionController ctrl(opts_);
		ctrl.setName("turing");
		return ctrl.exec();
	}
	else if (opts_.params().mode == "scan")
	{
		ScanController ctrl(opts_);
		ctrl.setName("turing");
		return ctrl.exec();
	}
	else
	{
		std::cerr << "Unknown operation mode.\n";
		return -1;
	}
}
