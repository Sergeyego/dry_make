#include "modeloven.h"

ModelOven::ModelOven(QObject *parent) : QSqlQueryModel(parent)
{
    this->refresh();
}

void ModelOven::refresh()
{
    this->setQuery("select id, num from owens order by num");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Печь"));
    }
}
