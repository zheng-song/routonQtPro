/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Widget_t {
    QByteArrayData data[21];
    char stringdata[267];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Widget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Widget_t qt_meta_stringdata_Widget = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Widget"
QT_MOC_LITERAL(1, 7, 8), // "slotPlay"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 8), // "slotStop"
QT_MOC_LITERAL(4, 26, 8), // "slotStep"
QT_MOC_LITERAL(5, 35, 12), // "slotBackward"
QT_MOC_LITERAL(6, 48, 8), // "slotMute"
QT_MOC_LITERAL(7, 57, 12), // "slotOpenFile"
QT_MOC_LITERAL(8, 70, 17), // "slotSliderChanged"
QT_MOC_LITERAL(9, 88, 5), // "value"
QT_MOC_LITERAL(10, 94, 14), // "slotVolumeDown"
QT_MOC_LITERAL(11, 109, 12), // "slotVolumeUp"
QT_MOC_LITERAL(12, 122, 20), // "slotVideoDataReceive"
QT_MOC_LITERAL(13, 143, 16), // "slotVideoStarted"
QT_MOC_LITERAL(14, 160, 17), // "slotVideoFinished"
QT_MOC_LITERAL(15, 178, 8), // "exitCode"
QT_MOC_LITERAL(16, 187, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(17, 208, 10), // "exitStatus"
QT_MOC_LITERAL(18, 219, 15), // "slotGetTimeInfo"
QT_MOC_LITERAL(19, 235, 12), // "slotCloseAPP"
QT_MOC_LITERAL(20, 248, 18) // "slotSliderReleased"

    },
    "Widget\0slotPlay\0\0slotStop\0slotStep\0"
    "slotBackward\0slotMute\0slotOpenFile\0"
    "slotSliderChanged\0value\0slotVolumeDown\0"
    "slotVolumeUp\0slotVideoDataReceive\0"
    "slotVideoStarted\0slotVideoFinished\0"
    "exitCode\0QProcess::ExitStatus\0exitStatus\0"
    "slotGetTimeInfo\0slotCloseAPP\0"
    "slotSliderReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Widget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x09 /* Protected */,
       3,    0,   90,    2, 0x09 /* Protected */,
       4,    0,   91,    2, 0x09 /* Protected */,
       5,    0,   92,    2, 0x09 /* Protected */,
       6,    0,   93,    2, 0x09 /* Protected */,
       7,    0,   94,    2, 0x09 /* Protected */,
       8,    1,   95,    2, 0x09 /* Protected */,
      10,    0,   98,    2, 0x09 /* Protected */,
      11,    0,   99,    2, 0x09 /* Protected */,
      12,    0,  100,    2, 0x09 /* Protected */,
      13,    0,  101,    2, 0x09 /* Protected */,
      14,    2,  102,    2, 0x09 /* Protected */,
      18,    0,  107,    2, 0x09 /* Protected */,
      19,    0,  108,    2, 0x09 /* Protected */,
      20,    0,  109,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 16,   15,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Widget *_t = static_cast<Widget *>(_o);
        switch (_id) {
        case 0: _t->slotPlay(); break;
        case 1: _t->slotStop(); break;
        case 2: _t->slotStep(); break;
        case 3: _t->slotBackward(); break;
        case 4: _t->slotMute(); break;
        case 5: _t->slotOpenFile(); break;
        case 6: _t->slotSliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slotVolumeDown(); break;
        case 8: _t->slotVolumeUp(); break;
        case 9: _t->slotVideoDataReceive(); break;
        case 10: _t->slotVideoStarted(); break;
        case 11: _t->slotVideoFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 12: _t->slotGetTimeInfo(); break;
        case 13: _t->slotCloseAPP(); break;
        case 14: _t->slotSliderReleased(); break;
        default: ;
        }
    }
}

const QMetaObject Widget::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Widget.data,
      qt_meta_data_Widget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Widget.stringdata))
        return static_cast<void*>(const_cast< Widget*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
