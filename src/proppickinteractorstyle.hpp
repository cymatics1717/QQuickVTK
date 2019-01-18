#ifndef propPickInteractorStyle_HPP
#define propPickInteractorStyle_HPP

#include "qvtkfborenderer.hpp"

#include <QMap>
#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>

class propPickInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static propPickInteractorStyle* New();
    vtkTypeMacro(propPickInteractorStyle, vtkInteractorStyleTrackballCamera)

    virtual void OnLeftButtonDown() override;
    virtual void OnRightButtonDown() override;

    void setParent(QVTKFBORenderer *parent);
protected:
    propPickInteractorStyle();
    virtual ~propPickInteractorStyle() override;
private:
    vtkActor    *LastPickedActor;
    vtkProperty *LastPickedProperty;
    vtkSmartPointer<vtkPropPicker>  prop_picker;

    QVTKFBORenderer *parent;
};


#endif // propPickInteractorStyle_HPP
