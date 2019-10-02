#include "modeleldim.h"

ModelElDim::ModelElDim(QObject *parent) : QSqlQueryModel(parent)
{
    this->refresh();
}

void ModelElDim::refresh()
{
    this->setQuery("select ide, fnam from dry_els order by fnam");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Марка, диаметр"));
    }
}

void ModelElDim::updateList()
{
    QSqlQuery query;
    query.prepare("select * from rx_els()");
    if (query.exec()){
        this->refresh();
        qDebug()<<"update eldim ok";
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}
