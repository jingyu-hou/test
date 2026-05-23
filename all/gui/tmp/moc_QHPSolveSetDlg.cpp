/****************************************************************************
** Meta object code from reading C++ file 'QHPSolveSetDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/HPprocess/QHPSolveSetDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QHPSolveSetDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHPSolveSetDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      38,   32,   15,   15, 0x0a,
      62,   15,   15,   15, 0x0a,
      78,   15,   15,   15, 0x0a,
      94,   91,   15,   15, 0x0a,
     119,  115,   15,   15, 0x0a,
     157,   15,   15,   15, 0x0a,
     175,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QHPSolveSetDlg[] = {
    "QHPSolveSetDlg\0\0AllChooseSlot()\0Index\0"
    "ChangeTimeFreqSlot(int)\0ApplayBtnSlot()\0"
    "PreBtnSlot()\0id\0ShowAndHideSlot(int)\0"
    "str\0AppendPSetComboxSolvesetSlot(QString)\0"
    "AddDeletSolt(int)\0MCFSlot(int)\0"
};

void QHPSolveSetDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QHPSolveSetDlg *_t = static_cast<QHPSolveSetDlg *>(_o);
        switch (_id) {
        case 0: _t->AllChooseSlot(); break;
        case 1: _t->ChangeTimeFreqSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ApplayBtnSlot(); break;
        case 3: _t->PreBtnSlot(); break;
        case 4: _t->ShowAndHideSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->AppendPSetComboxSolvesetSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->AddDeletSolt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->MCFSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QHPSolveSetDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QHPSolveSetDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QHPSolveSetDlg,
      qt_meta_data_QHPSolveSetDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHPSolveSetDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHPSolveSetDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHPSolveSetDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHPSolveSetDlg))
        return static_cast<void*>(const_cast< QHPSolveSetDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QHPSolveSetDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
