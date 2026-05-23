/****************************************************************************
** Meta object code from reading C++ file 'QPostWigResultOutput.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QPostWigResultOutput.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QPostWigResultOutput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CutParamWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   16,   15,   15, 0x05,
      59,   16,   15,   15, 0x05,
     106,   94,   15,   15, 0x05,
     149,   94,   15,   15, 0x05,
     212,  194,   15,   15, 0x05,
     249,   16,   15,   15, 0x05,
     293,  280,   15,   15, 0x05,
     344,  332,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     383,  376,   15,   15, 0x08,
     408,  376,   15,   15, 0x08,
     438,   15,   15,   15, 0x08,
     459,   15,   15,   15, 0x08,
     484,  478,   15,   15, 0x08,
     497,   15,   15,   15, 0x08,
     517,   15,   15,   15, 0x08,
     546,  541,   15,   15, 0x08,
     579,  572,   15,   15, 0x08,
     619,  613,   15,   15, 0x08,
     646,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CutParamWidget[] = {
    "CutParamWidget\0\0cutId,isShow\0"
    "CutZoneStateChanged(int,bool)\0"
    "CutVTKWidgetStateChanged(int,bool)\0"
    "cutId,x,y,z\0CutNormalChanged(int,double,double,double)\0"
    "CutPositionChanged(int,double,double,double)\0"
    "cutId,isShow,type\0"
    "CutContourStateChanged(int,bool,int)\0"
    "CutValueStateChanged(int,bool)\0"
    "cutId,scalar\0CutContourVariableChanged(int,QString)\0"
    "cutId,level\0CutContourLevelChanged(int,int)\0"
    "isShow\0ChangeCutZoneState(bool)\0"
    "ChangeCutVTKWidgetState(bool)\0"
    "ChangeCutNormal(int)\0ChangeCutNormal2()\0"
    "value\0MoveCut(int)\0ChangeCutPosition()\0"
    "ChangeCutContourState()\0type\0"
    "ChangeCutContourType(int)\0scalar\0"
    "ChangeCutContourVariable(QString)\0"
    "level\0ChangeCutContourLevel(int)\0"
    "ChkSectionContourSlot()\0"
};

void CutParamWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CutParamWidget *_t = static_cast<CutParamWidget *>(_o);
        switch (_id) {
        case 0: _t->CutZoneStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->CutVTKWidgetStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->CutNormalChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 3: _t->CutPositionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 4: _t->CutContourStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->CutValueStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: _t->CutContourVariableChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->CutContourLevelChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->ChangeCutZoneState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->ChangeCutVTKWidgetState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->ChangeCutNormal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->ChangeCutNormal2(); break;
        case 12: _t->MoveCut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->ChangeCutPosition(); break;
        case 14: _t->ChangeCutContourState(); break;
        case 15: _t->ChangeCutContourType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->ChangeCutContourVariable((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->ChangeCutContourLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->ChkSectionContourSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CutParamWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CutParamWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CutParamWidget,
      qt_meta_data_CutParamWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CutParamWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CutParamWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CutParamWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CutParamWidget))
        return static_cast<void*>(const_cast< CutParamWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CutParamWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void CutParamWidget::CutZoneStateChanged(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CutParamWidget::CutVTKWidgetStateChanged(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CutParamWidget::CutNormalChanged(int _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CutParamWidget::CutPositionChanged(int _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CutParamWidget::CutContourStateChanged(int _t1, bool _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CutParamWidget::CutValueStateChanged(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CutParamWidget::CutContourVariableChanged(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CutParamWidget::CutContourLevelChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
static const uint qt_meta_data_QPostWigResultOutput[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,   55,   21,   21, 0x0a,
      95,   21,   21,   21, 0x08,
     109,   21,   21,   21, 0x08,
     128,   21,   21,   21, 0x08,
     150,  145,   21,   21, 0x08,
     175,  170,   21,   21, 0x08,
     196,   21,   21,   21, 0x08,
     213,   21,   21,   21, 0x08,
     227,   21,   21,   21, 0x08,
     246,   21,   21,   21, 0x08,
     266,   21,   21,   21, 0x08,
     298,  286,   21,   21, 0x08,
     344,  286,   21,   21, 0x08,
     397,  385,   21,   21, 0x08,
     426,  385,   21,   21, 0x08,
     469,  452,   21,   21, 0x08,
     511,  499,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPostWigResultOutput[] = {
    "QPostWigResultOutput\0\0ResultVis\0"
    "emitScalar(ResultVisS)\0frdObj\0"
    "InitSectionDataSlot(FrdDataVIS*)\0"
    "BtnEquiSlot()\0BtnNephogramSlot()\0"
    "BtnSectionSlot()\0iVal\0SpinBoxClrSlot(int)\0"
    "dVal\0SliderTrancSlot(int)\0SetChgShowSlot()\0"
    "ChkGridSlot()\0gridColorBtnSlot()\0"
    "BtnSectionAddSlot()\0BtnSectionDelSlot()\0"
    "cutId,x,y,z\0SectionNormalXYSlot(int,double,double,double)\0"
    "SectionPosSlot(int,double,double,double)\0"
    "cutId,bShow\0SectionOutLineSlot(int,bool)\0"
    "SectionZoneSlot(int,bool)\0cutId,bShow,type\0"
    "SectionShowSlot(int,bool,int)\0cutId,level\0"
    "SectionLevelSlot(int,int)\0"
};

void QPostWigResultOutput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPostWigResultOutput *_t = static_cast<QPostWigResultOutput *>(_o);
        switch (_id) {
        case 0: _t->emitScalar((*reinterpret_cast< ResultVisS(*)>(_a[1]))); break;
        case 1: _t->InitSectionDataSlot((*reinterpret_cast< FrdDataVIS*(*)>(_a[1]))); break;
        case 2: _t->BtnEquiSlot(); break;
        case 3: _t->BtnNephogramSlot(); break;
        case 4: _t->BtnSectionSlot(); break;
        case 5: _t->SpinBoxClrSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->SliderTrancSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->SetChgShowSlot(); break;
        case 8: _t->ChkGridSlot(); break;
        case 9: _t->gridColorBtnSlot(); break;
        case 10: _t->BtnSectionAddSlot(); break;
        case 11: _t->BtnSectionDelSlot(); break;
        case 12: _t->SectionNormalXYSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 13: _t->SectionPosSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 14: _t->SectionOutLineSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 15: _t->SectionZoneSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 16: _t->SectionShowSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 17: _t->SectionLevelSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QPostWigResultOutput::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPostWigResultOutput::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPostWigResultOutput,
      qt_meta_data_QPostWigResultOutput, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPostWigResultOutput::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPostWigResultOutput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPostWigResultOutput::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPostWigResultOutput))
        return static_cast<void*>(const_cast< QPostWigResultOutput*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPostWigResultOutput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void QPostWigResultOutput::emitScalar(ResultVisS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
