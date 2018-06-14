#ifndef ACE_SETUPANALYTICDIALOG_H
#define ACE_SETUPANALYTICDIALOG_H
#include <QDialog>
#include <../core/ace_analytic.h>



class QFormLayout;
//



namespace Ace
{
   /*!
    * This is the dialog that presents all input for an analytic using GUI form 
    * elements and sets them to the analytic manager once the user signals execution. 
    * Because this only sets up the input for the analytic manager it does not take 
    * ownership of it. If the user wishes to execute the analytic this dialog will 
    * return success else it will return failure when executed in modal. 
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
       * Pointer to the analytic manager this dialog uses for constructing its input 
       * argument form and setting those arguments if execute is clicked. 
       */
      Analytic::AbstractManager* _manager;
      /*!
       * List of edit widgets for all analytic manager arguments this dialog represents. 
       * The indexes of all edit widgets in this list must correspond with their analytic 
       * manager argument indexes. 
       */
      QList<QWidget*> _edits;
   };
}



#endif
