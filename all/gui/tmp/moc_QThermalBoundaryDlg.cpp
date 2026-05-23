/****************************************************************************
** Meta object code from reading C++ file 'QThermalBoundaryDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/HPprocess/QThermalBoundaryDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QThermalBoundaryDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTBConst[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      21,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTBConst[] = {
    "QTBConst\0\0m_OKBtnS()\0m_CancelBtnS()\0"
};

void QTBConst::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTBConst *_t = static_cast<QTBConst *>(_o);
        switch (_id) {
        case 0: _t->m_OKBtnS(); break;
        case 1: _t->m_CancelBtnS(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QTBConst::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTBConst::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QTBConst,
      qt_meta_data_QTBConst, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTBConst::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTBConst::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTBConst::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTBConst))
        return static_cast<void*>(const_cast< QTBConst*>(this));
    return QDialog::qt_metacast(_clname);
}

int QTBConst::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_ThermalBoundary[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      50,   16,   16,   16, 0x05,
      72,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     104,   16,   16,   16, 0x0a,
     119,   16,   16,   16, 0x0a,
     134,   16,   16,   16, 0x0a,
     148,   16,   16,   16, 0x0a,
     163,   16,   16,   16, 0x0a,
     185,   16,   16,   16, 0x0a,
     201,   16,   16,   16, 0x0a,
     232,  216,   16,   16, 0x0a,
     252,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ThermalBoundary[] = {
    "ThermalBoundary\0\0emitPsetHBHighLight(NElSurfChsS)\0"
    "emitHotName(m_TBName)\0"
    "emitHotName2(QMap<int,QString>)\0"
    "AddBtnHBSlot()\0DelBtnHBSlot()\0"
    "ApplyHBSlot()\0CancelHBSlot()\0"
    "ConstantChangesSlot()\0NextHBBtnSlot()\0"
    "PreHBBtnSlot()\0comWigId,index0\0"
    "comboxSlot(int,int)\0"
    "AppendSurfSetComboxTBSlot(QString)\0"
};

void ThermalBoundary::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ThermalBoundary *_t = static_cast<ThermalBoundary *>(_o);
        switch (_id) {
        case 0: _t->emitPsetHBHighLight((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        case 1: _t->emitHotName((*reinterpret_cast< m_TBName(*)>(_a[1]))); break;
        case 2: _t->emitHotName2((*reinterpret_cast< QMap<int,QString>(*)>(_a[1]))); break;
        case 3: _t->AddBtnHBSlot(); break;
        case 4: _t->DelBtnHBSlot(); break;
        case 5: _t->ApplyHBSlot(); break;
        case 6: _t->CancelHBSlot(); break;
        case 7: _t->ConstantChangesSlot(); break;
        case 8: _t->NextHBBtnSlot(); break;
        case 9: _t->PreHBBtnSlot(); break;
        case 10: _t->comboxSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->AppendSurfSetComboxTBSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ThermalBoundary::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ThermalBoundary::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ThermalBoundary,
      qt_meta_data_ThermalBoundary, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ThermalBoundary::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ThermalBoundary::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ThermalBoundary::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ThermalBoundary))
        return static_cast<void*>(const_cast< ThermalBoundary*>(this));
    return QDialog::qt_metacast(_clname);
}

int ThermalBoundary::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ThermalBoundary::emitPsetHBHighLight(NElSurfChsS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ThermalBoundary::emitHotName(m_TBName _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ThermalBoundary::emitHotName2(QMap<int,QString> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
