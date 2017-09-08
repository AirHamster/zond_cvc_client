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

#include <QtWidgets>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QFile>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QTextStream>
#include <QComboBox>
#include <QTime>
#include <QDate>
#include <QStringListModel>
#include "dialog.h"
#include <iostream>
QTextStream cout(stdout);
QTextStream cin(stdin);
//using namespace std;
const double minVoltage = -8;
const double maxVoltage = 50;
double currentVoltage = minVoltage;


//! [0]
Dialog::Dialog()
{
    createHorizontalGroupBox();
    createFormGroupBox();
    createHorizontalGroupBox2();
    zondDevice = new Device;
    const auto infos = QSerialPortInfo::availablePorts();
    serialPortComboBox = new QComboBox;
    for (const QSerialPortInfo &info : infos)
        serialPortComboBox->addItem(info.portName());

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    //connect(&thread, &MasterThread::response, this, &Dialog::transaction);
    connect(&thread, &MasterThread::error, this, &Dialog::processError);
    connect(&thread, &MasterThread::timeout, this, &Dialog::processTimeout);
    connect(&thread, &MasterThread::response1, this, &Dialog::responseProcessing);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(horizontalGroupBox2);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle("Zond CVC Meter Client");
}

void Dialog::createMenu()
{
    menuBar = new QMenuBar;
    fileMenu = new QMenu("&File", this);
    exitAction = fileMenu->addAction("E&xit");
    menuBar->addMenu(fileMenu);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(accept()));
}

void Dialog::createHorizontalGroupBox()
{

    horizontalGroupBox = new QGroupBox("Соединение");
    QHBoxLayout *layout = new QHBoxLayout;
    connectStatus = new QLabel("Нет соединения");
    searchDeviceButton = new QPushButton("Поиск");
    connect(searchDeviceButton, SIGNAL (released()), this, SLOT (searchDevice()));
    layout->addWidget(searchDeviceButton);
    layout->addWidget(connectStatus);
    horizontalGroupBox->setLayout(layout);
}

void Dialog::createFormGroupBox()
{
    formGroupBox = new QGroupBox("Настройки");
    QFormLayout *layout = new QFormLayout;
    requestLineEdit = new QLineEdit("Who are you?");
    vol_step = new QDoubleSpinBox;
    vol_step->setMinimum(0.25);
    vol_step->setMaximum(2);
    vol_step->setSingleStep(0.25);
    vol_step->setValue(1.0);

    filePathLine = new QLineEdit();
    filePathLine->setReadOnly(true);

    layout->addRow(new QLabel("Шаг измерений, V:"), vol_step);
    layout->addWidget(filePathLine);
    layout->addRow(new QLabel("Путь к файлу:"), filePathLine);

    savepath = new QPushButton("Задать имя...");
    connect(savepath, SIGNAL (released()), this, SLOT (handleSaveButton()));
    layout->addWidget(savepath);
    formGroupBox->setLayout(layout);
}

void Dialog::createHorizontalGroupBox2()
{
    horizontalGroupBox2 = new QGroupBox();
    QHBoxLayout *layout2 = new QHBoxLayout;
    //layout2->addWidget(serialPortComboBox);
    statusLabel = new QLabel("Нет подключения");

    runButton = new QPushButton("Старт");
    runButton->setEnabled(false);
    connect(runButton, &QPushButton::clicked, this, &Dialog::transaction);
    layout2->addWidget(runButton);
    layout2->addWidget(statusLabel);

    horizontalGroupBox2->setLayout(layout2);
}
//! [13]

void Dialog::handleSaveButton()
{
    QFileDialog *dialog = new QFileDialog;

    QString fileName = dialog->getSaveFileName(0, QString("Сохранить как..."), QString("./Saves/"), QString("*.csv"));
    if (fileName != "")
        {
            file = new QFile(QFileInfo(fileName).absoluteFilePath());
            if (file->open(QIODevice::WriteOnly))
            {
                QString text = "Дата:;";
                text.append(QDate::currentDate().toString("dd/MM/yy"));
                text.append("\nВремя:;");
                text.append(QTime::currentTime().toString());
                text.append("\nНапряжение,В;Ток,мА\n");
                QTextStream out(file);
                out << text;
                file->close();
            }
            else
            {
                //TODO: Error message
            }
        }
    filePathLine->setPlaceholderText(fileName);
//cout << fileName << endl;

}

//==Communication

void Dialog::transaction()
{

    cout << "Transaction..." << endl;
    statusLabel->setText("Сбор данных...");
    thread.transaction(serialPortComboBox->currentText(),1000,requestLineEdit->text());
}

void Dialog::processError(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    //trafficLabel->setText(tr("No traffic."));
}

void Dialog::processTimeout(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    if (avalibleCOMs.size() != 0)
    {
        thread.transaction(avalibleCOMs.takeLast(),1000, "Z");
    }else{

    }
   // trafficLabel->setText(tr("No traffic."));
}

void Dialog::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialPortComboBox->setEnabled(enable);
    //waitResponseSpinBox->setEnabled(enable);
    //requestLineEdit->setEnabled(enable);
}
//void Dialog::responseProcessing(const QString &s, QString &portname)
void Dialog::responseProcessing(const QString &s)
{
    if (s.toStdString() == "z\n"){
        zondDevice->setDevFound(true);
   //     zondDevice->setPortName(portname);
        //connectStatus->setText(tr("Прибор обнаружен на порту %1").arg(portname));
        connectStatus->setText("Прибор обнаружен");
        statusLabel->setText("Готово к работе");
        runButton->setEnabled(true);
        return;
    }else{
        connectStatus->setText("Прибор не найден");
    }
}

void Dialog::searchDevice()
{

    cout << "Searching..." << endl;
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    cout << infos.size() << endl;
    if(infos.size() == 0)
    {
        connectStatus->setText("Нет доступных устройств");
    }else{
        connectStatus->setText("Поиск...");
        for (const QSerialPortInfo &info : infos)
            serialPortComboBox->addItem(info.portName());

        QStringListModel* cbModel = new QStringListModel();
        serialPortComboBox->setModel(cbModel);
        //avalibleCOMs = new QStringList;
        avalibleCOMs = QStringList(cbModel->stringList());
         //thread.transaction(avalibleCOMs.takeFirst(),1000, "Z");
       // connect(&thread, &MasterThread::response, this, &Dialog::responseProcessing);
        thread.transaction("COM4",5000, "Z?\n");

    }
}

void Dialog::getValues()
{
    vol_step->setEnabled(false);
    savepath->setEnabled(false);
    //connect(&thread, &MasterThread::response1, this, &Dialog::saveToFile);
    currentVoltage += vol_step->value();
    if (currentVoltage <= maxVoltage)
    {
        thread.transaction(zondDevice->getPortName(),1000, "Z?");
    }else
    {
        currentVoltage = minVoltage;
    }
}
void Dialog::saveToFile(const QString &s, QString &portname)
{
    QTextStream out(file);
    QString *writeString = new QString;
    QString *voltageString = new QString;
    QString *currentString = new QString;
    int Vpos;
    int Cpos;
    Vpos = s.indexOf("V");
    Cpos = s.indexOf("C");
    voltageString->append(s.mid(Vpos, (Cpos-Vpos)));
    currentString->append(s.mid(Cpos, (s.length()-Cpos)));
    writeString->append(voltageString);
    writeString->append(";");
    writeString->append(currentString);
    writeString->append(";/n");
    getValues();
}
