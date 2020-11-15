#include <QApplication>
#include "QtRenderWidget.h"
#include "FileIO.h"
#include "VtkHelper.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
int main()
{
	QApplication app(__argc, __argv);
	
	/// STP READ Start
	auto aShape = io::readSTP("F:/akakakak.stp");
	if (aShape.IsNull()) {
		std::cout << "Failed to load stp file";
		return 1;
	}
	
	/// Convert Topology to VtkActor
	auto actor  = VtkHelper::convertShapeToActor(aShape);
	if (!actor) {
		std::cout << "Failed to convert stp file";
		return 1;
	}
	
    //io::writeSTP(aShape, "F:/.stp");

	/// Declare Qt renderer for vtkdata
	QtRenderWidget widget;
	widget.show();
	widget.renderer->AddActor(actor);
	widget.renderer->ResetCamera();
	widget.renderer->Render();
	return app.exec();
}