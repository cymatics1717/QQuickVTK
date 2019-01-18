#ifndef CELLPICKINTERACTORSTYLE_HPP
#define CELLPICKINTERACTORSTYLE_HPP

#include "qvtkfborenderer.hpp"

#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkCellPicker.h>

class cellPickInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static cellPickInteractorStyle* New();
    vtkTypeMacro(cellPickInteractorStyle, vtkInteractorStyleTrackballCamera)

    virtual void OnLeftButtonDown() override;

    void setParent(QVTKFBORenderer *parent);
protected:
    cellPickInteractorStyle();
    virtual ~cellPickInteractorStyle() override;
private:

    vtkSmartPointer<vtkCellPicker>  picker;

    QVTKFBORenderer *parent;
};

#endif // CELLPICKINTERACTORSTYLE_HPP
