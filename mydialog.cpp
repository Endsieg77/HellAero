#include "mydialog.h"
#include "titlebar.h"
#include <QtWidgets>
#include <QDesktopWidget>
#include <QBoxLayout>

MyDialog::MyDialog(const QString &text, const QString &title, QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
{
    setAttribute(Qt::WA_QuitOnClose,false);
    setFixedSize(QApplication::desktop()->width() * 0.35, QApplication::desktop()->height() * 0.30);

    windowTitle = new TitleBar(title, this);
    textLabel   = new QLabel(this);
    confirm     = new QPushButton("确定", this);
    confirm->setObjectName("login-btn");

    textLabel->setStyleSheet("font-size: 20px; color: rgb(117, 109, 109);");
    textLabel->setText(text);
    textLabel->setFixedHeight(height() * 0.255);
    textLabel->setFixedWidth(width() * 0.97);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);

    confirm->setStyleSheet("border-radius: 1rem;");
    confirm->setFixedHeight(height() * 0.25);
//    confirm->setFixedWidth(width() * 0.97);

    connect(confirm, &QPushButton::clicked, this, &MyDialog::close);

    auto dialogLayout = new QVBoxLayout(this);
    dialogLayout->setMargin(0);
    dialogLayout->setSpacing(20);
    dialogLayout->setObjectName("dialogLayout");
    dialogLayout->addWidget(windowTitle);
    dialogLayout->addWidget(textLabel);
    dialogLayout->addWidget(confirm);
    dialogLayout->setAlignment(Qt::AlignTop);
}

void MyDialog::setWindowTitle(const QString &_title)
{ windowTitle->setText(_title); }

void MyDialog::setText(const QString &_text)
{ textLabel->setText(_text); }
