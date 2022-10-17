#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void incrementTries();
    void incrementCorrect();
    void incrementFailure();
    void setInputTextColor(int color);
    QVector<int> getCheckedNumbers();

    QString lastDisplayedStr_{};
    Ui::MainWindow *ui;

    quint32 result_{0};
    int lastColor_{0};

private slots:
    void goPressed();
    void testPressed();
};
#endif // MAINWINDOW_H
