#pragma once
#include <string>
#include <TopoDS_Shape.hxx>
#include <vtkSmartPointer.h>
#include <vtkActor.h>

class VtkHelper {
public :
	/// Read step, stp file
	VtkHelper() = default;
	~VtkHelper() = default;

	static vtkSmartPointer<vtkActor> convertShapeToActor(const TopoDS_Shape& shape);
};
