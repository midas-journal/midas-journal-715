#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkAxesActor.h>
#include <vtkCommand.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkVectorText.h>
#include <vtkMath.h>
#include <vtkMatrix3x3.h>
#include <vtkSliderRepresentation2D.h>
#include <vtkSliderWidget.h>
#include <vtkLinearSubdivisionFilter.h>

#include "vtkImageCamera.h"
#include "vtkPhysicalCamera.h"

class ImageCameraSliderCallback : public vtkCommand
{
  public:
    static ImageCameraSliderCallback *New()
    {
      return new ImageCameraSliderCallback;
    }
    virtual void Execute(vtkObject *caller, unsigned long, void*);
    
    void SetImageCamera(vtkImageCamera* cam);
  private:
    ImageCameraSliderCallback():ImageCamera(0) {}
    vtkImageCamera* ImageCamera;
};

void ImageCameraSliderCallback::Execute(vtkObject *caller, unsigned long, void*)
{
  vtkSliderWidget *sliderWidget = 
      reinterpret_cast<vtkSliderWidget*>(caller);
  
  double dist = static_cast<vtkSliderRepresentation*>(sliderWidget->GetRepresentation())->GetValue();
  
  //cout << "New dist: " << dist << endl;
  this->ImageCamera->SetDistanceFromCamera(dist);
  this->ImageCamera->UpdateImageActor();
  
}

void ImageCameraSliderCallback::SetImageCamera(vtkImageCamera* cam) 
{
  this->ImageCamera = cam;
}

void CreateRotationY(double degree, vtkSmartPointer<vtkMatrix3x3> R);

int main (int argc, char *argv[])
{
  //verify command line arguments
  if(argc != 2)
    {
    cout << "Required arguments: Filename.jpg";
    return EXIT_FAILURE;
    }
    
  //parse command line arguments
  vtkstd::string filename = argv[1];
  
  vtkSmartPointer<vtkAxesActor> worldOrigin = 
      vtkSmartPointer<vtkAxesActor>::New();
  
  vtkSmartPointer<vtkJPEGReader> reader = 
      vtkSmartPointer<vtkJPEGReader>::New();
  reader->SetFileName(filename.c_str());
  reader->Update();
  
  vtkSmartPointer<vtkImageCamera> camera = 
      vtkSmartPointer<vtkImageCamera>::New();
  
  vtkSmartPointer<vtkMatrix3x3> r = 
      vtkSmartPointer<vtkMatrix3x3>::New();
   CreateRotationY(45, r);
  
  double cameraCenter[3] = {1.0, 2.0, 3.0};
  camera->SetR(r);
  camera->SetCameraCenter(cameraCenter);
  camera->SetFocalLength(200);
  camera->SetTextureImage(reader->GetOutput());
  
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  camera->SetRenderer(renderer);
  camera->UpdateCameraActor();
  camera->UpdateImageActor();
  
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(800,800);
  
  renderWindow->AddRenderer(renderer);
  
  renderer->AddActor(worldOrigin);
  
  renderer->ResetCamera();
  //renderer->SetBackground(1,1,1);//white
  renderer->SetBackground(0,0,0);//black
  
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
  
  vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = 
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  renderWindowInteractor->SetInteractorStyle( style );
    
  //////////////// setup slider ////////////////
  vtkSmartPointer<vtkSliderRepresentation2D> sliderRep = 
      vtkSmartPointer<vtkSliderRepresentation2D>::New();
  sliderRep->SetMinimumValue(0.01);
  sliderRep->SetMaximumValue(40.0);
  
  sliderRep->SetValue(camera->GetDistanceFromCamera());
  sliderRep->SetTitleText("Distance from Camera");
  sliderRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  sliderRep->GetPoint1Coordinate()->SetValue(.15 ,.15);
  sliderRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  sliderRep->GetPoint2Coordinate()->SetValue(.35, .15);

  vtkSmartPointer<vtkSliderWidget> sliderWidget = 
      vtkSmartPointer<vtkSliderWidget>::New();
  sliderWidget->SetInteractor(renderWindowInteractor);
  sliderWidget->SetRepresentation(sliderRep);
  sliderWidget->SetAnimationModeToAnimate();
  sliderWidget->EnabledOn();
  
  vtkSmartPointer<ImageCameraSliderCallback> callback = 
      vtkSmartPointer<ImageCameraSliderCallback>::New();
  callback->SetImageCamera(camera);
  sliderWidget->AddObserver(vtkCommand::EndInteractionEvent,callback);
  
  renderWindow->Render();
  renderWindowInteractor->Start();
  
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