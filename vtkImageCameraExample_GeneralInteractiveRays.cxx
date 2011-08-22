#include <QApplication>

#include "vtkImageCameraExample_GeneralInteractiveRaysQt.h"

int main(int argc, char *argv[])
{
  //verify command line arguments
  if(argc != 2)
    {
    cout << "Required arguments: Filename.jpg";
    return EXIT_FAILURE;
    }
    
  //parse command line arguments
  vtkstd::string filename = argv[1];
  
  QApplication app(argc, argv);
  MainForm form(filename.c_str());
  form.show();
  return app.exec();
}
