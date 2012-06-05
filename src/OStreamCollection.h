/**
 * @file StreamCollection.h
 * @date 05.06.2012
 */

#ifndef OSTREAMCOLLECTION_H_
#define OSTREAMCOLLECTION_H_

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <boost/ptr_container/ptr_vector.hpp>

/**
 * 
 *
 */
class OStreamCollection
{
public:
	OStreamCollection() {}
	~OStreamCollection() {}
	std::ostream& openStream(std::string filename)
	{
		std::auto_ptr<std::ofstream> o(new std::ofstream(filename.c_str()));
		if (o->bad())
		{
			o->close();
			throw("Could not open " + filename + " for writing!\n");
		}
		streams_.push_back(o);
		return streams_.back();

	}
private:
	typedef boost::ptr_vector<std::ostream> ostream_ptr_v;
	ostream_ptr_v streams_;
};

#endif /* OSTREAMCOLLECTION_H_ */
