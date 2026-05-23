/****************************************************************************
** Meta object code from reading C++ file 'QPostWigFile.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QPostWigFile.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QPostWigFile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QPostWigFile[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      60,   13,   13,   13, 0x05,
      83,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     101,   13,   13,   13, 0x08,
     118,   13,   13,   13, 0x08,
     134,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPostWigFile[] = {
    "QPostWigFile\0\0"
    "emitDataMenu(const map<QString,QStringList>*)\0"
    "frdDataOk(FrdDataVIS*)\0DatOk(resultDatS)\0"
    "BtnOpenFrdSlot()\0BtnOutPutSlot()\0"
    "SwitchActorPickSlot(bool)\0"
};

void QPostWigFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPostWigFile *_t = static_cast<QPostWigFile *>(_o);
        switch (_id) {
        case 0: _t->emitDataMenu((*reinterpret_cast< const map<QString,QStringList>*(*)>(_a[1]))); break;
        case 1: _t->frdDataOk((*reinterpret_cast< FrdDataVIS*(*)>(_a[1]))); break;
        case 2: _t->DatOk((*reinterpret_cast< resultDatS(*)>(_a[1]))); break;
        case 3: _t->BtnOpenFrdSlot(); break;
        case 4: _t->BtnOutPutSlot(); break;
        case 5: _t->SwitchActorPickSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QPostWigFile::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPostWigFile::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPostWigFile,
      qt_meta_data_QPostWigFile, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPostWigFile::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPostWigFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPostWigFile::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPostWigFile))
        return static_cast<void*>(const_cast< QPostWigFile*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPostWigFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QPostWigFile::emitDataMenu(const map<QString,QStringList> * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QPostWigFile::frdDataOk(FrdDataVIS * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QPostWigFile::DatOk(resultDatS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
