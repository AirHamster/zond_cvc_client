/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
//#include "masterthread.h"
#include <QtSerialPort/QSerialPort>
QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QFile;
class QSerialPort;
class QTimer;
class QProgressBar;
QT_END_NAMESPACE
class Device
{
private:
    QString portName;
    bool devFound = false;

public:
    void setPortName(QString &s)
    {
        this->portName = s;
    }
    QString getPortName()
    {
        return(this->portName);
    }
    void setDevFound(bool val)
    {
        devFound = val;
    }
    bool getDevFound(void)
    {
        return devFound;
    }
};

//! [0]
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog();

public slots:
  //  void blocking(const QString &s);
private:
    void createMenu();
    void createHorizontalGroupBox();
   // void createGridGroupBox();
    void createFormGroupBox();
    void createHorizontalGroupBox2();
    void createHorizontalGroupBox3();
    void setControlsEnabled(bool enable);

    enum { NumGridRows = 3, NumButtons = 4 };

    QMenuBar *menuBar;
    QGroupBox *horizontalGroupBox;
    QGroupBox *horizontalGroupBox2;
    QGroupBox *horizontalGroupBox3;
    QGroupBox *gridGroupBox;
    QGroupBox *formGroupBox;
    QTextEdit *smallEditor;
    QTextEdit *bigEditor;
    QDoubleSpinBox *vol_step;
    QLabel *labels[NumGridRows];
    QLabel *statusLabel;
    QLineEdit *filePathLine;
    QLineEdit *lineEdits[NumGridRows];
    QPushButton *searchDeviceButton;
    QPushButton *runButton;
    QDialogButtonBox *buttonBox;
    QMenu *fileMenu;
    QAction *exitAction;
    QComboBox *serialPortComboBox;
    QStringList *avalibleCOMs;
    QLabel *waitResponseLabel;
    QSpinBox *waitResponseSpinBox;
    QLineEdit *requestLineEdit;
    QLabel *connectStatus;
    //QStringList avalibleCOMs;
    QFile *file;
    QPushButton *savepath;
    //MasterThread thread;
    Device *zondDevice;
    QSerialPort *serial;
    QTimer *waitTimer;
    QTimer *searchTimer;
    QProgressBar * progressBar;
    QList<QString> portList;
private slots:
void waitTimeout();
void searchTimeout();
void handleSaveButton();
void responseProcessing(const QString &s);
void searchDevice();
void getValues();
void saveToFile(const QString &s);
//void searchPortSwitch(const QString &s, QString &portname);
void openSerialPort();
void closeSerialPort();
void writeData(const QByteArray &data);
void readData();

void handleError(QSerialPort::SerialPortError error);
void handleRunButton();
};
//! [0]

#endif // DIALOG_H
