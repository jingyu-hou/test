/****************************************************************************
** Meta object code from reading C++ file 'QHIPsystemDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QHIPsystemDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QHIPsystemDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHIPsystemDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      48,   46,   14,   14, 0x05,
      91,   46,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     136,   14,   14,   14, 0x0a,
     149,   14,   14,   14, 0x0a,
     168,   14,   14,   14, 0x0a,
     199,  188,   14,   14, 0x0a,
     258,  248,   14,   14, 0x0a,
     284,  276,   14,   14, 0x0a,
     312,  276,   14,   14, 0x0a,
     334,  330,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QHIPsystemDlg[] = {
    "QHIPsystemDlg\0\0emitPsetHighLight(NElSurfChsS)\0"
    ",\0emitHIPCurveData(QStringList,InpCurveInpS)\0"
    "emitUpDataCurveTreeName(QString,QStringList)\0"
    "comBoxSlot()\0PreHipSysBtnSlot()\0"
    "NextHipSysBtnSlot()\0,CurveData\0"
    "updateHIPCurveDataSlot(QStringList,InpCurveInpS)\0"
    "Row,index\0NewCurve(int,int)\0strName\0"
    "updataFromTreeSlot(QString)\0"
    "DelCurve(QString)\0str\0"
    "AppendSurfSetComboxSlot(QString)\0"
};

void QHIPsystemDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QHIPsystemDlg *_t = static_cast<QHIPsystemDlg *>(_o);
        switch (_id) {
        case 0: _t->emitPsetHighLight((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        case 1: _t->emitHIPCurveData((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< InpCurveInpS(*)>(_a[2]))); break;
        case 2: _t->emitUpDataCurveTreeName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 3: _t->comBoxSlot(); break;
        case 4: _t->PreHipSysBtnSlot(); break;
        case 5: _t->NextHipSysBtnSlot(); break;
        case 6: _t->updateHIPCurveDataSlot((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< InpCurveInpS(*)>(_a[2]))); break;
        case 7: _t->NewCurve((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->updataFromTreeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->DelCurve((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->AppendSurfSetComboxSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QHIPsystemDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QHIPsystemDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QHIPsystemDlg,
      qt_meta_data_QHIPsystemDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHIPsystemDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHIPsystemDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHIPsystemDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHIPsystemDlg))
        return static_cast<void*>(const_cast< QHIPsystemDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QHIPsystemDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void QHIPsystemDlg::emitPsetHighLight(NElSurfChsS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QHIPsystemDlg::emitHIPCurveData(QStringList _t1, InpCurveInpS _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QHIPsystemDlg::emitUpDataCurveTreeName(QString _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
