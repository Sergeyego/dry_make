#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    copyId=-1;
    copyGrp=-1;

    modelOven = new ModelOven(this);
    ui->comboBoxOvn->setModel(modelOven);
    ui->comboBoxOvn->setModelColumn(1);

    modelGrp = new DbRelationalModel("select id, nam from owens_groups order by id",this);
    ui->comboBoxGrp->setModel(modelGrp);
    ui->comboBoxGrp->setModelColumn(1);

    modelProgOvn = new ModelProgOvn(this);
    ui->tableViewOvn->setModel(modelProgOvn);
    ui->tableViewOvn->verticalHeader()->setDefaultSectionSize(ui->tableViewOvn->verticalHeader()->fontMetrics().height()*1.5);

    modelElDim = new ModelElDim(this);
    ui->tableViewElDim->setModel(modelElDim);
    ui->tableViewElDim->verticalHeader()->setDefaultSectionSize(ui->tableViewElDim->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewElDim->setColumnHidden(0,true);
    ui->tableViewElDim->setColumnWidth(1,200);

    relRcp = new DbRelation(new DbRelationalModel("select id, nam, 1 as ord from rcp_nam where lev=1 and id<>0 "
                                                  "union "
                                                  "select 0 as id, "+tr("'Рецептура по умолчанию'")+" as nam, 0 as ord "
                                                  "order by ord, nam",this),0,1,this);
    modelPrg = new DbTableModel("dry_reg",this);
    modelPrg->addColumn("id",tr("id"),true,true,TYPE_INT);
    modelPrg->addColumn("ide",tr("ide"),false,false,TYPE_INT);
    modelPrg->addColumn("nam",tr("Название"),false,false,TYPE_STRING);
    modelPrg->addColumn("id_rcp",tr("Рецептура"),false,false,TYPE_STRING,NULL,relRcp);
    modelPrg->setSort("dry_reg.nam");

    ui->tableViewPrg->setModel(modelPrg);
    ui->tableViewPrg->setColumnHidden(0,true);
    ui->tableViewPrg->setColumnHidden(1,true);
    ui->tableViewPrg->setColumnWidth(2,200);
    ui->tableViewPrg->setColumnWidth(3,220);

    modelCont = new ModelCont(this);
    ui->tableViewCont->setModel(modelCont);
    ui->tableViewCont->setColumnHidden(0,true);
    ui->tableViewCont->setColumnHidden(1,true);
    ui->tableViewCont->setColumnWidth(2,60);
    ui->tableViewCont->setColumnWidth(3,120);
    ui->tableViewCont->setColumnWidth(4,120);

    connect(ui->tableViewElDim->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(setCurrentPrg(QModelIndex)));
    connect(ui->tableViewPrg->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(setCurrentCont(QModelIndex)));
    connect(ui->actionRcp,SIGNAL(triggered(bool)),relRcp->model(),SLOT(refresh()));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionElDim,SIGNAL(triggered(bool)),modelElDim,SLOT(updateList()));
    connect(ui->actionGrp,SIGNAL(triggered(bool)),modelGrp,SLOT(refresh()));
    connect(ui->pushButtonCopy,SIGNAL(clicked(bool)),this,SLOT(copyCont()));
    connect(ui->pushButtonPaste,SIGNAL(clicked(bool)),this,SLOT(pasteCont()));
    connect(ui->actionOvn,SIGNAL(triggered(bool)),modelOven,SLOT(refresh()));
    connect(ui->comboBoxOvn,SIGNAL(currentIndexChanged(int)),this,SLOT(updPrgOvn()));
    connect(ui->lineEditT,SIGNAL(textChanged(QString)),this,SLOT(updPrgOvn()));
    connect(ui->lineEditW,SIGNAL(textChanged(QString)),this,SLOT(updPrgOvn()));
    connect(modelCont,SIGNAL(sigUpd()),this,SLOT(updPrgOvn()));
    connect(modelCont,SIGNAL(sigRefresh()),this,SLOT(updPrgOvn()));
    connect(ui->comboBoxGrp,SIGNAL(currentIndexChanged(int)),this,SLOT(chGrp()));

    if (ui->tableViewElDim->model()->rowCount())
        ui->tableViewElDim->selectRow(0);

    ui->tableViewOvn->setColumnWidth(0,60);
    ui->tableViewOvn->setColumnWidth(1,120);
    ui->tableViewOvn->setColumnWidth(2,120);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getCurrentId()
{
    int id=-1;
    QModelIndex currentIndex=ui->tableViewPrg->currentIndex();
    if (!(currentIndex.row()==modelPrg->rowCount()-1 && modelPrg->isAdd()) && currentIndex.isValid()){
        id=ui->tableViewPrg->model()->data(ui->tableViewPrg->model()->index(currentIndex.row(),0),Qt::EditRole).toInt();
    }
    return id;
}

