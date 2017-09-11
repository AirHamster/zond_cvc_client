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
#include <QProgressBar>
#include "dialog.h"
#include <iostream>
QTextStream cout(stdout);
QTextStream cin(stdin);
const double minVoltage = -8;
const double maxVoltage = 50;
const double singleStep = (1/(maxVoltage-minVoltage));
double currentVoltage = minVoltage;

Dialog::Dialog()
{
    createHorizontalGroupBox();
    createFormGroupBox();
    createHorizontalGroupBox2();
    createHorizontalGroupBox3();
    zondDevice = new Device;
    QList<QString> portList;
    serial = new QSerialPort(this);
    const auto infos = QSerialPortInfo::availablePorts();
    serialPortComboBox = new QComboBox;
    for (const QSerialPortInfo &info : infos)
        serialPortComboBox->addItem(info.portName());

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &Dialog::handleError);
    connect(serial, &QSerialPort::readyRead, this, &Dialog::readData);
    waitTimer = new QTimer(this);
    searchTimer = new QTimer(this);
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(waitTimeout()));
    connect(searchTimer, SIGNAL(timeout()), this, SLOT(searchTimeout()));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    file = new QFile;

    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(horizontalGroupBox2);
    mainLayout->addWidget(horizontalGroupBox3);
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
    savepath = new QPushButton("Задать имя...");
    connect(savepath, SIGNAL (released()), this, SLOT (handleSaveButton()));
    layout->addRow(new QLabel("Шаг измерений, V:"), vol_step);
    // layout->addWidget(filePathLine);
    layout->addRow(new QLabel("Путь к файлу:"), filePathLine);
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
    connect(runButton, &QPushButton::clicked, this, &Dialog::getValues);
    layout2->addWidget(runButton);
    layout2->addWidget(statusLabel);


    horizontalGroupBox2->setLayout(layout2);
}

void Dialog::createHorizontalGroupBox3()
{
    horizontalGroupBox3 = new QGroupBox();
    QHBoxLayout *layout3 = new QHBoxLayout;
    progressBar = new QProgressBar();
    progressBar->setAlignment(Qt::AlignHCenter);
    progressBar->setMinimum(0);
    progressBar->setTextVisible(false);
    layout3->addWidget(progressBar);
    horizontalGroupBox3->setLayout(layout3);
}

void Dialog::handleSaveButton()
{
    QFileDialog *dialog = new QFileDialog;

    QString fileName = dialog->getSaveFileName(0, QString("Сохранить как..."), QString("./Saves/"), QString("*.csv"));
    if (fileName != "")
    {
        file->setFileName(QFileInfo(fileName).absoluteFilePath());
        if (file->open(QIODevice::WriteOnly))
        {
            QString text = "Дата:;";
            text.append(QDate::currentDate().toString("dd/MM/yy"));
            text.append("\nВремя:;");
            text.append(QTime::currentTime().toString());
            text.append("\nНапряжение,В;Ток,мкА\n");
            QTextStream out(file);
            out << text;
            //file->close();
        }
        else
        {
            return;
            //TODO: Error message
        }
    }
    filePathLine->setPlaceholderText(fileName);
    //cout << fileName << endl;

}


void Dialog::responseProcessing(const QString &s)
{
    if (s.toStdString() == "z\n"){
        searchTimer->stop();
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
    serial->close();
    cout << "Searching..." << endl;
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    cout << infos.size() << endl;
    if(infos.size() == 0)
    {
        connectStatus->setText("Нет доступных устройств");
    }else{
        connectStatus->setText("Поиск...");
        for (const QSerialPortInfo &info : infos)
           portList << info.portName();
            //serialPortComboBox->addItem(info.portName());

      //  QStringListModel* cbModel = new QStringListModel();
       // serialPortComboBox->setModel(cbModel);
        //avalibleCOMs = new QStringList;
       // avalibleCOMs = new QStringList(cbModel->stringList());
        serial->setPortName(portList.takeLast());
        searchTimer->start(500);
        openSerialPort();
    }
}

void Dialog::getValues()
{
    vol_step->setEnabled(false);
    savepath->setEnabled(false);
    runButton->setEnabled(false);
    searchDeviceButton->setEnabled(false);
    progressBar->setMaximum((maxVoltage-minVoltage)/vol_step->value());
    progressBar->setValue(progressBar->value()+1);
    if (currentVoltage <= maxVoltage)
    {
        QString *getRequest = new QString;
        getRequest->append("get ");
        getRequest->append(QString::number(currentVoltage));
        getRequest->append("\n");
        writeData(QByteArray (getRequest->toUtf8()));
        waitTimer->start(1000);
        statusLabel->setText("Измерения выполняются");
        currentVoltage += vol_step->value();
    }else
    {
        currentVoltage = minVoltage;
        waitTimer->stop();
        writeData(QByteArray ("stop\n"));
        statusLabel->setText("Завершено");
        vol_step->setEnabled(true);
        savepath->setEnabled(true);
        runButton->setEnabled(true);
        searchDeviceButton->setEnabled(true);
        progressBar->setValue(0);
        file->close();
    }
}

void Dialog::openSerialPort()
{
    //serial->setPortName(portname);
    serial->setBaudRate(115200);
    if (serial->open(QIODevice::ReadWrite)) {
        cout << "Opened" << endl;
        writeData("Z?\n");
    } else {
        cout << "Not Opened" << endl;
    }
}

void Dialog::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
}

void Dialog::writeData(const QByteArray &data)
{
    serial->write(data);
}

void Dialog::readData()
{
    QByteArray data = serial->readAll();
    cout << data << endl;
    if (zondDevice->getDevFound() == true) {
        saveToFile(QString(data));
    }else{
        responseProcessing(QString(data));
    }
}

void Dialog::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        //QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        statusLabel->setText("Ошибка!");
        closeSerialPort();
    }
}


void Dialog::saveToFile(const QString &s)
{
    if (!(file->isOpen())) {
        file = new QFile;
        file->setFileName("./probe_results.csv");
        if (file->open(QIODevice::WriteOnly)) {
            QString text = "Дата:;";
            text.append(QDate::currentDate().toString("dd/MM/yy"));
            text.append("\nВремя:;");
            text.append(QTime::currentTime().toString());
            text.append("\nНапряжение,В;Ток,мкА\n");
            QTextStream out(file);
            out << text;
        }else {
            return;
        }
    }
    waitTimer->stop();
    QTextStream out(file);
    QString *writeString = new QString;
    QString *voltageString = new QString;
    QString *currentString = new QString;
    int Vpos;
    int Cpos;
    Vpos = s.indexOf("V");
    Cpos = s.indexOf("C");
    voltageString->append(s.mid((Vpos+1), (Cpos-Vpos-1)));
    currentString->append(s.mid((Cpos+1), (s.length()-Cpos-1)));
    writeString->append(voltageString);
    writeString->append(";");
    writeString->append(currentString);
    writeString->remove(QChar('\n'), Qt::CaseInsensitive);
    writeString->append(";\n");
    writeString->replace(QChar('.'), QChar(','), Qt::CaseSensitive);
    cout << *writeString << endl;
    out << *writeString;
    getValues();
}

void Dialog::waitTimeout()
{
    statusLabel->setText("Устройство не отвечает");
}

void Dialog::searchTimeout()
{
    if (portList.size() !=0) {
     serial->setPortName(portList.takeLast());
    }else {
     connectStatus->setText("Прибор не найден");
     searchTimer->stop();
    }
    //serial->setPortName(avalibleCOMs->takeLast());
    //connectStatus->setText("");
}
