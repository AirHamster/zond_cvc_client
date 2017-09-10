/****************************************************************************
** Meta object code from reading C++ file 'dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Dialog_t {
    QByteArrayData data[22];
    char stringdata0[257];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Dialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Dialog_t qt_meta_stringdata_Dialog = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Dialog"
QT_MOC_LITERAL(1, 7, 8), // "blocking"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 1), // "s"
QT_MOC_LITERAL(4, 19, 11), // "waitTimeout"
QT_MOC_LITERAL(5, 31, 13), // "searchTimeout"
QT_MOC_LITERAL(6, 45, 16), // "handleSaveButton"
QT_MOC_LITERAL(7, 62, 11), // "transaction"
QT_MOC_LITERAL(8, 74, 12), // "processError"
QT_MOC_LITERAL(9, 87, 14), // "processTimeout"
QT_MOC_LITERAL(10, 102, 18), // "responseProcessing"
QT_MOC_LITERAL(11, 121, 12), // "searchDevice"
QT_MOC_LITERAL(12, 134, 9), // "getValues"
QT_MOC_LITERAL(13, 144, 10), // "saveToFile"
QT_MOC_LITERAL(14, 155, 14), // "openSerialPort"
QT_MOC_LITERAL(15, 170, 15), // "closeSerialPort"
QT_MOC_LITERAL(16, 186, 9), // "writeData"
QT_MOC_LITERAL(17, 196, 4), // "data"
QT_MOC_LITERAL(18, 201, 8), // "readData"
QT_MOC_LITERAL(19, 210, 11), // "handleError"
QT_MOC_LITERAL(20, 222, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(21, 251, 5) // "error"

    },
    "Dialog\0blocking\0\0s\0waitTimeout\0"
    "searchTimeout\0handleSaveButton\0"
    "transaction\0processError\0processTimeout\0"
    "responseProcessing\0searchDevice\0"
    "getValues\0saveToFile\0openSerialPort\0"
    "closeSerialPort\0writeData\0data\0readData\0"
    "handleError\0QSerialPort::SerialPortError\0"
    "error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x0a /* Public */,
       4,    0,   97,    2, 0x08 /* Private */,
       5,    0,   98,    2, 0x08 /* Private */,
       6,    0,   99,    2, 0x08 /* Private */,
       7,    0,  100,    2, 0x08 /* Private */,
       8,    1,  101,    2, 0x08 /* Private */,
       9,    1,  104,    2, 0x08 /* Private */,
      10,    1,  107,    2, 0x08 /* Private */,
      11,    0,  110,    2, 0x08 /* Private */,
      12,    0,  111,    2, 0x08 /* Private */,
      13,    1,  112,    2, 0x08 /* Private */,
      14,    0,  115,    2, 0x08 /* Private */,
      15,    0,  116,    2, 0x08 /* Private */,
      16,    1,  117,    2, 0x08 /* Private */,
      18,    0,  120,    2, 0x08 /* Private */,
      19,    1,  121,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Dialog *_t = static_cast<Dialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->blocking((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->waitTimeout(); break;
        case 2: _t->searchTimeout(); break;
        case 3: _t->handleSaveButton(); break;
        case 4: _t->transaction(); break;
        case 5: _t->processError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->processTimeout((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->responseProcessing((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->searchDevice(); break;
        case 9: _t->getValues(); break;
        case 10: _t->saveToFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->openSerialPort(); break;
        case 12: _t->closeSerialPort(); break;
        case 13: _t->writeData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 14: _t->readData(); break;
        case 15: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog.data,
      qt_meta_data_Dialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog.stringdata0))
        return static_cast<void*>(const_cast< Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
