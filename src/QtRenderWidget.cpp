#include "QtRenderWidget.h"

//VTK
#include <QVTKInteractor.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <QVTKInteractorAdapter.h>
#include <vtkBoxWidget2.h>
#include <vtkWidgetRepresentation.h>
#include <vtkCamera.h>

QtRenderWidget::QtRenderWidget(QWidget *parent) : QMainWindow(parent)
{
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    render_widget = new QVTKOpenGLNativeWidget;
    render_widget->SetRenderWindow(renderWindow);

    auto const render_interactor = render_widget->GetInteractor();
    render_interactor->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());
    render_interactor->SetRenderWindow(renderWindow);

    setCentralWidget(render_widget);
}

QtRenderWidget::~QtRenderWidget() {

}
