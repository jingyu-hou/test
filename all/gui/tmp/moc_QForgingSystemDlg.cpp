/****************************************************************************
** Meta object code from reading C++ file 'QForgingSystemDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/HPprocess/QForgingSystemDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QForgingSystemDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QFSConst[] = {

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

static const char qt_meta_stringdata_QFSConst[] = {
    "QFSConst\0\0m_OKBtnS()\0m_CancelBtnS()\0"
};

void QFSConst::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QFSConst *_t = static_cast<QFSConst *>(_o);
        switch (_id) {
        case 0: _t->m_OKBtnS(); break;
        case 1: _t->m_CancelBtnS(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QFSConst::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QFSConst::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QFSConst,
      qt_meta_data_QFSConst, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QFSConst::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QFSConst::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QFSConst::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QFSConst))
        return static_cast<void*>(const_cast< QFSConst*>(this));
    return QDialog::qt_metacast(_clname);
}

int QFSConst::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_QForgingSystemDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      50,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      77,   18,   18,   18, 0x0a,
      94,   18,   18,   18, 0x0a,
     110,   18,   18,   18, 0x0a,
     125,   18,   18,   18, 0x0a,
     140,   18,   18,   18, 0x0a,
     155,   18,   18,   18, 0x0a,
     193,  177,   18,   18, 0x0a,
     222,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QForgingSystemDlg[] = {
    "QForgingSystemDlg\0\0emitPsetHighLight(NElSurfChsS)\0"
    "emitForgingsystemNext(int)\0ApplySABtnSlot()\0"
    "NextSABtnSlot()\0PreSABtnSlot()\0"
    "AddSABtnSlot()\0DelSABtnSlot()\0"
    "MotionParameterSlot()\0comWigId,index0\0"
    "ComboxSlotHighlight(int,int)\0"
    "ComboxSlotHighlight2(NElSurfChsS)\0"
};

void QForgingSystemDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QForgingSystemDlg *_t = static_cast<QForgingSystemDlg *>(_o);
        switch (_id) {
        case 0: _t->emitPsetHighLight((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        case 1: _t->emitForgingsystemNext((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ApplySABtnSlot(); break;
        case 3: _t->NextSABtnSlot(); break;
        case 4: _t->PreSABtnSlot(); break;
        case 5: _t->AddSABtnSlot(); break;
        case 6: _t->DelSABtnSlot(); break;
        case 7: _t->MotionParameterSlot(); break;
        case 8: _t->ComboxSlotHighlight((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->ComboxSlotHighlight2((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QForgingSystemDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QForgingSystemDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QForgingSystemDlg,
      qt_meta_data_QForgingSystemDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QForgingSystemDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QForgingSystemDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QForgingSystemDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QForgingSystemDlg))
        return static_cast<void*>(const_cast< QForgingSystemDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QForgingSystemDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void QForgingSystemDlg::emitPsetHighLight(NElSurfChsS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QForgingSystemDlg::emitForgingsystemNext(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
