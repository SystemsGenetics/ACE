#ifndef ACE_SETUPANALYTICDIALOG_H
#define ACE_SETUPANALYTICDIALOG_H
#include <QDialog>
#include <core/ace_analytic.h>



class QFormLayout;
//



namespace Ace
{
   /*!
    */
   class SetupAnalyticDialog : public QDialog
   {
      Q_OBJECT
   public:
      SetupAnalyticDialog(Analytic::AbstractManager* manager);
   private slots:
      void findFile(int index);
      void findDataObject(int index);
      void executeTriggered();
   private:
      QString processBool(quint16 index);
      QString processInteger(quint16 index);
      QString processDouble(quint16 index);
      QString processString(quint16 index);
      QString processSelection(quint16 index);
      QLayout* createForm();
      QLayout* createButtons();
      QWidget* createBool(quint16 index);
      QWidget* createInteger(quint16 index);
      QWidget* createDouble(quint16 index);
      QWidget* createString(quint16 index);
      QWidget* createSelection(quint16 index);
      QWidget* createFile(quint16 index);
      /*!
       */
      Analytic::AbstractManager* _manager;
      /*!
       */
      QList<QWidget*> _edits;
   };
}



#endif
