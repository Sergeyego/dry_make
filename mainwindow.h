#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "modeleldim.h"
#include "dbtablemodel.h"
#include "modelcont.h"
#include "modeloven.h"
#include "modelprogovn.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ModelElDim *modelElDim;
    DbTableModel *modelPrg;
    ModelCont *modelCont;
    DbRelation *relRcp;
    int copyId;
    int copyGrp;
    int getCurrentId();
    ModelOven *modelOven;
    ModelProgOvn *modelProgOvn;
    DbRelationalModel *modelGrp;

private slots:
    void setCurrentPrg(QModelIndex index);
    void setCurrentCont(QModelIndex index);
    void copyCont();
    void pasteCont();
    void updPrgOvn();
    void chGrp();
};

#endif // MAINWINDOW_H
