#pragma once
#include <string>
#include <TopoDS_Shape.hxx>

namespace io {
	/// Read step, stp file
	TopoDS_Shape readSTP(const std::string file);
	
	bool writeSTP(const TopoDS_Shape& aShape, const std::string outfile);
};
