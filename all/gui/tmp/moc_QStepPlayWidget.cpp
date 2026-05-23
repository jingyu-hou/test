/****************************************************************************
** Meta object code from reading C++ file 'QStepPlayWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QStepPlayWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QStepPlayWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QPlaySetDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPlaySetDlg[] = {
    "QPlaySetDlg\0\0OKSetBtnSlot()\0"
};

void QPlaySetDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPlaySetDlg *_t = static_cast<QPlaySetDlg *>(_o);
        switch (_id) {
        case 0: _t->OKSetBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QPlaySetDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPlaySetDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QPlaySetDlg,
      qt_meta_data_QPlaySetDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPlaySetDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPlaySetDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPlaySetDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPlaySetDlg))
        return static_cast<void*>(const_cast< QPlaySetDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QPlaySetDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_QStepPlayWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   16,   16,   16, 0x08,
      77,   16,   16,   16, 0x08,
     103,  100,   16,   16, 0x08,
     126,   16,   16,   16, 0x08,
     141,   16,   16,   16, 0x08,
     156,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QStepPlayWidget[] = {
    "QStepPlayWidget\0\0ResultVis\0"
    "emitPlayStepParam(StepPlayVisS)\0"
    "GroupBtnSlot(int)\0ComboxParam01Slot(int)\0"
    "id\0ComboxParam02Slot(int)\0AutoPlaySlot()\0"
    "NowOKBtnSlot()\0SetUpBtnSlot()\0"
};

void QStepPlayWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QStepPlayWidget *_t = static_cast<QStepPlayWidget *>(_o);
        switch (_id) {
        case 0: _t->emitPlayStepParam((*reinterpret_cast< StepPlayVisS(*)>(_a[1]))); break;
        case 1: _t->GroupBtnSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ComboxParam01Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->ComboxParam02Slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->AutoPlaySlot(); break;
        case 5: _t->NowOKBtnSlot(); break;
        case 6: _t->SetUpBtnSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QStepPlayWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QStepPlayWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QStepPlayWidget,
      qt_meta_data_QStepPlayWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QStepPlayWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QStepPlayWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QStepPlayWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QStepPlayWidget))
        return static_cast<void*>(const_cast< QStepPlayWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QStepPlayWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QStepPlayWidget::emitPlayStepParam(StepPlayVisS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
