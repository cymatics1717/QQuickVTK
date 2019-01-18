#ifndef QVTKFBORENDERER_HPP
#define QVTKFBORENDERER_HPP

#include <QObject>
#include <QOpenGLFunctions>
#include <QQuickFramebufferObject>
#include <vtkCellPicker.h>
#include <vtkCubeSource.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <QVTKInteractorAdapter.h>
#include <QVTKInteractor.h>
#include <vtkEventQtSlotConnect.h>
#include <memory>
#include <vtkPropPicker.h>
#include "modelmanager.hpp"

class QVTKFBOItem;

class QVTKFBORenderer : public QObject, public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit QVTKFBORenderer(QObject *parent = nullptr);
    ~QVTKFBORenderer() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void render() override;
    void synchronize(QQuickFramebufferObject *item) override;

    void resetCamera();

    modelManager *manager;

signals:

public slots:
    void initScene();
    void selectedModel(QString modelFileName);

private:
    QList<std::shared_ptr<QEvent>> events;
    QVTKFBOItem *m_vtkFboItem;

    vtkSmartPointer<vtkExternalOpenGLRenderWindow> renderWindow;
    QVTKInteractorAdapter* m_dapter;
    vtkSmartPointer<vtkRenderer> m_renderer;

    std::shared_ptr<QEvent> selectedMouse;

};

#endif // QVTKFBORENDERER_HPP
