#include <vtkSmartPointer.h>
#include <vtkMatrix3x3.h>
#include <vtkPointSource.h>
#include <vtkMath.h>

#include "vtkPhysicalCamera.h"

void CreateRotationY(double degree, vtkSmartPointer<vtkMatrix3x3> R);

int main (int argc, char *argv[])
{
  //create a point to project
  vtkSmartPointer<vtkPointSource> pointSource = 
      vtkSmartPointer<vtkPointSource>::New();
  pointSource->SetNumberOfPoints(1);
  pointSource->SetCenter(1.0, 2.0, 3.0);
  pointSource->SetRadius(0);
  pointSource->Update();
  
  vtkPolyData* polydata = pointSource->GetOutput();

  // Setup camera parameters
  // Create a rotation matrix
  vtkSmartPointer<vtkMatrix3x3> r = 
      vtkSmartPointer<vtkMatrix3x3>::New();
  CreateRotationY(45, r);

  double cameraCenter[3] = {10.0, 20.0, 30.0};

  // Create a camera
  vtkSmartPointer<vtkPhysicalCamera> camera = 
      vtkSmartPointer<vtkPhysicalCamera>::New();
  camera->SetR(r);
  camera->SetCameraCenter(cameraCenter);
  camera->SetFocalLength(200.0);
    
  //projection
  double p[3];
  polydata->GetPoint(0,p);
  double pixel[2];
  camera->ProjectPoint(p, pixel);
  cout << "The projection of (" << cameraCenter[0] << ", " << cameraCenter[1] << ", " << cameraCenter[2] << ") using the camera is (" << pixel[0] << ", " << pixel[1] << ")" << endl;
  
  return EXIT_SUCCESS;
}

void CreateRotationY(double degree, vtkSmartPointer<vtkMatrix3x3> R)
{
  //Rx rotates the y-axis towards the z-axis
  //Ry rotates the z-axis towards the x-axis
  //Rz rotates the x-axis towards the y-axis
  R->Identity();
  double ang = vtkMath::RadiansFromDegrees(degree);
  R->SetElement(0,0, cos(ang));
  R->SetElement(0,2, sin(ang));
  R->SetElement(2,0, -sin(ang));
  R->SetElement(2,2, cos(ang)); 
  
}