void MainWindow::setCurrentPrg(QModelIndex index)
{
    QVariant ide= index.isValid() ? ui->tableViewElDim->model()->data(ui->tableViewElDim->model()->index(index.row(),0),Qt::EditRole) : -1;
    modelPrg->setFilter("dry_reg.ide = "+ide.toString());
    modelPrg->setDefaultValue(1,ide);
    modelPrg->select();
    QString nam=ui->tableViewElDim->model()->data(ui->tableViewElDim->model()->index(index.row(),1),Qt::DisplayRole).toString();
    ui->statusBar->showMessage(nam);
    ui->tableViewPrg->selectRow(0);
}

void MainWindow::setCurrentCont(QModelIndex index)
{
    int id=-1;
    if (!(index.row()==modelPrg->rowCount()-1 && modelPrg->isAdd()) && index.isValid()) {
        id=ui->tableViewPrg->model()->data(ui->tableViewPrg->model()->index(index.row(),0),Qt::EditRole).toInt();
        QString nam=ui->tableViewPrg->model()->data(ui->tableViewPrg->model()->index(index.row(),2),Qt::EditRole).toString();
        ui->groupBoxCont->setEnabled(true);
        ui->groupBoxCont->setTitle(nam);
    } else {
        ui->groupBoxCont->setEnabled(false);
        ui->groupBoxCont->setTitle(tr("Программа не создана"));
    }
    int id_grp=ui->comboBoxGrp->model()->data(ui->comboBoxGrp->model()->index(ui->comboBoxGrp->currentIndex(),0),Qt::EditRole).toInt();
    modelCont->refresh(id, id_grp);
}

void MainWindow::copyCont()
{
    copyId=getCurrentId();
    copyGrp=ui->comboBoxGrp->model()->data(ui->comboBoxGrp->model()->index(ui->comboBoxGrp->currentIndex(),0),Qt::EditRole).toInt();
    int r=modelCont->isAdd()? modelCont->rowCount()-1 : modelCont->rowCount();
    ui->pushButtonPaste->setEnabled(copyId!=-1 && r>0);
}

void MainWindow::pasteCont()
{
    int r=modelCont->isAdd()? modelCont->rowCount()-1 : modelCont->rowCount();
    if (r){
        QMessageBox::information(this,tr("Предупреждение"),tr("Сначала удалите все уже существующие шаги!"),QMessageBox::Ok);
    } else {
        modelCont->copyCnt(copyId,copyGrp);
    }
}

void MainWindow::updPrgOvn()
{
    int id_prg=getCurrentId();
    double t=ui->lineEditT->text().toDouble();
    double w=ui->lineEditW->text().toDouble();
    int id_ovn=ui->comboBoxOvn->model()->data(ui->comboBoxOvn->model()->index(ui->comboBoxOvn->currentIndex(),0),Qt::EditRole).toInt();
    modelProgOvn->refresh(id_prg,t,w,id_ovn);
}

void MainWindow::chGrp()
{
    setCurrentCont(ui->tableViewPrg->currentIndex());
}
