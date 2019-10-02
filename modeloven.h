#ifndef MODELOVEN_H
#define MODELOVEN_H

#include <QObject>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class ModelOven : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelOven(QObject *parent);
public slots:
    void refresh();
};

#endif // MODELOVEN_H
