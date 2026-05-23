/****************************************************************************
** Meta object code from reading C++ file 'QMaterPropDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QMaterPropDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMaterPropDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMaterPropDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   14,   14,   14, 0x0a,
      56,   14,   14,   14, 0x0a,
      72,   14,   14,   14, 0x0a,
      92,   14,   14,   14, 0x0a,
     112,   14,   14,   14, 0x0a,
     131,  127,   14,   14, 0x0a,
     150,   14,   14,   14, 0x0a,
     194,  192,   14,   14, 0x0a,
     223,   14,   14,   14, 0x0a,
     247,  241,   14,   14, 0x0a,
     289,  281,   14,   14, 0x0a,
     311,   14,   14,   14, 0x0a,
     330,   14,   14,   14, 0x0a,
     385,  373,  349,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QMaterPropDlg[] = {
    "QMaterPropDlg\0\0emitMaterParam(SectionS)\0"
    "BtnApplaySlot()\0BtnCancelSlot()\0"
    "BtnImportDataSlot()\0BtnExportDataSlot()\0"
    "BtnDeletSlot()\0Row\0MaterListSlot(int)\0"
    "UpDataMaterListSLot(QMap<int,MCompoment>)\0"
    "p\0contextMaterMenuSlot(QPoint)\0"
    "DelFileNameSlot()\0index\0"
    "ChangeModelIndexSlot(QModelIndex)\0"
    "row,col\0DataTestSlot(int,int)\0"
    "NewMatialDlgSlot()\0EditMaterialSlot()\0"
    "QMap<int,PhaseBaseItem>\0BalanceList\0"
    "ProcessMapData(QList<HPphaseBalanceInps>)\0"
};

void QMaterPropDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QMaterPropDlg *_t = static_cast<QMaterPropDlg *>(_o);
        switch (_id) {
        case 0: _t->emitMaterParam((*reinterpret_cast< SectionS(*)>(_a[1]))); break;
        case 1: _t->BtnApplaySlot(); break;
        case 2: _t->BtnCancelSlot(); break;
        case 3: _t->BtnImportDataSlot(); break;
        case 4: _t->BtnExportDataSlot(); break;
        case 5: _t->BtnDeletSlot(); break;
        case 6: _t->MaterListSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->UpDataMaterListSLot((*reinterpret_cast< QMap<int,MCompoment>(*)>(_a[1]))); break;
        case 8: _t->contextMaterMenuSlot((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 9: _t->DelFileNameSlot(); break;
        case 10: _t->ChangeModelIndexSlot((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 11: _t->DataTestSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->NewMatialDlgSlot(); break;
        case 13: _t->EditMaterialSlot(); break;
        case 14: { QMap<int,PhaseBaseItem> _r = _t->ProcessMapData((*reinterpret_cast< QList<HPphaseBalanceInps>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QMap<int,PhaseBaseItem>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QMaterPropDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QMaterPropDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QMaterPropDlg,
      qt_meta_data_QMaterPropDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMaterPropDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMaterPropDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMaterPropDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMaterPropDlg))
        return static_cast<void*>(const_cast< QMaterPropDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QMaterPropDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void QMaterPropDlg::emitMaterParam(SectionS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
