#ifndef MODELPROGOVN_H
#define MODELPROGOVN_H

#include <QObject>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class ModelProgOvn : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelProgOvn(QObject *parent);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
    void refresh(int id_prog, double t, double w, int id_oven);
};

#endif // MODELPROGOVN_H
