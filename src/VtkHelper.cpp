#include "VtkHelper.h"

#include <NCollection_DataMap.hxx>
#include <NCollection_Shared.hxx>
#include <Standard_Transient.hxx>
#include <TopoDS_Shape.hxx>
#include <IVtk_Types.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeObject.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkPolyDataNormals.h>

vtkSmartPointer<vtkActor> VtkHelper::convertShapeToActor(const TopoDS_Shape& aShape)
{
	/// Create IVtkOCC_Shape instance (VIS wrapper for OCCT shape) and initialize it with TopoDS_Shape object containing the actual geometry
	auto aShapeImpl = new IVtkOCC_Shape(aShape);
	auto aShapeDS = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
	aShapeDS->SetShape(aShapeImpl);

	/// Display mode filter
	/// WireFrame or Shape mode
	auto DMFilter = vtkSmartPointer<IVtkTools_DisplayModeFilter>::New();
	DMFilter->AddInputConnection(aShapeDS->GetOutputPort());
	DMFilter->SetDisplayMode(DM_Shading);

	/// convert IVTK to VTK 
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	if (false) {
		auto normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
		normalGenerator->SetInputConnection(DMFilter->GetOutputPort());
		normalGenerator->ComputePointNormalsOn();
		normalGenerator->ComputeCellNormalsOn();
		normalGenerator->Update();
	}
	else {
		mapper->SetInputConnection(DMFilter->GetOutputPort());
	}
	
	/// Create actor and color selection
	auto Actor = vtkSmartPointer<vtkActor>::New();
	if (false) {
		auto Table = vtkSmartPointer<vtkLookupTable>::New();
		// Set colors table for 3D shapes
		double aRange[2];
		aRange[0] = MT_Undefined;
		aRange[1] = MT_ShadedFace;
		Table->Allocate(9);
		Table->SetNumberOfTableValues(9);
		Table->SetTableRange(aRange);
		Table->SetValueRange(0, 1);
		Table->SetTableValue(0, 0, 0, 0); // Undefined
		Table->SetTableValue(1, 0.5, 0.5, 0.5); // gray for IsoLine
		Table->SetTableValue(2, 1, 0, 0); // red for Free vertex
		Table->SetTableValue(3, 1, 1, 0); // yellow for Shared vertex
		Table->SetTableValue(4, 1, 0, 0); // red for Free edge
		Table->SetTableValue(5, 0, 1, 0); // green for Boundary edge (related to a single face)
		Table->SetTableValue(6, 1, 1, 0); // yellow for Shared edge (related to several faces)
		Table->SetTableValue(7, 1, 1, 0); // yellow for Wireframe face
		Table->SetTableValue(8, 1, 1, 0); // yellow for Shaded face
		IVtkTools::InitShapeMapper(mapper, Table);
		mapper->ScalarVisibilityOn();  // use colors from lookup table
	}
	else {
		mapper->ScalarVisibilityOff(); // use a color of actor’s property
		Actor->GetProperty()->SetColor(1, 0, 0);
	}
	mapper->Update();
	Actor->SetMapper(mapper);
	
	/// actor와 shapesource 바인딩이 반드시 필요함..
	IVtkTools_ShapeObject::SetShapeSource(aShapeDS, Actor);
	return Actor;
}