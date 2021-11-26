#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QBoxLayout)
QT_FORWARD_DECLARE_CLASS(QGridLayout)
QT_FORWARD_DECLARE_CLASS(TitleBar)
QT_FORWARD_DECLARE_CLASS(MyDialog)
QT_FORWARD_DECLARE_CLASS(queryHelper)

class loginWidget : public QWidget
{
    Q_OBJECT

public:
    loginWidget(QWidget *parent = nullptr);
    auto getMessageSender() { return message_sender; }
    auto getQueryHelper() { return _query_helper; }
    ~loginWidget() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    enum
    {
        $MaxLength = 32
    };

    TitleBar *_window_hint;

    QLabel
        *_title,
        *_uname,
        *_pwd;

    QLineEdit
        *_uname_edit,
        *_pwd_edit;

    QAbstractButton
        *_sign_up,
        *_forgot_pwd,
        *_login;

    QBoxLayout
        *_main_layout;

    QGridLayout *_grid;

    MyDialog *message_sender;

    queryHelper *_query_helper;

    void clearLineEdit();

private slots:
    void onSignUpClicked();
    void onLogInClicked();
};
#endif // LOGINWIDGET_H
