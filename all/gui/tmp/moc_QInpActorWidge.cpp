/****************************************************************************
** Meta object code from reading C++ file 'QInpActorWidge.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QInpActorWidge.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QInpActorWidge.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QInpActorWidge[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   15,   15,   15, 0x08,
      68,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QInpActorWidge[] = {
    "QInpActorWidge\0\0emitCheckActor(QStringList)\0"
    "checkBoxSelectAllSlot()\0BtnOK()\0"
};

void QInpActorWidge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QInpActorWidge *_t = static_cast<QInpActorWidge *>(_o);
        switch (_id) {
        case 0: _t->emitCheckActor((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 1: _t->checkBoxSelectAllSlot(); break;
        case 2: _t->BtnOK(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QInpActorWidge::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QInpActorWidge::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QInpActorWidge,
      qt_meta_data_QInpActorWidge, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QInpActorWidge::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QInpActorWidge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QInpActorWidge::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QInpActorWidge))
        return static_cast<void*>(const_cast< QInpActorWidge*>(this));
    return QDialog::qt_metacast(_clname);
}

int QInpActorWidge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QInpActorWidge::emitCheckActor(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
