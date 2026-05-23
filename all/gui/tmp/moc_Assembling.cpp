/****************************************************************************
** Meta object code from reading C++ file 'Assembling.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Assembling.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Assembling.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Assembling[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      43,   11,   11,   11, 0x05,
      75,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     107,   11,   11,   11, 0x08,
     126,   11,   11,   11, 0x0a,
     138,   11,   11,   11, 0x0a,
     151,   11,   11,   11, 0x0a,
     167,   11,   11,   11, 0x0a,
     191,  184,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Assembling[] = {
    "Assembling\0\0emitPsetHighLight(NElSurfChsS)\0"
    "emitPsetHighLight1(NElSurfChsS)\0"
    "emitAssemblingS(AssemblingS_ZP)\0"
    "PickPointSlot(int)\0ApplySlot()\0"
    "CancelSlot()\0HighLightSlot()\0"
    "ActivationSlot()\0InpObj\0"
    "InitInpDataSlot1(InpDataVIS*)\0"
};

void Assembling::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Assembling *_t = static_cast<Assembling *>(_o);
        switch (_id) {
        case 0: _t->emitPsetHighLight((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        case 1: _t->emitPsetHighLight1((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        case 2: _t->emitAssemblingS((*reinterpret_cast< AssemblingS_ZP(*)>(_a[1]))); break;
        case 3: _t->PickPointSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ApplySlot(); break;
        case 5: _t->CancelSlot(); break;
        case 6: _t->HighLightSlot(); break;
        case 7: _t->ActivationSlot(); break;
        case 8: _t->InitInpDataSlot1((*reinterpret_cast< InpDataVIS*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Assembling::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Assembling::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Assembling,
      qt_meta_data_Assembling, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Assembling::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Assembling::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Assembling::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Assembling))
        return static_cast<void*>(const_cast< Assembling*>(this));
    return QDialog::qt_metacast(_clname);
}

int Assembling::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void Assembling::emitPsetHighLight(NElSurfChsS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Assembling::emitPsetHighLight1(NElSurfChsS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Assembling::emitAssemblingS(AssemblingS_ZP _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
