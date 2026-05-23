/****************************************************************************
** Meta object code from reading C++ file 'QHPPartDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/HPprocess/QHPPartDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QHPPartDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHPPartDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      44,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      56,   11,   11,   11, 0x08,
      70,   11,   11,   11, 0x08,
      87,   11,   11,   11, 0x08,
     104,   11,   11,   11, 0x08,
     118,   11,   11,   11, 0x08,
     135,   11,   11,   11, 0x08,
     154,   11,   11,   11, 0x08,
     168,   11,   11,   11, 0x08,
     184,  180,   11,   11, 0x08,
     231,  215,   11,   11, 0x08,
     274,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QHPPartDlg[] = {
    "QHPPartDlg\0\0emitElsetHighLight(NElSurfChsS)\0"
    "emitMerge()\0SHPPropSlot()\0DeletePropSlot()\0"
    "ApplayPropSlot()\0NextBtnSlot()\0"
    "comBoxMChgSlot()\0comBoxDoubleSlot()\0"
    "ComboElSlot()\0MergeSlot()\0str\0"
    "AppendElSetComboxSlot(QString)\0"
    "str,iStyle,data\0"
    "AppendPElData(QString,int,DecodeGenerateS)\0"
    "AMOptions(bool)\0"
};

void QHPPartDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QHPPartDlg *_t = static_cast<QHPPartDlg *>(_o);
        switch (_id) {
        case 0: _t->emitElsetHighLight((*reinterpret_cast< NElSurfChsS(*)>(_a[1]))); break;
        case 1: _t->emitMerge(); break;
        case 2: _t->SHPPropSlot(); break;
        case 3: _t->DeletePropSlot(); break;
        case 4: _t->ApplayPropSlot(); break;
        case 5: _t->NextBtnSlot(); break;
        case 6: _t->comBoxMChgSlot(); break;
        case 7: _t->comBoxDoubleSlot(); break;
        case 8: _t->ComboElSlot(); break;
        case 9: _t->MergeSlot(); break;
        case 10: _t->AppendElSetComboxSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->AppendPElData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< DecodeGenerateS(*)>(_a[3]))); break;
        case 12: _t->AMOptions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QHPPartDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QHPPartDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QHPPartDlg,
      qt_meta_data_QHPPartDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHPPartDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHPPartDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHPPartDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHPPartDlg))
        return static_cast<void*>(const_cast< QHPPartDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QHPPartDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void QHPPartDlg::emitElsetHighLight(NElSurfChsS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QHPPartDlg::emitMerge()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
