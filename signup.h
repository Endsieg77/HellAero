#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(TitleBar)
QT_FORWARD_DECLARE_CLASS(loginWidget)
QT_FORWARD_DECLARE_CLASS(queryHelper)
QT_FORWARD_DECLARE_CLASS(QDialog)

class signUp : public QWidget
{
    Q_OBJECT
public:
    explicit signUp(queryHelper *_Qh, loginWidget *caller, QWidget *parent = nullptr);
    ~signUp() override;

private:
    enum
    {
        $MaxLength = 32
    };

    queryHelper *_query_helper;
    loginWidget *_caller;
    TitleBar *_window_hint;

    QLabel
        *_uname,
        *_Id,
        *_pwd,
        *_confirm_pwd;

    QLineEdit
        *_uname_edit,
        *_Id_edit,
        *_pwd_edit,
        *_confirm_pwd_edit;

    QAbstractButton
        *_confirmed,
        *_zuruck;

    QBoxLayout *_main_layout;
    QGridLayout *_grid;

signals:

public slots:
    void onConfirmedClicked();
    void onZuruckClicked();
};

#endif // SIGNUP_H
