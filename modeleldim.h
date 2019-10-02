#ifndef MODELELDIM_H
#define MODELELDIM_H

#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>


class ModelElDim : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelElDim(QObject *parent=0);
public slots:
    void refresh();
    void updateList();
};

#endif // MODELELDIM_H
