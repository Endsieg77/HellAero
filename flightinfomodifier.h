#ifndef FLIGHTINFOMODIFIER_H
#define FLIGHTINFOMODIFIER_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(adminWidget)
QT_FORWARD_DECLARE_CLASS(queryHelper)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(TitleBar)

class flightInfoModifier : public QWidget
{
    Q_OBJECT
public:
    explicit flightInfoModifier(queryHelper *_Qh, adminWidget *caller, QWidget *parent = nullptr);
    ~flightInfoModifier() override;

private:
    queryHelper *_query_helper;
    adminWidget *_caller;
    TitleBar *_window_hint;

    QLabel
        *_ind,
        *_abfahrt,
        *_von,
        *_bis,
        *_duration,
        *_capacity;

    QLineEdit
        *_ind_edit,
        *_abfahrt_edit,
        *_von_edit,
        *_bis_edit,
        *_duration_edit,
        *_capacity_edit;

    QAbstractButton
        *_commit_change,
        *_zuruck;

    QBoxLayout *_main_layout;
    QGridLayout *_grid;

private slots:
    void onCommitChangeClicked();
    void onZuruckClicked();
};

#endif // FLIGHTINFOMODIFIER_H
