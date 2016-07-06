#ifndef LogModel_H
#define LogModel_H

#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QTableView>
#include <QTextDocument>
#include <QRectF>


#include "logrow.h"


class LogModel : public QAbstractTableModel {
    Q_OBJECT

public:
    LogModel(QObject* parent);
    int columnCount(const QModelIndex &parent) const{Q_UNUSED(parent); return 4;}
    int rowCount(const QModelIndex &parent) const{Q_UNUSED(parent); return logRows.size();}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addRow(LogRow* row);
    LogRow* getRow(int index){ return logRows.at(index);}
    void clear();

private:
    QList<LogRow*> logRows;
};


//Delegate
class PlainStyleDelegate: public QItemDelegate{

public:
    PlainStyleDelegate(QTableView* table) : QItemDelegate(){
        this->table = table;
    }

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        int top = table->rowAt(0);
        int bottom = table->rowAt(table->height() - table->rowHeight(0));
        bottom = bottom == -1 ? table->model()->rowCount() : bottom;

        int row = index.row();

        if(row >= top && row <= bottom){
            QTextDocument doc;
            doc.setHtml("<html<font color=white><b>%1</b></font></html>");

            QRectF rc(option.rect);
            doc.drawContents(painter, rc);

        }
    }

private:
    QTableView* table;
};

#endif // LogModel_H
