#include "ace_defaultcustomizer.h"
#include <QObject>
#include <QAction>
#include <QMenu>
#include "../core/eabstractdatafactory.h"
#include "../core/eabstractanalyticfactory.h"



void Ace::DefaultCustomizer::setupDataMenu(QMenu &menu, QObject *parent, const std::function<void(int)>& callback)
{
    EAbstractDataFactory& factory {EAbstractDataFactory::instance()};
    for (quint16 i = 0; i < factory.size() ;++i)
    {
       QAction* action {new QAction(factory.name(i),parent)};
       QObject::connect(action,&QAction::triggered,[callback,i]{ callback(i); });
       menu.addAction(action);
    }
}



void Ace::DefaultCustomizer::setupAnalyticMenu(QMenu &menu, QObject *parent, const std::function<void(int)>& callback)
{
    EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
    for (quint16 i = 0; i < factory.size() ;++i)
    {
       QAction* action {new QAction(factory.name(i),parent)};
       QObject::connect(action,&QAction::triggered,[callback,i]{ callback(i); });
       menu.addAction(action);
    }
}



QString Ace::DefaultCustomizer::aboutTitle()
{
    return QStringLiteral("About ACE");
}



QString Ace::DefaultCustomizer::aboutRichText()
{
    return QStringLiteral("<p>ACE is a generic framework for creating GPU-accelerated applications.</p>");
}



QPixmap Ace::DefaultCustomizer::splashImage()
{
    return QPixmap();
}



QIcon Ace::DefaultCustomizer::icon()
{
    return QIcon();
}
