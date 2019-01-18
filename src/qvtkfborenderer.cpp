#include "qvtkfborenderer.hpp"
#include "qvtkfboitem.hpp"
#include "cellpickinteractorstyle.hpp"
#include "proppickinteractorstyle.hpp"

#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <memory>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <QMetaObject>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPolyLine.h>
#include <vtkVersion.h>
#include <vtkPropPicker.h>
#include <vtkRendererCollection.h>
#include <vtkPointPicker.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkProp3DCollection.h>


QVTKFBORenderer::QVTKFBORenderer(QObject *parent) : QObject(parent),manager(new modelManager(this))
  ,m_vtkFboItem(nullptr), m_dapter(new QVTKInteractorAdapter(this))
  ,m_renderer(vtkSmartPointer<vtkRenderer>::New()),selectedMouse(nullptr)
{
    qDebug() << vtkVersion::GetVTKSourceVersion();
    // Renderer
    vtkSmartPointer<QVTKInteractor> mInteractor = vtkSmartPointer<QVTKInteractor>::New();

    mInteractor->EnableRenderOff();
    renderWindow = vtkSmartPointer<vtkExternalOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(m_renderer);
    renderWindow->OpenGLInitContext();
    renderWindow->SetOffScreenRendering(true);

//    renderWindow->SetReadyForRendering(false);
//    renderWindow->SetForceMaximumHardwareLineWidth(1);
    renderWindow->SetInteractor(mInteractor);


//        vtkSmartPointer<cellPickInteractorStyle> style = vtkSmartPointer<cellPickInteractorStyle>::New();
    vtkSmartPointer<propPickInteractorStyle> style = vtkSmartPointer<propPickInteractorStyle>::New();
//    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    style->SetDefaultRenderer(m_renderer);
//    style->SetMotionFactor(10.0);
    style->setParent(this);
    mInteractor->SetInteractorStyle(style);

    mInteractor->Initialize();
    // Initial camera position
    resetCamera();

    manager->setRenderer(m_renderer);

}

QVTKFBORenderer::~QVTKFBORenderer()
{

}

QOpenGLFramebufferObject *QVTKFBORenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);

    std::unique_ptr<QOpenGLFramebufferObject>framebufferObject(new QOpenGLFramebufferObject(size, format));

//    renderWindow->SetBackLeftBuffer(GL_COLOR_ATTACHMENT0);
//    renderWindow->SetFrontLeftBuffer(GL_COLOR_ATTACHMENT0);
//    renderWindow->SetBackBuffer(GL_COLOR_ATTACHMENT0);
//    renderWindow->SetFrontBuffer(GL_COLOR_ATTACHMENT0);
//    renderWindow->SetSize(framebufferObject->size().width(), framebufferObject->size().height());
//    renderWindow->SetOffScreenRendering(true);
//    renderWindow->Modified();

    return framebufferObject.release();
}

void QVTKFBORenderer::resetCamera()
{
    renderWindow->OpenGLInitState();
    renderWindow->MakeCurrent();
    QOpenGLFunctions::initializeOpenGLFunctions();
    QOpenGLFunctions::glUseProgram(0);
    qDebug()<<"OpenGL:"<< reinterpret_cast<const char*>(glGetString(GL_VERSION));

    m_renderer->ResetCameraClippingRange();
    m_renderer->GetActiveCamera()->SetPosition(0, 0, 1000);
    m_renderer->GetActiveCamera()->SetFocalPoint(0.0, 0.0, 0.0);
    m_renderer->GetActiveCamera()->SetViewUp(0.0, 1, 0.0);
}

void QVTKFBORenderer::initScene()
{
    m_renderer->SetBackground(.8,.8,.8);
    m_renderer->SetBackground2(.4, .4,.4);
    m_renderer->GradientBackgroundOn();
    connect(m_vtkFboItem,&QVTKFBOItem::openFile,manager,&modelManager::addModel);

    manager->addModel("axes");
}

void QVTKFBORenderer::selectedModel(QString modelFileName)
{
    m_vtkFboItem->selectedModel(modelFileName);
}

void QVTKFBORenderer::synchronize(QQuickFramebufferObject *item)
{
//    static int cnt=0;
//    qDebug() <<++cnt<<QString(60,'-')<< __LINE__ << __FUNCTION__;
    if(!m_vtkFboItem){
        m_vtkFboItem = static_cast<QVTKFBOItem*>(item);
        initScene();
    }

    if (renderWindow && renderWindow->GetReadyForRendering())
    {
        while (!m_vtkFboItem->events.empty()) {
            auto e = m_vtkFboItem->events.takeFirst();
            e->accept();
            m_dapter->ProcessEvent(e.get(), renderWindow->GetInteractor());
            if(e->type() ==QEvent::MouseButtonPress){
                selectedMouse = e;
            }
        }
    }

    int *rendererSize = renderWindow->GetSize();
    if (int(m_vtkFboItem->width()) != rendererSize[0] || int(m_vtkFboItem->height()) != rendererSize[1])
    {
        renderWindow->SetSize(int(m_vtkFboItem->width()), int(m_vtkFboItem->height()));
    }
}

void QVTKFBORenderer::render()
{
//    static int cnt = 0;
//    cnt++;
//    qDebug() <<cnt<<QString(60,'-')<< __LINE__ << __FUNCTION__;
    if(selectedMouse){
        QMouseEvent *mouse = static_cast<QMouseEvent*>(selectedMouse.get());
//        qDebug()<<cnt <<QString(30,selectedMouse->isAccepted()?'1':'0') <<selectedMouse.get();
        if(Qt::LeftButton & mouse->buttons()) {
        } else if(Qt::RightButton & mouse->buttons()){
            auto menuevent = QSharedPointer<QContextMenuEvent>::create(
                        QContextMenuEvent::Mouse, mouse->pos(),mouse->globalPos());
            m_dapter->ProcessEvent(menuevent.get(), renderWindow->GetInteractor());
        }
        selectedMouse.reset();
    } else {
//        qDebug()<<cnt <<QString(30,'-');
    }

    renderWindow->PushState();
//    renderWindow->Start();
//    m_renderer->GetActiveCamera()->SetPosition(-1000, 0, 1000);
//    m_renderer->GetActiveCamera()->SetViewUp(0.0, 1, 0.0);

    // Render
    renderWindow->Render();
    renderWindow->PopState();

    m_vtkFboItem->window()->resetOpenGLState();

}

