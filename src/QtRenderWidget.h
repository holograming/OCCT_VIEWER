#include "TopoDS_Shape.hxx"
#include <QVTKOpenGLNativeWidget.h>
#include <QMainWindow>


//VTK
#include <QVTKWidget.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>


class QtRenderWidget : public QMainWindow
{
  Q_OBJECT

public:
  explicit QtRenderWidget(QWidget *parent = 0);
  ~QtRenderWidget();
  

  QVTKOpenGLNativeWidget* render_widget;
  vtkSmartPointer<vtkRenderer> renderer;
  vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
};