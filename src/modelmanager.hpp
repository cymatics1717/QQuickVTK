#ifndef MODELMANAGER_HPP
#define MODELMANAGER_HPP

#include <QMap>
#include <QObject>
#include <vtkRenderer.h>
#include "model.hpp"

class modelManager : public QObject
{
    Q_OBJECT
public:
    explicit modelManager(QObject *parent = nullptr);
    ~modelManager();
    void setRenderer(vtkSmartPointer<vtkRenderer> renderer);
    vtkSmartPointer<vtkRenderer> getRenderer() const;

    QString getModel(vtkProp3D *prop) const;
    void insertModel(vtkProp3D *prop, Model* m);
signals:

public slots:
    void addModel(QString filename/*,bool async = false*/);
    void done(QString file, int result);

private:
    QMap<vtkProp3D*,Model*> pool;
    vtkSmartPointer<vtkRenderer> m_renderer;

    QList<Model*> models;
};

#endif // MODELMANAGER_HPP
