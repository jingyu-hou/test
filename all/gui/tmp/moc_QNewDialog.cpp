/****************************************************************************
** Meta object code from reading C++ file 'QNewDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/HPprocess/QNewDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QNewDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTCurveDataPlot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      37,   16,   16,   16, 0x0a,
      55,   16,   16,   16, 0x0a,
      76,   16,   16,   16, 0x0a,
     107,   94,   16,   16, 0x0a,
     134,  132,   16,   16, 0x0a,
     163,   16,   16,   16, 0x0a,
     182,   16,   16,   16, 0x0a,
     202,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTCurveDataPlot[] = {
    "QTCurveDataPlot\0\0CurveApplyBtnSlot()\0"
    "CurveAddBtnSlot()\0CurveInsertBtnSlot()\0"
    "CurveDelBtnSlot()\0nRow,strData\0"
    "SetData(int,QStringList)\0p\0"
    "contextTableMenuSlot(QPoint)\0"
    "TableCopyBtnSlot()\0TablePasteBtnSlot()\0"
    "TableClearRBtnSlot()\0"
};

void QTCurveDataPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTCurveDataPlot *_t = static_cast<QTCurveDataPlot *>(_o);
        switch (_id) {
        case 0: _t->CurveApplyBtnSlot(); break;
        case 1: _t->CurveAddBtnSlot(); break;
        case 2: _t->CurveInsertBtnSlot(); break;
        case 3: _t->CurveDelBtnSlot(); break;
        case 4: _t->SetData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 5: _t->contextTableMenuSlot((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 6: _t->TableCopyBtnSlot(); break;
        case 7: _t->TablePasteBtnSlot(); break;
        case 8: _t->TableClearRBtnSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTCurveDataPlot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTCurveDataPlot::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QTCurveDataPlot,
      qt_meta_data_QTCurveDataPlot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTCurveDataPlot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTCurveDataPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTCurveDataPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTCurveDataPlot))
        return static_cast<void*>(const_cast< QTCurveDataPlot*>(this));
    return QWidget::qt_metacast(_clname);
}

int QTCurveDataPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
static const uint qt_meta_data_QNewDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_QNewDialog[] = {
    "QNewDialog\0"
};

void QNewDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QNewDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QNewDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QNewDialog,
      qt_meta_data_QNewDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QNewDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QNewDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QNewDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QNewDialog))
        return static_cast<void*>(const_cast< QNewDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QNewDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_QAddTab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_QAddTab[] = {
    "QAddTab\0"
};

void QAddTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QAddTab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QAddTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QAddTab,
      qt_meta_data_QAddTab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QAddTab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QAddTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QAddTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QAddTab))
        return static_cast<void*>(const_cast< QAddTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int QAddTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_QChangePointDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      32,   19,   19,   19, 0x08,
      48,   19,   19,   19, 0x08,
      62,   19,   19,   19, 0x08,
      78,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QChangePointDialog[] = {
    "QChangePointDialog\0\0OkBtnSlot()\0"
    "AddTabBtnSlot()\0PlotBtnSlot()\0"
    "DelTabBtnSlot()\0AutoRenameSlot()\0"
};

void QChangePointDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QChangePointDialog *_t = static_cast<QChangePointDialog *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        case 1: _t->AddTabBtnSlot(); break;
        case 2: _t->PlotBtnSlot(); break;
        case 3: _t->DelTabBtnSlot(); break;
        case 4: _t->AutoRenameSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QChangePointDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QChangePointDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QChangePointDialog,
      qt_meta_data_QChangePointDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QChangePointDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QChangePointDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QChangePointDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QChangePointDialog))
        return static_cast<void*>(const_cast< QChangePointDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QChangePointDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_QPhaseBalanceDialog[] = {

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
      21,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPhaseBalanceDialog[] = {
    "QPhaseBalanceDialog\0\0OkBtnSlot()\0"
};

void QPhaseBalanceDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPhaseBalanceDialog *_t = static_cast<QPhaseBalanceDialog *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QPhaseBalanceDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPhaseBalanceDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QPhaseBalanceDialog,
      qt_meta_data_QPhaseBalanceDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPhaseBalanceDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPhaseBalanceDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPhaseBalanceDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPhaseBalanceDialog))
        return static_cast<void*>(const_cast< QPhaseBalanceDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QPhaseBalanceDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_QPhaseLatentHeatDialog[] = {

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
      24,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPhaseLatentHeatDialog[] = {
    "QPhaseLatentHeatDialog\0\0OkBtnSlot()\0"
};

void QPhaseLatentHeatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPhaseLatentHeatDialog *_t = static_cast<QPhaseLatentHeatDialog *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QPhaseLatentHeatDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPhaseLatentHeatDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QPhaseLatentHeatDialog,
      qt_meta_data_QPhaseLatentHeatDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPhaseLatentHeatDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPhaseLatentHeatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPhaseLatentHeatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPhaseLatentHeatDialog))
        return static_cast<void*>(const_cast< QPhaseLatentHeatDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QPhaseLatentHeatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_QPhasePropDialog[] = {

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
      18,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPhasePropDialog[] = {
    "QPhasePropDialog\0\0OkBtnSlot()\0"
};

void QPhasePropDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPhasePropDialog *_t = static_cast<QPhasePropDialog *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QPhasePropDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPhasePropDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QPhasePropDialog,
      qt_meta_data_QPhasePropDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPhasePropDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPhasePropDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPhasePropDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPhasePropDialog))
        return static_cast<void*>(const_cast< QPhasePropDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QPhasePropDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_QIncubationPeriod[] = {

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
      19,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QIncubationPeriod[] = {
    "QIncubationPeriod\0\0OkBtnSlot()\0"
};

void QIncubationPeriod::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QIncubationPeriod *_t = static_cast<QIncubationPeriod *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QIncubationPeriod::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QIncubationPeriod::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QIncubationPeriod,
      qt_meta_data_QIncubationPeriod, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QIncubationPeriod::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QIncubationPeriod::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QIncubationPeriod::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QIncubationPeriod))
        return static_cast<void*>(const_cast< QIncubationPeriod*>(this));
    return QDialog::qt_metacast(_clname);
}

int QIncubationPeriod::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_QPhaseZBFDialog[] = {

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
      17,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPhaseZBFDialog[] = {
    "QPhaseZBFDialog\0\0OkBtnSlot()\0"
};

void QPhaseZBFDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPhaseZBFDialog *_t = static_cast<QPhaseZBFDialog *>(_o);
        switch (_id) {
        case 0: _t->OkBtnSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QPhaseZBFDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPhaseZBFDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QPhaseZBFDialog,
      qt_meta_data_QPhaseZBFDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPhaseZBFDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPhaseZBFDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPhaseZBFDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPhaseZBFDialog))
        return static_cast<void*>(const_cast< QPhaseZBFDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QPhaseZBFDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
