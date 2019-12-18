#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QCursor c = cursor();

private slots:
    void on_comboBox_activated(const QString &arg1);
    void on_label();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
