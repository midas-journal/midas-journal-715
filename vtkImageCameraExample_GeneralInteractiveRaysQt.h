#ifndef MAINFORM_H
#define MAINFORM_H

#include <vtkSmartPointer.h>
class vtkSphereWidget;
class vtkLineSource;
class vtkImageCamera;

#include <QMainWindow>

// Forward Qt class declarations
class Ui_MainForm;

class MainForm : public QMainWindow
{
  Q_OBJECT
public:

  // Constructor/Destructor
  MainForm(const char* filename);  
  ~MainForm() {};

  std::string ImageFilename;
private:

  // Designer form
  Ui_MainForm *ui;
  vtkSmartPointer<vtkSphereWidget> SphereWidget;
  vtkSmartPointer<vtkLineSource> LineSource;
  vtkSmartPointer<vtkImageCamera> Camera;
};

#endif
