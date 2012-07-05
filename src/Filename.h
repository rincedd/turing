/*
 * Filename.h
 *
 *  Created on: 02.07.2012
 *      Author: gerd
 */

#ifndef FILENAME_H_
#define FILENAME_H_

#include <string>

class Filename
{
public:
	template<class Options>
	Filename(const std::string& name, const Options& opts,
			const std::string& tag = "") :
			value_(name)
	{
		if (value_ != "")
			value_ += "-";
		value_ += opts.toStr();
		if (tag != "")
			value_ += "-" + tag;
		value_ += ".dat";
	}

	operator std::string() const
	{
		return value_;
	}

private:
	std::string value_;
};

#endif /* FILENAME_H_ */
