/****************************************************************************
** Meta object code from reading C++ file 'QVariableOutputDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QVariableOutputDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QVariableOutputDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QVariableOutputDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      42,   36,   19,   19, 0x08,
      66,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QVariableOutputDlg[] = {
    "QVariableOutputDlg\0\0AllChooseSlot()\0"
    "Index\0ChangeTimeFreqSlot(int)\0OKBtnSlot()\0"
};

void QVariableOutputDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QVariableOutputDlg *_t = static_cast<QVariableOutputDlg *>(_o);
        switch (_id) {
        case 0: _t->AllChooseSlot(); break;
        case 1: _t->ChangeTimeFreqSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OKBtnSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QVariableOutputDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QVariableOutputDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QVariableOutputDlg,
      qt_meta_data_QVariableOutputDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QVariableOutputDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QVariableOutputDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QVariableOutputDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QVariableOutputDlg))
        return static_cast<void*>(const_cast< QVariableOutputDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QVariableOutputDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
