#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>

#include "vtkImageCamera.h"

int main (int argc, char *argv[])
{
  //verify command line arguments
  if(argc != 2)
    {
    cout << "Required arguments: InputFilename.jpg" << endl;
    return EXIT_FAILURE;
    }
  
  //parse command line arguments
  vtkstd::string inputFilename = argv[1];

  vtkSmartPointer<vtkJPEGReader> reader = 
    vtkSmartPointer<vtkJPEGReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  
  vtkSmartPointer<vtkImageCamera> imageCamera = 
      vtkSmartPointer<vtkImageCamera>::New();
  imageCamera->SetTextureImage(reader->GetOutput());
  imageCamera->SetRenderer(renderer);
  imageCamera->UpdateCameraActor();
  imageCamera->UpdateImageActor();
    
  return EXIT_SUCCESS;
}
