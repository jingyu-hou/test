/****************************************************************************
** Meta object code from reading C++ file 'QSectionPropDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QSectionPropDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QSectionPropDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QSectionPropDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   16,   16,   16, 0x0a,
      56,   16,   16,   16, 0x0a,
      76,   72,   16,   16, 0x0a,
      98,   72,   16,   16, 0x0a,
     126,  120,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QSectionPropDlg[] = {
    "QSectionPropDlg\0\0emitSectionParam(SectionS)\0"
    "BtnOkSlot()\0BtnCancelSlot()\0Row\0"
    "SectionListSlot1(int)\0SectionListSlot2(int)\0"
    "Index\0SetCategorySlot(int)\0"
};

void QSectionPropDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QSectionPropDlg *_t = static_cast<QSectionPropDlg *>(_o);
        switch (_id) {
        case 0: _t->emitSectionParam((*reinterpret_cast< SectionS(*)>(_a[1]))); break;
        case 1: _t->BtnOkSlot(); break;
        case 2: _t->BtnCancelSlot(); break;
        case 3: _t->SectionListSlot1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->SectionListSlot2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->SetCategorySlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QSectionPropDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QSectionPropDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QSectionPropDlg,
      qt_meta_data_QSectionPropDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QSectionPropDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QSectionPropDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QSectionPropDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QSectionPropDlg))
        return static_cast<void*>(const_cast< QSectionPropDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QSectionPropDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void QSectionPropDlg::emitSectionParam(SectionS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
