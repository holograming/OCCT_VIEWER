#include "FileIO.h"
#include <BRepTools.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

namespace io {
	TopoDS_Shape readSTP(const std::string file) {
		TopoDS_Shape shape;
		STEPControl_Reader reader;
		reader.ReadFile(file.c_str());
		reader.TransferRoots();
		shape = reader.OneShape();
		BRepTools::Clean(shape);
		return shape;
	}

	bool writeSTP(const TopoDS_Shape& aShape, const std::string outFile)
	{
		STEPControl_Writer writer;
		writer.Transfer(aShape, STEPControl_AsIs);
		IFSelect_ReturnStatus ret;
		if ((ret = writer.Write(outFile.c_str())) == IFSelect_ReturnStatus::IFSelect_RetDone) {
			return true;
		}
		std::cout << "Write result : " <<  ret << std::endl;
		return false;
	}

}