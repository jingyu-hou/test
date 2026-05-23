/****************************************************************************
** Meta object code from reading C++ file 'QMaterNewDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QMaterNewDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMaterNewDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTableCurveDataPlot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      38,   20,   20,   20, 0x0a,
      55,   20,   20,   20, 0x0a,
      75,   20,   20,   20, 0x0a,
      91,   89,   20,   20, 0x0a,
     120,   20,   20,   20, 0x0a,
     140,   20,   20,   20, 0x0a,
     161,   20,   20,   20, 0x0a,
     182,   20,   20,   20, 0x0a,
     207,   20,   20,   20, 0x0a,
     225,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTableCurveDataPlot[] = {
    "QTableCurveDataPlot\0\0AddLineBtnSlot()\0"
    "DelLineBtnSlot()\0InsertLineBtnSlot()\0"
    "PlotBtnSlot()\0p\0contextTableMenuSlot(QPoint)\0"
    "TableCopyRBtnSlot()\0TablePasteRBtnSlot()\0"
    "TableClearRBtnSlot()\0RateDependentSlot01(int)\0"
    "DamageSlot01(int)\0PlasticWorkValueSlot()\0"
};

void QTableCurveDataPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTableCurveDataPlot *_t = static_cast<QTableCurveDataPlot *>(_o);
        switch (_id) {
        case 0: _t->AddLineBtnSlot(); break;
        case 1: _t->DelLineBtnSlot(); break;
        case 2: _t->InsertLineBtnSlot(); break;
        case 3: _t->PlotBtnSlot(); break;
        case 4: _t->contextTableMenuSlot((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->TableCopyRBtnSlot(); break;
        case 6: _t->TablePasteRBtnSlot(); break;
        case 7: _t->TableClearRBtnSlot(); break;
        case 8: _t->RateDependentSlot01((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->DamageSlot01((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->PlasticWorkValueSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTableCurveDataPlot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTableCurveDataPlot::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QTableCurveDataPlot,
      qt_meta_data_QTableCurveDataPlot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTableCurveDataPlot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTableCurveDataPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTableCurveDataPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTableCurveDataPlot))
        return static_cast<void*>(const_cast< QTableCurveDataPlot*>(this));
    return QWidget::qt_metacast(_clname);
}

int QTableCurveDataPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
static const uint qt_meta_data_QMaterNewDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      28,   13,   13,   13, 0x0a,
      42,   13,   13,   13, 0x0a,
      56,   13,   13,   13, 0x0a,
      76,   13,   13,   13, 0x0a,
      89,   13,   13,   13, 0x0a,
     120,   13,   13,   13, 0x0a,
     136,   13,   13,   13, 0x0a,
     150,   13,   13,   13, 0x0a,
     165,   13,   13,   13, 0x0a,
     183,   13,   13,   13, 0x0a,
     193,   13,   13,   13, 0x0a,
     212,   13,   13,   13, 0x0a,
     224,   13,   13,   13, 0x0a,
     240,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QMaterNewDlg[] = {
    "QMaterNewDlg\0\0DensitySlot()\0ElasticSlot()\0"
    "PlasticSlot()\0RateDependentSlot()\0"
    "DamageSlot()\0DynamicRecrystallizationSlot()\0"
    "ExpansionSlot()\0ConductSlot()\0"
    "SpecificSlot()\0MatelPowderSlot()\0"
    "TTTSlot()\0MaterListSlot(int)\0OkBtnSlot()\0"
    "CancelBtnSlot()\0DelMaterialSlot()\0"
};

void QMaterNewDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QMaterNewDlg *_t = static_cast<QMaterNewDlg *>(_o);
        switch (_id) {
        case 0: _t->DensitySlot(); break;
        case 1: _t->ElasticSlot(); break;
        case 2: _t->PlasticSlot(); break;
        case 3: _t->RateDependentSlot(); break;
        case 4: _t->DamageSlot(); break;
        case 5: _t->DynamicRecrystallizationSlot(); break;
        case 6: _t->ExpansionSlot(); break;
        case 7: _t->ConductSlot(); break;
        case 8: _t->SpecificSlot(); break;
        case 9: _t->MatelPowderSlot(); break;
        case 10: _t->TTTSlot(); break;
        case 11: _t->MaterListSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->OkBtnSlot(); break;
        case 13: _t->CancelBtnSlot(); break;
        case 14: _t->DelMaterialSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QMaterNewDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QMaterNewDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QMaterNewDlg,
      qt_meta_data_QMaterNewDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMaterNewDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMaterNewDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMaterNewDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMaterNewDlg))
        return static_cast<void*>(const_cast< QMaterNewDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QMaterNewDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
