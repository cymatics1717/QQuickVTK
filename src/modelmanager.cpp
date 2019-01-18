#include "modelmanager.hpp"
#include <QThread>
#include <QDebug>
#include <QFileInfo>
#include <QUrl>

modelManager::modelManager(QObject *parent) : QObject(parent)
{

}

modelManager::~modelManager()
{
    qDeleteAll(models);
}

void modelManager::setRenderer(vtkSmartPointer<vtkRenderer> renderer)
{
    m_renderer = renderer;
}

vtkSmartPointer<vtkRenderer> modelManager::getRenderer() const
{
    return m_renderer;
}

QString modelManager::getModel(vtkProp3D *prop) const
{
    Model *m = pool.value(prop,nullptr);
    if(m){
        return m->source();
    } else {
        return "";
    }
}

void modelManager::insertModel(vtkProp3D *prop, Model *m)
{
    pool.insert(prop,m);
}

void modelManager::addModel(QString filename)
{
    QString source = QUrl(filename).path();
    if(source.contains(':')){
        source.remove(0,1);
    }
    Model *m = new Model(source,this);
    connect(m, &Model::done, this, &modelManager::done);

    qDebug() << m;

    if(QFileInfo(source).isDir()){
        QThread *job= new QThread;
        m->moveToThread(job);
        connect(job, &QThread::started, m, &Model::load);
        connect(job, &QThread::finished, job, &QObject::deleteLater);
        job->start();
    } else {
        m->load();
    }
}

void modelManager::done(QString file, int result)
{
    qDebug() << sender()<< file <<result;
    Model *model = static_cast<Model*>(sender());
    if(model){
        if(result ==0){
            models.append(model);
        } else {
            model->deleteLater();
        }
    }
}
