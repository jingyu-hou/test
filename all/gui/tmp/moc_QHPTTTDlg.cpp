/****************************************************************************
** Meta object code from reading C++ file 'QHPTTTDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/HPprocess/QHPTTTDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QHPTTTDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHPTTTDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      27,   10,   10,   10, 0x08,
      45,   10,   10,   10, 0x08,
      58,   10,   10,   10, 0x08,
      73,   10,   10,   10, 0x08,
      93,   10,   10,   10, 0x08,
     110,   10,   10,   10, 0x08,
     127,   10,   10,   10, 0x08,
     139,   10,   10,   10, 0x08,
     152,   10,   10,   10, 0x08,
     167,   10,   10,   10, 0x08,
     187,   10,   10,   10, 0x08,
     243,  231,  207,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QHPTTTDlg[] = {
    "QHPTTTDlg\0\0GestationSlot()\0CrystalSizeSlot()\0"
    "QianReSlot()\0ReWuXingSlot()\0"
    "InsertLineBtnSlot()\0AddLineBtnSlot()\0"
    "DelLineBtnSlot()\0OkBtnSlot()\0SetBtnSLot()\0"
    "CalYunyuSlot()\0ImportLineBtnSlot()\0"
    "ExportLineBtnSlot()\0QMap<int,PhaseBaseItem>\0"
    "BalanceList\0ProcessMapData(QList<HPphaseBalanceInps>)\0"
};

void QHPTTTDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QHPTTTDlg *_t = static_cast<QHPTTTDlg *>(_o);
        switch (_id) {
        case 0: _t->GestationSlot(); break;
        case 1: _t->CrystalSizeSlot(); break;
        case 2: _t->QianReSlot(); break;
        case 3: _t->ReWuXingSlot(); break;
        case 4: _t->InsertLineBtnSlot(); break;
        case 5: _t->AddLineBtnSlot(); break;
        case 6: _t->DelLineBtnSlot(); break;
        case 7: _t->OkBtnSlot(); break;
        case 8: _t->SetBtnSLot(); break;
        case 9: _t->CalYunyuSlot(); break;
        case 10: _t->ImportLineBtnSlot(); break;
        case 11: _t->ExportLineBtnSlot(); break;
        case 12: { QMap<int,PhaseBaseItem> _r = _t->ProcessMapData((*reinterpret_cast< QList<HPphaseBalanceInps>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QMap<int,PhaseBaseItem>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QHPTTTDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QHPTTTDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QHPTTTDlg,
      qt_meta_data_QHPTTTDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHPTTTDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHPTTTDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHPTTTDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHPTTTDlg))
        return static_cast<void*>(const_cast< QHPTTTDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QHPTTTDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
