#ifndef TICKETSELLERWIDGET_H
#define TICKETSELLERWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(TitleBar)
QT_FORWARD_DECLARE_CLASS(queryHelper)
QT_FORWARD_DECLARE_CLASS(QScrollArea)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QTableWidget)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)

class ticketSellerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ticketSellerWidget(queryHelper *_Qh, QWidget *caller, QWidget *parent = nullptr);
    ~ticketSellerWidget() override;

private:
    queryHelper *_query_helper;
    QWidget *_caller;
    TitleBar *_window_hint;
    QLabel *_type_index;
    QLineEdit *_index_edit;
    QTableWidget *_table;
    QScrollArea *_scrollable;
    QAbstractButton
        *_sell,
        *_cancel,
        *_zuruck;
    QBoxLayout
        *_ind_edit_layout,
        *_functional_layout,
        *_main_layout;

private slots:
    void onSellClicked();
    void onCancelClicked();
    void onZuruckClicked();
};

#endif // TICKETSELLERWIDGET_H
