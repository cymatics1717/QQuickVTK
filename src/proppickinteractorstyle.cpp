#include "proppickinteractorstyle.hpp"
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkPickingManager.h>
#include <vtkRenderer.h>
#include <QDebug>
#include <vtkCellPicker.h>

vtkStandardNewMacro(propPickInteractorStyle);

propPickInteractorStyle::propPickInteractorStyle()
{
    LastPickedActor = nullptr;
    LastPickedProperty = vtkProperty::New();
//    cell_picker = vtkSmartPointer<vtkCellPicker>::New();
//    cell_picker->SetTolerance(0.0);

    prop_picker = vtkSmartPointer<vtkPropPicker>::New();
}

propPickInteractorStyle::~propPickInteractorStyle()
{
    LastPickedProperty->Delete();
}

void propPickInteractorStyle::OnLeftButtonDown()
{
    int* clickPos = GetInteractor()->GetEventPosition();

    vtkRenderer *renderer = GetDefaultRenderer();
    vtkSmartPointer<vtkPropPicker>  picker = vtkSmartPointer<vtkPropPicker>::New();
    picker->Pick(clickPos[0], renderer->GetSize()[1]+clickPos[1],0, renderer);

//    qDebug() <<picker->GetProp3D() << picker->GetActor()/* << filename*/;
    // If we picked something before, reset its property
    if (LastPickedActor)
    {
        LastPickedActor->GetProperty()->DeepCopy(LastPickedProperty);
    }

    LastPickedActor = picker->GetActor();
    if (LastPickedActor)
    {
        QString filename = parent->manager->getModel(picker->GetProp3D());
        qDebug() << picker->GetProp3D()->GetClassName()<<picker->GetProp3D()  << filename;
        // Save the property of the picked actor so that we can
        // restore it next time
        LastPickedProperty->DeepCopy(LastPickedActor->GetProperty());
        // Highlight the picked actor by changing its properties
        LastPickedActor->GetProperty()->SetColor(0., 0.5, 1);
        LastPickedActor->GetProperty()->SetDiffuse(1.0);
        LastPickedActor->GetProperty()->SetSpecular(0.0);

        emit parent->selectedModel(filename);
    } else if(picker->GetProp3D()){
        QString filename = parent->manager->getModel(picker->GetProp3D());
        qDebug() << picker->GetProp3D()->GetClassName()<<picker->GetProp3D()  << filename;
        emit parent->selectedModel(filename);
    }

    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void propPickInteractorStyle::OnRightButtonDown()
{
    int* clickPos = GetInteractor()->GetEventPosition();

    vtkRenderer *renderer = GetDefaultRenderer();
    vtkSmartPointer<vtkPropPicker>  picker = vtkSmartPointer<vtkPropPicker>::New();
    picker->Pick(clickPos[0], renderer->GetSize()[1]+clickPos[1],0, renderer);


    // If we picked something before, reset its property
    if (LastPickedActor)
    {
        LastPickedActor->GetProperty()->DeepCopy(LastPickedProperty);
    }
    LastPickedActor = picker->GetActor();
    if (LastPickedActor)
    {
//        QString filename = parent->manager->getModelName(picker->GetProp3D(),"none");
        qDebug() << picker->GetProp3D()->GetClassName()<<picker->GetProp3D()  /*<< filename*/;
        // Save the property of the picked actor so that we can
        // restore it next time
        LastPickedProperty->DeepCopy(LastPickedActor->GetProperty());
        // Highlight the picked actor by changing its properties
        LastPickedActor->GetProperty()->SetColor(0., 0.5, .8);
        LastPickedActor->GetProperty()->SetDiffuse(1.0);
        LastPickedActor->GetProperty()->SetSpecular(0.0);

//        emit parent->selectedModel(filename);
    }

    vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}

void propPickInteractorStyle::setParent(QVTKFBORenderer *parent)
{
    this->parent = parent;
}

