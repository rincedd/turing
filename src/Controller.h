/*
 * Controller.h
 *
 *  Created on: 29.06.2012
 *      Author: gerd
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <string>

class Controller
{
public:
	Controller() : name_("") {}
	virtual ~Controller() {}

	void setName(std::string name)
	{
		name_ = name;
	}

	std::string name() const
	{
		return name_;
	}

	virtual int exec() = 0;

private:
	std::string name_;
};

#endif /* CONTROLLER_H_ */
