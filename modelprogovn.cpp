#include "modelprogovn.h"

ModelProgOvn::ModelProgOvn(QObject *parent) : QSqlQueryModel(parent)
{

}

QVariant ModelProgOvn::data(const QModelIndex &item, int role) const
{
    if (role==Qt::TextAlignmentRole)
        return int(Qt::AlignRight | Qt::AlignVCenter);
    if (role==Qt::DisplayRole && item.column()>0){
        double val=QSqlQueryModel::data(item,role).toDouble();
        return val? QLocale().toString(val,'f',1) : QString();
    }
    return QSqlQueryModel::data(item,role);
}

void ModelProgOvn::refresh(int id_prog, double t, double w, int id_oven)
{
    setQuery("select step, tim, tra from calc_proga_new( "+QString::number(id_prog)+", "+QString::number(w)+", "+
             QString::number(t)+", "+QString::number(id_oven)+" ) order by step");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Ok);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Шаг"));
        setHeaderData(1, Qt::Horizontal,tr("Время, мин."));
        setHeaderData(2, Qt::Horizontal,tr("Темпер., град."));
    }
}
