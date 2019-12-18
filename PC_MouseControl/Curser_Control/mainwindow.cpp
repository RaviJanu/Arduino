#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort>
#include <QString>
#include <QDebug>
#include <windows.h>

QSerialPort *serial;
QString char_array;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{    ui->setupUi(this);
     char_array = "";
     c.setShape(Qt::UpArrowCursor);
     setCursor(c);
}
/*
void QDropLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - offset));
    }
}
*/
MainWindow::~MainWindow()
{   delete ui;
    serial->close();
}

void MainWindow::on_label()
{    QByteArray SerialRead = serial->readAll();
     static char right_click_flag = 0,left_click_flag = 0;
     char_array += QString::fromStdString(SerialRead.toStdString());
     QStringList bufferSplit = char_array.split(",");

     if(bufferSplit.length() > 4)
     {    ui->label_2->setText(bufferSplit[0]);
          ui->label_3->setText(bufferSplit[1]);
          int x = bufferSplit[0].toInt();
          int y = bufferSplit[1].toInt();
          x *= 7;
          y *= 4;
          c.setPos(x,y);

          if(bufferSplit[3] == "0")
          {     mouse_event(MOUSEEVENTF_RIGHTDOWN,x,y, 0,0);
                qDebug() << "Right Down";
                right_click_flag = 0;
          }
          else
          {   if(right_click_flag == 0)
              {     mouse_event(MOUSEEVENTF_RIGHTUP,x,y, 0,0);
                    right_click_flag = 1;
              }
          }
          if(bufferSplit[4] == "0__")
          {     mouse_event(MOUSEEVENTF_LEFTDOWN,x,y, 0,0);
                qDebug() << "Left Down";
                left_click_flag = 0;
          }
          else
          {    if(left_click_flag == 0)
               {    mouse_event(MOUSEEVENTF_LEFTUP,x,y, 0,0);
                    left_click_flag = 1;
               }
          }

     }
     if(char_array.contains("__"))
     {   qDebug() << char_array;
         ui->label->setText(char_array);
         char_array = "";
     }
}


void MainWindow::on_comboBox_activated(const QString &arg1)
{      ui->label->setText(arg1);
       serial = new QSerialPort(this);
       serial->setPortName("COM5");
       serial->setBaudRate(QSerialPort::Baud9600);
       serial->setDataBits(QSerialPort::Data8);
       serial->setParity(QSerialPort::NoParity);
       serial->setStopBits(QSerialPort::OneStop);
       serial->setFlowControl(QSerialPort::NoFlowControl);
       if (serial->open(QIODevice::ReadWrite))
       {   qDebug() << "Connected";
        }
       else
       {   qDebug() << "Open error";
        }
       connect(serial,SIGNAL(readyRead()),this,SLOT(on_label()));
}

