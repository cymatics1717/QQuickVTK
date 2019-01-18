#include "cellpickinteractorstyle.hpp"
#include <vtkProp3DCollection.h>

vtkStandardNewMacro(cellPickInteractorStyle);

void cellPickInteractorStyle::OnLeftButtonDown()
{
    int* pos = GetInteractor()->GetEventPosition();
    vtkRenderer *m_renderer = GetDefaultRenderer();

    picker->Pick(pos[0], m_renderer->GetSize()[1]+pos[1], 0, m_renderer);

    double clickPosition[3];
    picker->GetPickPosition(clickPosition);
    {
        auto collection = picker->GetProp3Ds();
        qDebug() <<m_renderer->GetActors()->GetNumberOfItems()
                <<collection->GetNumberOfItems()
              <<"..."<< clickPosition[0]<< clickPosition[1]<< clickPosition[2];
        if(collection){
            collection->InitTraversal();
            auto item = collection->GetNextProp();
            while(item) {
//                qDebug() << item->GetClassName()<<item << parent->objects.value(item,"none");
                item = collection->GetNextProp();
                auto actor = static_cast<vtkActor*>(item);
                if(actor){
                    actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
                } else {
                    qDebug() << "failed to cast "<<item;
                }

//                if(QString("vtkImageActor")==QString(item->GetClassName())){
//                    auto actor = static_cast<vtkImageActor*>(item);
//                    actor->GetProperty()->SetOpacity(.5);
//                    actor->Modified();
//                } else if(QString("vtkOpenGLActor")== QString(item->GetClassName())){
//                    auto actor = static_cast<vtkActor*>(item);
//                    actor->GetProperty()->SetOpacity(.8);
//                    actor->Modified();
//                }

            }
        }
    }
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

}

void cellPickInteractorStyle::setParent(QVTKFBORenderer *parent)
{
    this->parent = parent;
}

cellPickInteractorStyle::cellPickInteractorStyle():
    picker(vtkSmartPointer<vtkCellPicker>::New())
{

}

cellPickInteractorStyle::~cellPickInteractorStyle()
{

}
