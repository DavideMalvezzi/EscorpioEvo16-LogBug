#ifndef LOGFILTERMODEL_H
#define LOGFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "logmodel.h"
#include "logrow.h"

//Filter model
class LogFilterModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    LogFilterModel(QObject* parent);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    void updateFilter(bool showInfo, bool showWarn, bool showErr, QString tag);

private:
    bool showInfo, showWarn, showErr;
    QString tag;

};

#endif // LOGFILTERMODEL_H
