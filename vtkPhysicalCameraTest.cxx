#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkMatrix3x3.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkstd/limits>

#include "vtkPhysicalCamera.h"

template<class A>
bool fuzzyCompare(A a, A b) 
{
  return fabs(a - b) < vtkstd::numeric_limits<A>::epsilon();
}

int main (int argc, char *argv[])
{
  //setup camera parameters
  
  //identity rotation
  vtkSmartPointer<vtkMatrix3x3> r = 
      vtkSmartPointer<vtkMatrix3x3>::New();
  
  //camera center is the origin
  double cameraCenter[3] = {0.0, 0.0, 0.0};
  
  //create the camera
  vtkSmartPointer<vtkPhysicalCamera> camera = 
      vtkSmartPointer<vtkPhysicalCamera>::New();
  camera->SetR(r);
  camera->SetCameraCenter(cameraCenter);

  double imageCenter[2] = {0.0, 0.0};
  double ray[3];
  camera->GetRay(imageCenter, ray);
  cout << "Ray through (0,0) is : (" << ray[0] << ", " << ray[1] << ", " << ray[2] << ")" << endl;
  
  if(!fuzzyCompare(ray[0], 0.0) || !fuzzyCompare(ray[1], 0.0) || !fuzzyCompare(ray[2], 1.0))
    {
    return EXIT_FAILURE;
    }
    
  return EXIT_SUCCESS;
}

