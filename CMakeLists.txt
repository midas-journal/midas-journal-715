cmake_minimum_required(VERSION 2.6)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

PROJECT(Cameras)
ENABLE_TESTING()

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(Qt4)

########## Interactive (Qt) Examples
IF(Qt4_FOUND)
  INCLUDE(${QT_USE_FILE})
    
  ###### interactive rays (general) ###########
  QT4_WRAP_UI(UISrcsGeneral vtkImageCameraExample_GeneralInteractiveRays.ui)
  QT4_WRAP_CPP(MOCSrcsGeneral vtkImageCameraExample_GeneralInteractiveRaysQt.h)
  
  include_directories(${include_directories} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
  
  ADD_EXECUTABLE(vtkImageCameraExample_GeneralInteractiveRays ../vtkImageCameraExample_GeneralInteractiveRays.cxx vtkImageCameraExample_GeneralInteractiveRaysQt.h vtkImageCameraExample_GeneralInteractiveRaysQt.cxx vtkImageCamera.cxx vtkPhysicalCamera.cxx ${MOCSrcsGeneral} ${UISrcsGeneral})
  TARGET_LINK_LIBRARIES(vtkImageCameraExample_GeneralInteractiveRays ${QT_LIBRARIES} QVTK)

ENDIF(Qt4_FOUND)

########## Examples 
## vtkPhysicalCamera ##

#forward projection
ADD_EXECUTABLE(vtkPhysicalCameraExample_GeneralCameraForwardProjection vtkPhysicalCameraExample_GeneralCameraForwardProjection.cxx vtkPhysicalCamera.cxx)
TARGET_LINK_LIBRARIES(vtkPhysicalCameraExample_GeneralCameraForwardProjection vtkHybrid)

# back projection
ADD_EXECUTABLE(vtkPhysicalCameraExample_GeneralCameraBackwardProjection vtkPhysicalCameraExample_GeneralCameraBackwardProjection.cxx
vtkPhysicalCamera.cxx)
TARGET_LINK_LIBRARIES(vtkPhysicalCameraExample_GeneralCameraBackwardProjection vtkHybrid)

#visualize
ADD_EXECUTABLE(vtkPhysicalCameraExample_VisualizeGeneral vtkPhysicalCameraExample_VisualizeGeneral.cxx vtkPhysicalCamera.cxx)
TARGET_LINK_LIBRARIES(vtkPhysicalCameraExample_VisualizeGeneral vtkHybrid vtkWidgets)

## vtkImageCamera ##

ADD_EXECUTABLE(vtkImageCameraExample_GeneralCamera vtkImageCameraExample_GeneralCamera.cxx
vtkImageCamera.cxx
vtkPhysicalCamera.cxx)
TARGET_LINK_LIBRARIES(vtkImageCameraExample_GeneralCamera vtkHybrid vtkWidgets)

#########
# Tests #
#########

## vtkPhysicalCamera ##
ADD_EXECUTABLE(vtkPhysicalCameraTest vtkPhysicalCameraTest.cxx vtkPhysicalCamera.cxx)
TARGET_LINK_LIBRARIES(vtkPhysicalCameraTest vtkHybrid )
ADD_TEST(vtkPhysicalCameraTest vtkPhysicalCameraTest)

## vtkImageCamera ##
ADD_EXECUTABLE(vtkImageCameraTest vtkImageCameraTest.cxx 
vtkImageCamera.cxx
vtkPhysicalCamera.cxx)
TARGET_LINK_LIBRARIES(vtkImageCameraTest vtkHybrid )
ADD_TEST(vtkImageCameraTest vtkImageCameraTest R.jpg)
