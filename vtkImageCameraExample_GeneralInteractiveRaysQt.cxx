#include "ui_vtkImageCameraExample_GeneralInteractiveRays.h"
#include "vtkImageCameraExample_GeneralInteractiveRaysQt.h"

#include <vtkSmartPointer.h>
#include <vtkLineSource.h>
#include <vtkImageActor.h>
#include <vtkProperty2D.h>
#include <vtkDataObjectToTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkCommand.h>
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
#include <vtkInteractorStyleImage.h>
#include <vtkMath.h>
#include <vtkMatrix3x3.h>
#include <vtkSphereWidget.h>
#include <vtkImageChangeInformation.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextActor.h>

#include "vtkImageCamera.h"
#include "vtkPhysicalCamera.h"

void CreateRotationY(double degree, vtkSmartPointer<vtkMatrix3x3> R);

class SphereCallback : public vtkCommand
{
  public:
    static SphereCallback *New()
    {
      return new SphereCallback;
    }
    SphereCallback(){}
    
    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
      
      vtkSphereWidget *sphereWidget = 
          reinterpret_cast<vtkSphereWidget*>(caller);
      
      double center[3];
      sphereWidget->GetCenter(center);
      //cout << "Center: " << center[0] << " " << center[1] << " " << center[2] << endl;
      
      double pix[2];
      pix[0] = -center[0]; //here is the elusive negation
      pix[1] = center[1];
      cout << "pix: " << pix[0] << " " << pix[1] << endl;
      double ray[3];
      this->Camera->GetRay(pix, ray);
      
      double p2[3];
      double cameraCenter[3];
      this->Camera->GetCameraCenter(cameraCenter);
      vtkMath::Add(cameraCenter, ray, p2);
      this->LineSource->SetPoint2(p2);
      this->LineSource->Update();
  
      this->LeftRenderWindow->Render();
    }
    
  void SetLineSource(vtkSmartPointer<vtkLineSource> line) {this->LineSource = line;}
  void SetCamera(vtkSmartPointer<vtkImageCamera> camera) {this->Camera = camera;}
  void SetLeftRenderWindow(vtkSmartPointer<vtkRenderWindow> renderWindow) {this->LeftRenderWindow = renderWindow;}
  
  private:
    vtkSmartPointer<vtkLineSource> LineSource;
    vtkSmartPointer<vtkImageCamera> Camera;
    vtkSmartPointer<vtkRenderer> LeftRenderer;
    vtkSmartPointer<vtkRenderWindow> LeftRenderWindow;
};

