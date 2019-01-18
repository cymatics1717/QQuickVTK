#ifndef QVTKFBOITEM_HPP
#define QVTKFBOITEM_HPP

#include <QQuickFramebufferObject>
#include <memory>

class QVTKFBORenderer;
class QVTKFBOItem : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    QVTKFBOItem();
    QQuickFramebufferObject::Renderer *createRenderer() const override;

    QList<std::shared_ptr<QEvent>> events;

public slots:

signals:
    void openFile(QString filename);
    void selectedModel(QString modelFileName);

protected:
    bool eventFilter(QObject *object, QEvent *evt) override;

public slots:
    void handleEvent(std::shared_ptr<QEvent> event);

private:
    QVTKFBORenderer *renderer;
};

#endif // QVTKFBOITEM_HPP
