#include "qvtkfboitem.hpp"
#include "qvtkfborenderer.hpp"

QVTKFBOItem::QVTKFBOItem()
{
    startTimer(1000);
    installEventFilter(this);
    setMirrorVertically(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptTouchEvents(true);
//    setAcceptHoverEvents(true);
    setFlags(flags()|QQuickItem::ItemAcceptsDrops
             |QQuickItem::ItemIsFocusScope
             |QQuickItem::ItemClipsChildrenToShape
//             |QQuickItem::ItemAcceptsInputMethod
             |QQuickItem::ItemHasContents);
}

QQuickFramebufferObject::Renderer *QVTKFBOItem::createRenderer() const
{
    return new QVTKFBORenderer();
}

bool QVTKFBOItem::eventFilter(QObject *o, QEvent *e)
{
//    qDebug()<<QString(30,'+') <<"event: "<< o << e;
    const QEvent::Type t = e->type();
    if (t == QEvent::MouseButtonPress||t==QEvent::MouseButtonRelease
            ||t==QEvent::MouseButtonDblClick||t==QEvent::MouseMove) {
        handleEvent(std::make_shared<QMouseEvent>(static_cast<const QMouseEvent&>(*e)));
    } else if(t==QEvent::KeyPress||t==QEvent::KeyRelease||t==QEvent::ShortcutOverride){
        handleEvent(std::make_shared<QKeyEvent>(static_cast<const QKeyEvent&>(*e)));
    } else if(t==QEvent::Wheel){
        handleEvent(std::make_shared<QWheelEvent>(static_cast<const QWheelEvent&>(*e)));
    } else if(t==QEvent::HoverMove||t==QEvent::HoverEnter||t==QEvent::HoverLeave){
        handleEvent(std::make_shared<QHoverEvent>(static_cast<const QHoverEvent&>(*e)));
    } else if(t==QEvent::FocusIn||t==QEvent::FocusOut){
        handleEvent(std::make_shared<QFocusEvent>(static_cast<const QFocusEvent&>(*e)));
    } else if(t==QEvent::Timer){
        handleEvent(std::make_shared<QTimerEvent>(static_cast<const QTimerEvent&>(*e)));
    } else if(t == QEvent::TouchBegin ||t == QEvent::TouchUpdate || t == QEvent::TouchEnd){
        handleEvent(std::make_shared<QTouchEvent>(static_cast<const QTouchEvent&>(*e)));
//    } else if(t == QEvent::ContextMenu){
//        handleEvent(std::make_shared<QContextMenuEvent>(static_cast<const QContextMenuEvent&>(*e)));
    } else {
        if(!static_cast<QInputMethodQueryEvent*>(e)){
            qDebug()<<QString(30,'+') <<"unhandled event: "<< o << e;
    //        handleEvent(std::make_shared<QEvent>(static_cast<const QEvent&>(*e)));
            return QObject::eventFilter(o,e);
        }
    }
    return true;
}

void QVTKFBOItem::handleEvent(std::shared_ptr<QEvent> event)
{
//    qDebug() << event.get();
    event->accept();
    events.append(event);
    update();
}
