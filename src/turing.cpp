#include "TuringApp.h"

int main(int argc, char **argv)
{
	TuringApp a;
	a.parseCommandLine(argc, argv);
	return a.exec();
}
