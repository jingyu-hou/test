/****************************************************************************
** Meta object code from reading C++ file 'XYPlot_Panel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/PostProcess/XYPlot_Panel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'XYPlot_Panel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_XYPlot_Panel[] = {

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
      21,   14,   13,   13, 0x0a,
      54,   47,   13,   13, 0x0a,
      82,   13,   13,   13, 0x08,
     104,   13,   13,   13, 0x08,
     125,  119,   13,   13, 0x08,
     150,  119,   13,   13, 0x08,
     175,   13,   13,   13, 0x08,
     190,   13,   13,   13, 0x08,
     208,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_XYPlot_Panel[] = {
    "XYPlot_Panel\0\0frdObj\0InitPlotData(FrdDataVIS*)\0"
    "DatObj\0InitPlotDatData(resultDatS)\0"
    "SwitchPointPick(bool)\0CreateXYPlot()\0"
    "curId\0ChangeCombStyleSlot(int)\0"
    "ChangeCombCurveSlot(int)\0SaveDataSlot()\0"
    "PointClrBtnSlot()\0PointApplyBtnSlot()\0"
};

void XYPlot_Panel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        XYPlot_Panel *_t = static_cast<XYPlot_Panel *>(_o);
        switch (_id) {
        case 0: _t->InitPlotData((*reinterpret_cast< FrdDataVIS*(*)>(_a[1]))); break;
        case 1: _t->InitPlotDatData((*reinterpret_cast< resultDatS(*)>(_a[1]))); break;
        case 2: _t->SwitchPointPick((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->CreateXYPlot(); break;
        case 4: _t->ChangeCombStyleSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ChangeCombCurveSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->SaveDataSlot(); break;
        case 7: _t->PointClrBtnSlot(); break;
        case 8: _t->PointApplyBtnSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData XYPlot_Panel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject XYPlot_Panel::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_XYPlot_Panel,
      qt_meta_data_XYPlot_Panel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &XYPlot_Panel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *XYPlot_Panel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *XYPlot_Panel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_XYPlot_Panel))
        return static_cast<void*>(const_cast< XYPlot_Panel*>(this));
    return QDialog::qt_metacast(_clname);
}

int XYPlot_Panel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
