#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(TitleBar)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QScrollArea)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(queryHelper)

class queryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit queryWidget(queryHelper *_Qh, QWidget *caller, bool _editable = false, QWidget *parent = nullptr);
    ~queryWidget() override;

private:
    queryHelper *_query_helper;
    QWidget *_caller;
    TitleBar *_window_hint;
    QScrollArea *_scrollable;
    QTableWidget *_table;
    QLineEdit *_query_edit;

    QBoxLayout
        *_query_line,
        *_zuruck_line,
        *_main_layout;

    QAbstractButton
        *_query,
        *_zuruck;
    bool _editable = false;

private slots:
    void doDeleteItem(int, int);
    void onQueryClicked();
    void onZuruckClicked();
};

#endif // QUERYWIDGET_H
