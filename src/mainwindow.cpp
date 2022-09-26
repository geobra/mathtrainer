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
    ui->lcdNumberTime->display(3);

    ui->check2->setChecked(true);
    ui->check3->setChecked(true);
    ui->check4->setChecked(true);
    ui->check5->setChecked(true);
    ui->check6->setChecked(true);
    ui->check7->setChecked(true);
    ui->check8->setChecked(true);
    ui->check9->setChecked(true);

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

    // time and chars buttons to display
    connect(ui->pushButtonTimeAdd, SIGNAL(clicked()), this, SLOT(lcdTimeInc()));
    connect(ui->pushButtonTimeSub, SIGNAL(clicked()), this, SLOT(lcdTimeDec()));

    // return on lineEdit
    connect(ui->lineEditTest, SIGNAL(returnPressed()), this, SLOT(testPressed()));

    ui->lineEditTest->setFocus();
}

void MainWindow::lcdCharInc()
{
}

void MainWindow::lcdCharDec()
{
}

void MainWindow::lcdTimeInc()
{
    auto value = ui->lcdNumberTime->value();
    if (value < 20)
    {
        ui->lcdNumberTime->display(value + 1);
    }
}

void MainWindow::lcdTimeDec()
{
    auto value = ui->lcdNumberTime->value();
    if (value > 1)
    {
        ui->lcdNumberTime->display(value - 1);
    }
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
    if (ui->zehner->isChecked())
    {
        nr *= 10;
    }
    quint32 rand = (QRandomGenerator::global()->generate() % 9) + 1;
    quint32 result = rand * nr;

    if (ui->geteilt->isChecked())
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
            //timeout = 2000;

            setInputTextColor(1);
            //ui->textEdit->setText(lastDisplayedStr_ + " (" + enteredText + ")");
            //ui->textEdit->setAlignment(Qt::AlignCenter);
        }

        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->start(timeout);

        connect(timer, &QTimer::timeout, [=]() {
          this->setInputTextColor(2);
          //ui->textEdit->clear();
          ui->lineEditTest->setDisabled(false);
          ui->lineEditTest->setFocus();
          timer->deleteLater();
        } );

        lastDisplayedStr_.clear();
    }
}

void MainWindow::setInputTextColor(int color) // 0: green, 1: orange, 2: white
{
    // display a colored background for a short period of time
    Qt::GlobalColor targetColor = Qt::green;
    if (color == 1)
        targetColor = Qt::red;
    else if (color == 2)
        targetColor = Qt::white;

    QPalette pal = ui->textEdit->palette();
    pal.setColor(QPalette::Base, QColor(targetColor));
    ui->textEdit->setAutoFillBackground(true);
    ui->textEdit->setPalette(pal);
    ui->textEdit->update();
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


QChar MainWindow::getRandomSymbol()
{
    QString symbols = {"!#$%&()+-/<>=?[]{}@*§\"\\~.,;_|°^'"};
    QChar c = symbols.at(QRandomGenerator::global()->generate() % symbols.size());

    return c;
}

QChar MainWindow::getRandomChar()
{
    int diff = 'Z'-'A';
    char c = 'A'+(QRandomGenerator::global()->generate() % diff);

    return c;
}

MainWindow::~MainWindow()
{
    delete ui;
}

