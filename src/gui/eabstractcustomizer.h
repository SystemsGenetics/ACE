#ifndef EABSTRACTCUSTOMIZER
#define EABSTRACTCUSTOMIZER
#include <functional>
class QObject;
class QMenu;
class QString;
class QPixmap;
class QIcon;



/*
 * WARNING: An Abstract Customizer must be set BEFORE the EApplication class is created!
 */
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
    virtual QPixmap splashImage() = 0;
    virtual QIcon icon() = 0;
private:
    static EAbstractCustomizer* _instance;
};



#endif
