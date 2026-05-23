/****************************************************************************
** Meta object code from reading C++ file 'QInitDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QInitDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QInitDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QInitDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,    9,    9,    9, 0x0a,
      56,    9,    9,    9, 0x0a,
      69,    9,    9,    9, 0x0a,
      85,    9,    9,    9, 0x0a,
     100,   98,    9,    9, 0x0a,
     123,   98,    9,    9, 0x0a,
     153,  149,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QInitDlg[] = {
    "QInitDlg\0\0emitPsetHighLight(NElSurfChsS)\0"
    "ApplyBtnSlot()\0AddBtnSlot()\0CancelBtnSlot()\0"
    "DelBtnSlot()\0,\0comBoxChgSlot(int,int)\0"
    "comBoxDoubleSlot(int,int)\0str\0"
    "AppendPSetComboxIniSlot(QString)\0"
};

void QInitDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QInitDlg *_t = static_cast<QInitDlg *>(_o);
        switch (_id) {
        case 0: _t->emitPsetHighLight((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        case 1: _t->ApplyBtnSlot(); break;
        case 2: _t->AddBtnSlot(); break;
        case 3: _t->CancelBtnSlot(); break;
        case 4: _t->DelBtnSlot(); break;
        case 5: _t->comBoxChgSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->comBoxDoubleSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->AppendPSetComboxIniSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QInitDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QInitDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QInitDlg,
      qt_meta_data_QInitDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QInitDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QInitDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QInitDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QInitDlg))
        return static_cast<void*>(const_cast< QInitDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QInitDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void QInitDlg::emitPsetHighLight(NElSurfChsS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
