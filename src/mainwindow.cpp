#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // defaults
    ui->check2->setChecked(true);
    ui->check3->setChecked(true);
    ui->check4->setChecked(true);
    ui->check5->setChecked(true);
    ui->check6->setChecked(true);
    ui->check7->setChecked(true);
    ui->check8->setChecked(true);
    ui->check9->setChecked(true);

    ui->rbMult->setChecked(true);

    ui->rb_1->setChecked(true);

    // make go button blue
    QPalette pal = ui->pushButtonGo->palette();
    pal.setColor(QPalette::Button, QColor(Qt::blue));
    ui->pushButtonGo->setAutoFillBackground(true);
    ui->pushButtonGo->setPalette(pal);
    ui->pushButtonGo->update();

    // some connects
    // go and test button
    connect(ui->pushButtonGo, SIGNAL(clicked()), this, SLOT(goPressed()));
    connect(ui->pushButtonTest, SIGNAL(clicked()), this, SLOT(testPressed()));

    // return on lineEdit
    connect(ui->lineEditTest, SIGNAL(returnPressed()), this, SLOT(testPressed()));

    ui->lineEditTest->setFocus();
}

QVector<int> MainWindow::getCheckedNumbers()
{
    QVector<int> nbrs{};

    if (ui->check1->isChecked())
        nbrs.push_back(1);

    if (ui->check2->isChecked())
        nbrs.push_back(2);

    if (ui->check3->isChecked())
        nbrs.push_back(3);

    if (ui->check4->isChecked())
        nbrs.push_back(4);

    if (ui->check5->isChecked())
        nbrs.push_back(5);

    if (ui->check6->isChecked())
        nbrs.push_back(6);

    if (ui->check7->isChecked())
        nbrs.push_back(7);

    if (ui->check8->isChecked())
        nbrs.push_back(8);

    if (ui->check9->isChecked())
        nbrs.push_back(9);

    if (ui->check10->isChecked())
        nbrs.push_back(10);

    return nbrs;
}

void MainWindow::goPressed()
{
    lastDisplayedStr_.clear();
    incrementTries();

    auto nbrs = getCheckedNumbers();

    quint32 nr = nbrs.at(QRandomGenerator::global()->generate() % nbrs.size());
    quint32 rand = (QRandomGenerator::global()->generate() % 8) + 2; // 2 to 10 (8+2)

    bool do_multi_ten{false};
    quint32 multi_ten_rand = (QRandomGenerator::global()->generate() % 2);
    if (ui->rb_mixed->isChecked() && (multi_ten_rand == 0))
    {
        do_multi_ten = true;
    }

    qDebug() << "mulit rand: " << multi_ten_rand;

    if (ui->rb_10->isChecked() || do_multi_ten)
    {
        if (QRandomGenerator::global()->generate() % 2 == 0)
        {
            nr *= 10;
        }
        else
        {
            rand *= 10;
        }
    }
    quint32 result = rand * nr;

    bool div{true};
    if (ui->rbMult->isChecked() == true)
    {
        div = false;
    }
    else if (ui->rbMixed->isChecked() == true)
    {
        if (QRandomGenerator::global()->generate() % 2 == 0)
        {
            div = false;
        }
    }

    if (div)
    {
        lastDisplayedStr_ = QString::number(result) + " : " + QString::number(nr) + " = ";
        result_ = rand;
    }
    else
    {
        lastDisplayedStr_ = QString::number(rand) + " * " + QString::number(nr) + " = ";
        result_ = result;
    }

    // display the text
    ui->textEdit->setText(lastDisplayedStr_);
    ui->textEdit->setAlignment(Qt::AlignCenter);

}

void MainWindow::testPressed()
{
    QString enteredText = ui->lineEditTest->text().trimmed();

    if (enteredText.isEmpty())
    {
        goPressed();
    }
    else
    {
        ui->lineEditTest->setDisabled(true);

        auto timeout = 600;
        quint32 enteredResult = enteredText.toInt();
        if (enteredResult == result_)
        {
            incrementCorrect();
            ui->lineEditTest->clear();
            ui->textEdit->setText("");

            setInputTextColor(0);
        }
        else
        {
            incrementFailure();
            setInputTextColor(1);
        }

        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->start(timeout);

        connect(timer, &QTimer::timeout, [=]() {
          this->setInputTextColor(2);
          ui->lineEditTest->setDisabled(false);
          ui->lineEditTest->setFocus();
          timer->deleteLater();
        } );

        lastDisplayedStr_.clear();
    }
}

void MainWindow::setInputTextColor(int color) // 0: green, 1: red, 2: white
{
    // display a colored background for a short period of time
    Qt::GlobalColor targetColor = Qt::green;
    if (color == 0)
    {
        quint32 nr = QRandomGenerator::global()->generate() % 200;
        QString img = QString::number(nr) + ".jpeg";
        ui->textEdit->setHtml("<center><img src=\"" + img + "\"/></center>");
    }
    else if (color == 1)
    {
        targetColor = Qt::red;
        //ui->textEdit->clear();
        //ui->textEdit->setText("");
    }
    else if (color == 2)
    {
        targetColor = Qt::white;
        //ui->textEdit->clear();
        //if (lastColor_ == 1) // red
        {
            //ui->textEdit->clear();
            //ui->textEdit->setText(lastDisplayedStr_);
        }
        //else
        //    ui->textEdit->clear();
    }


    QPalette pal = ui->textEdit->palette();
    pal.setColor(QPalette::Base, QColor(targetColor));
    ui->textEdit->setAutoFillBackground(true);
    ui->textEdit->setPalette(pal);
    ui->textEdit->update();

    lastColor_ = color;
}

void MainWindow::incrementTries()
{
    auto tries = ui->lcdNumberTries->value();
    tries++;
    ui->lcdNumberTries->display(tries);
}

void MainWindow::incrementCorrect()
{
    auto tries = ui->lcdNumberCorrect->value();
    tries++;
    ui->lcdNumberCorrect->display(tries);
}

void MainWindow::incrementFailure()
{
    auto tries = ui->lcdNumberWrong->value();
    tries++;
    ui->lcdNumberWrong->display(tries);
}

MainWindow::~MainWindow()
{
    delete ui;
}

