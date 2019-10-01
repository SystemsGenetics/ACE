#ifndef ACE_DEFAULTCUSTOMIZER_H
#define ACE_DEFAULTCUSTOMIZER_H
#include "eabstractcustomizer.h"



namespace Ace
{
    class DefaultCustomizer : public EAbstractCustomizer
    {
        virtual void setupDataMenu(QMenu& menu, QObject* parent, const std::function<void(int)>& callback) override final;
        virtual void setupAnalyticMenu(QMenu& menu, QObject* parent, const std::function<void(int)>& callback) override final;
        virtual QString aboutTitle() override final;
        virtual QString aboutRichText() override final;
    };
}



#endif
