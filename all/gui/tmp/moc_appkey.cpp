/****************************************************************************
** Meta object code from reading C++ file 'appkey.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/appkey.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'appkey.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppKey[] = {

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
       8,    7,    7,    7, 0x08,
      37,   28,   20,    7, 0x08,
      72,    7,    7,    7, 0x0a,
      80,    7,    7,    7, 0x0a,
      98,   92,   87,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppKey[] = {
    "AppKey\0\0checkTime()\0QString\0data,key\0"
    "getXorEncryptDecrypt(QString,char)\0"
    "start()\0stop()\0bool\0count\0checkCount(int)\0"
};

void AppKey::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppKey *_t = static_cast<AppKey *>(_o);
        switch (_id) {
        case 0: _t->checkTime(); break;
        case 1: { QString _r = _t->getXorEncryptDecrypt((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: _t->start(); break;
        case 3: _t->stop(); break;
        case 4: { bool _r = _t->checkCount((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppKey::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppKey::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppKey,
      qt_meta_data_AppKey, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppKey::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppKey::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppKey::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppKey))
        return static_cast<void*>(const_cast< AppKey*>(this));
    return QObject::qt_metacast(_clname);
}

int AppKey::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
