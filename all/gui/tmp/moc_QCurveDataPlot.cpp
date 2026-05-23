/****************************************************************************
** Meta object code from reading C++ file 'QCurveDataPlot.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QCurveDataPlot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QCurveDataPlot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QCurveDataPlot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      36,   15,   15,   15, 0x0a,
      53,   15,   15,   15, 0x0a,
      73,   15,   15,   15, 0x0a,
      91,   15,   15,   15, 0x0a,
     112,   15,   15,   15, 0x0a,
     132,  130,   15,   15, 0x0a,
     161,   15,   15,   15, 0x0a,
     180,   15,   15,   15, 0x0a,
     200,   15,   15,   15, 0x0a,
     221,   15,   15,   15, 0x0a,
     234,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QCurveDataPlot[] = {
    "QCurveDataPlot\0\0CurveApplyBtnSlot()\0"
    "CurveOkBtnSlot()\0CurveCanleBtnSlot()\0"
    "CurveAddBtnSlot()\0CurveInsertBtnSlot()\0"
    "CurveDelBtnSlot()\0p\0contextTableMenuSlot(QPoint)\0"
    "TableCopyBtnSlot()\0TablePasteBtnSlot()\0"
    "TableClearRBtnSlot()\0ImportSlot()\0"
    "ExportSlot()\0"
};

void QCurveDataPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QCurveDataPlot *_t = static_cast<QCurveDataPlot *>(_o);
        switch (_id) {
        case 0: _t->CurveApplyBtnSlot(); break;
        case 1: _t->CurveOkBtnSlot(); break;
        case 2: _t->CurveCanleBtnSlot(); break;
        case 3: _t->CurveAddBtnSlot(); break;
        case 4: _t->CurveInsertBtnSlot(); break;
        case 5: _t->CurveDelBtnSlot(); break;
        case 6: _t->contextTableMenuSlot((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: _t->TableCopyBtnSlot(); break;
        case 8: _t->TablePasteBtnSlot(); break;
        case 9: _t->TableClearRBtnSlot(); break;
        case 10: _t->ImportSlot(); break;
        case 11: _t->ExportSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QCurveDataPlot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QCurveDataPlot::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QCurveDataPlot,
      qt_meta_data_QCurveDataPlot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QCurveDataPlot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QCurveDataPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QCurveDataPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QCurveDataPlot))
        return static_cast<void*>(const_cast< QCurveDataPlot*>(this));
    return QDialog::qt_metacast(_clname);
}

int QCurveDataPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
