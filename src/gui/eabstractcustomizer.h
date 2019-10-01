#ifndef EABSTRACTCUSTOMIZER
#define EABSTRACTCUSTOMIZER
#include <functional>
class QObject;
class QMenu;
class QString;



class EAbstractCustomizer
{
public:
    static EAbstractCustomizer& instance();
    static void setInstance(EAbstractCustomizer* instance);
public:
    virtual ~EAbstractCustomizer() = default;
public:
    virtual void setupDataMenu(QMenu& menu, QObject* parent, const std::function<void(int)>& callback) = 0;
    virtual void setupAnalyticMenu(QMenu& menu, QObject* parent, const std::function<void(int)>& callback) = 0;
    virtual QString aboutTitle() = 0;
    virtual QString aboutRichText() = 0;
private:
    static EAbstractCustomizer* _instance;
};



#endif
