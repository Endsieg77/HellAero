#ifndef PASSENGERWIDGET_H
#define PASSENGERWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(queryHelper)
QT_FORWARD_DECLARE_CLASS(QScrollArea)

class passengerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit passengerWidget(queryHelper *_Qh, QWidget *caller, QWidget *parent = nullptr);

private:
    queryHelper *_query_helper;
    QWidget *_caller;
    QScrollArea *_scrollable;

};

#endif // PASSENGERWIDGET_H