// Constructor
MainForm::MainForm(const char* filename)  
{
  this->ImageFilename = filename;
  
  this->ui = new Ui_MainForm;
  this->ui->setupUi(this);
  
  //renderers
  vtkSmartPointer<vtkRenderer> leftRenderer = 
      vtkSmartPointer<vtkRenderer>::New();
  
  vtkSmartPointer<vtkRenderer> rightRenderer = 
      vtkSmartPointer<vtkRenderer>::New();

  // VTK/Qt wedded
  this->ui->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(leftRenderer);
  this->ui->qvtkWidgetRight->GetRenderWindow()->AddRenderer(rightRenderer);
    
  vtkSmartPointer<vtkAxesActor> worldOrigin = 
      vtkSmartPointer<vtkAxesActor>::New();
  worldOrigin->SetXAxisLabelText("Xo");
  worldOrigin->SetYAxisLabelText("Yo");
  worldOrigin->SetZAxisLabelText("Zo");
  int fontSize = 10;
  worldOrigin->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
  worldOrigin->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
  worldOrigin->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
  
  worldOrigin->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(fontSize);
  worldOrigin->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(fontSize);
  worldOrigin->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(fontSize);
  
  worldOrigin->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
  worldOrigin->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
  worldOrigin->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
  
  vtkSmartPointer<vtkJPEGReader> reader = 
      vtkSmartPointer<vtkJPEGReader>::New();
  cout << "Reading " << this->ImageFilename.c_str() << endl;
  reader->SetFileName(this->ImageFilename.c_str());
  reader->Update();
    
  vtkSmartPointer<vtkImageChangeInformation> changeInformation = 
      vtkSmartPointer<vtkImageChangeInformation>::New();
  changeInformation->SetInputConnection(reader->GetOutputPort());
  changeInformation->CenterImageOn();
  changeInformation->Update();
  
   //create an actor
  vtkSmartPointer<vtkImageActor> imageActor = 
      vtkSmartPointer<vtkImageActor>::New();
  imageActor->SetInput ( changeInformation->GetOutput() );
  rightRenderer->AddActor(imageActor);
  
  this->Camera = vtkSmartPointer<vtkImageCamera>::New();
  
  vtkSmartPointer<vtkMatrix3x3> r = 
      vtkSmartPointer<vtkMatrix3x3>::New();
  //CreateRotationY(10, r);
  CreateRotationY(45, r);
  
  //double cameraCenter[3] = {5.0, 0.0, 0.0};
  //double cameraCenter[3] = {0.0, 0.0, 5.0};
  double cameraCenter[3] = {1.0, 2.0, 3.0};
  this->Camera->SetR(r);
  this->Camera->SetCameraCenter(cameraCenter);
  this->Camera->SetFocalLength(200);
  this->Camera->SetTextureImage(reader->GetOutput());
  
  this->Camera->SetRenderer(leftRenderer);
  this->Camera->UpdateCameraActor();
  this->Camera->UpdateImageActor();
  //renderer->AddActor(camera->GetAxesActor());
  
  this->SphereWidget = vtkSmartPointer<vtkSphereWidget>::New();
  this->SphereWidget->SetRadius(10);
  this->SphereWidget->SetInteractor(this->ui->qvtkWidgetRight->GetRenderWindow()->GetInteractor());
  this->SphereWidget->SetRepresentationToSurface();
  
  vtkSmartPointer<SphereCallback> sphereCallback = 
      vtkSmartPointer<SphereCallback>::New();
 
  this->SphereWidget->AddObserver(vtkCommand::InteractionEvent,sphereCallback);
  this->SphereWidget->GetSphereProperty()->SetColor(1.0, 0.0, 0.0);
  this->SphereWidget->GetSelectedSphereProperty()->SetColor(1.0, 0.0, 0.0);
  this->SphereWidget->On();
  
  double pix[2] = {0, 0};
  double ray[3];
  this->Camera->GetRay(pix, ray);
  
  double p2[3];
  vtkMath::Add(cameraCenter, ray, p2);
  this->LineSource = vtkSmartPointer<vtkLineSource>::New();
  this->LineSource->SetPoint1(cameraCenter);
  this->LineSource->SetPoint2(p2);
  this->LineSource->Update();
  
  sphereCallback->SetLineSource(this->LineSource);
  sphereCallback->SetCamera(this->Camera);
  sphereCallback->SetLeftRenderWindow(this->ui->qvtkWidgetLeft->GetRenderWindow());
  
  vtkSmartPointer<vtkPolyDataMapper> lineMapper = 
      vtkSmartPointer<vtkPolyDataMapper>::New();
  lineMapper->SetInputConnection(this->LineSource->GetOutputPort());
  vtkSmartPointer<vtkActor> lineActor = 
      vtkSmartPointer<vtkActor>::New();
  lineActor->SetMapper(lineMapper);
  lineActor->GetProperty()->SetColor(1.0, .2, .5);
  leftRenderer->AddActor(lineActor);
  
  leftRenderer->AddActor(worldOrigin);
  leftRenderer->ResetCamera();
    
  vtkSmartPointer<vtkInteractorStyleImage> imageStyle = 
      vtkSmartPointer<vtkInteractorStyleImage>::New();
  this->ui->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->SetInteractorStyle(imageStyle);
  
};


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