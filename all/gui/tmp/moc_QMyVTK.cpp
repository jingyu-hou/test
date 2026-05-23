/****************************************************************************
** Meta object code from reading C++ file 'QMyVTK.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QMyVTK.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMyVTK.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dialog_BG[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      27,   10,   10,   10, 0x08,
      41,   10,   10,   10, 0x08,
      52,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Dialog_BG[] = {
    "Dialog_BG\0\0BGModeChanged()\0ChooseColor()\0"
    "UpdateBG()\0OKSlot()\0"
};

void Dialog_BG::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dialog_BG *_t = static_cast<Dialog_BG *>(_o);
        switch (_id) {
        case 0: _t->BGModeChanged(); break;
        case 1: _t->ChooseColor(); break;
        case 2: _t->UpdateBG(); break;
        case 3: _t->OKSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Dialog_BG::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dialog_BG::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog_BG,
      qt_meta_data_Dialog_BG, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dialog_BG::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dialog_BG::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dialog_BG::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog_BG))
        return static_cast<void*>(const_cast< Dialog_BG*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dialog_BG::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
