#include <vtkSmartPointer.h>
#include <vtkMatrix3x3.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkAxesActor.h>
#include <vtkImageData.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkJPEGWriter.h>
#include <vtkCellArray.h>
#include <vtkMath.h>
#include <vtkPointSource.h>
#include <vtkCameraActor.h>
#include <vtkCamera.h>

#include "vtkPhysicalCamera.h"

void CreateRotationY(double degree, vtkSmartPointer<vtkMatrix3x3> R);

int main (int argc, char *argv[])
{
  //Setup camera parameters
  
  //Create a rotation matrix
  vtkSmartPointer<vtkMatrix3x3> r = 
      vtkSmartPointer<vtkMatrix3x3>::New();
  CreateRotationY(45, r);
  
  double cameraCenter[3] = {0.0, 0.0, 0.0};

  // Create a camera
  vtkSmartPointer<vtkPhysicalCamera> camera = 
      vtkSmartPointer<vtkPhysicalCamera>::New();
  camera->SetR(r);
  camera->SetCameraCenter(cameraCenter);
  camera->SetFocalLength(200.0);
  
  double pix[2] = {400.0, 282.84};
  double ray[3];
  camera->GetRay(pix, ray);
  
  cout << "The ray through pixel (" << pix[0] << "," << pix[1] << " is (" << ray[0] << " " << ray[1] << " " << ray[2] << ")" << endl;
  
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