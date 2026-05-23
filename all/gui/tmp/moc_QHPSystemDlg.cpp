/****************************************************************************
** Meta object code from reading C++ file 'QHPSystemDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/HPprocess/QHPSystemDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QHPSystemDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHPSystemDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      45,   13,   13,   13, 0x05,
      69,   67,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     107,   13,   13,   13, 0x08,
     122,   13,   13,   13, 0x08,
     137,   13,   13,   13, 0x08,
     153,   13,   13,   13, 0x08,
     166,   13,   13,   13, 0x08,
     181,   13,   13,   13, 0x08,
     199,   13,   13,   13, 0x08,
     215,  213,   13,   13, 0x08,
     249,   13,   13,   13, 0x08,
     268,   67,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QHPSystemDlg[] = {
    "QHPSystemDlg\0\0emitPsetHighLight(NElSurfChsS)\0"
    "emitHPsystemNext(int)\0,\0"
    "emitHPCurve(QStringList,InpCurveInpS)\0"
    "AddBtnHPSlot()\0DelBtnHPSlot()\0"
    "NextBtnHPSlot()\0comboxSlot()\0"
    "PreBtnHPSlot()\0InsertBtnHPSlot()\0"
    "DelRBtnSlot()\0p\0contextListWidgetMenuSlot(QPoint)\0"
    "comboxChangeSlot()\0"
    "updateHPCurveDataSlot(QStringList,InpCurveInpS)\0"
};

void QHPSystemDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QHPSystemDlg *_t = static_cast<QHPSystemDlg *>(_o);
        switch (_id) {
        case 0: _t->emitPsetHighLight((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        case 1: _t->emitHPsystemNext((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->emitHPCurve((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< InpCurveInpS(*)>(_a[2]))); break;
        case 3: _t->AddBtnHPSlot(); break;
        case 4: _t->DelBtnHPSlot(); break;
        case 5: _t->NextBtnHPSlot(); break;
        case 6: _t->comboxSlot(); break;
        case 7: _t->PreBtnHPSlot(); break;
        case 8: _t->InsertBtnHPSlot(); break;
        case 9: _t->DelRBtnSlot(); break;
        case 10: _t->contextListWidgetMenuSlot((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 11: _t->comboxChangeSlot(); break;
        case 12: _t->updateHPCurveDataSlot((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< InpCurveInpS(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QHPSystemDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QHPSystemDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QHPSystemDlg,
      qt_meta_data_QHPSystemDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHPSystemDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHPSystemDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHPSystemDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHPSystemDlg))
        return static_cast<void*>(const_cast< QHPSystemDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QHPSystemDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void QHPSystemDlg::emitPsetHighLight(NElSurfChsS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QHPSystemDlg::emitHPsystemNext(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QHPSystemDlg::emitHPCurve(QStringList _t1, InpCurveInpS _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_QTemperateDialog[] = {

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
      18,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTemperateDialog[] = {
    "QTemperateDialog\0\0OkBtnSlot()\0"
};

void QTemperateDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTemperateDialog *_t = static_cast<QTemperateDialog *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QTemperateDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTemperateDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QTemperateDialog,
      qt_meta_data_QTemperateDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTemperateDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTemperateDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTemperateDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTemperateDialog))
        return static_cast<void*>(const_cast< QTemperateDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QTemperateDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_QFilmDialog[] = {

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

static const char qt_meta_stringdata_QFilmDialog[] = {
    "QFilmDialog\0\0OkBtnSlot()\0"
};

void QFilmDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QFilmDialog *_t = static_cast<QFilmDialog *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QFilmDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QFilmDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QFilmDialog,
      qt_meta_data_QFilmDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QFilmDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QFilmDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QFilmDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QFilmDialog))
        return static_cast<void*>(const_cast< QFilmDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QFilmDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_QRadiateDialog[] = {

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
      16,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QRadiateDialog[] = {
    "QRadiateDialog\0\0OkBtnSlot()\0"
};

void QRadiateDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QRadiateDialog *_t = static_cast<QRadiateDialog *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QRadiateDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QRadiateDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QRadiateDialog,
      qt_meta_data_QRadiateDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QRadiateDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QRadiateDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QRadiateDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRadiateDialog))
        return static_cast<void*>(const_cast< QRadiateDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QRadiateDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
