/****************************************************************************
** Meta object code from reading C++ file 'QTreeMangerView.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QTreeMangerView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QTreeMangerView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTreeMangerView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   17,   16,   16, 0x05,
      57,   17,   16,   16, 0x05,
      95,   17,   16,   16, 0x05,
     133,  131,   16,   16, 0x05,
     163,   16,  155,   16, 0x05,
     186,   16,  155,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     217,  208,   16,   16, 0x08,
     247,  245,   16,   16, 0x08,
     274,   16,   16,   16, 0x08,
     285,   16,   16,   16, 0x08,
     300,   16,   16,   16, 0x08,
     316,   16,   16,   16, 0x08,
     331,  131,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QTreeMangerView[] = {
    "QTreeMangerView\0\0index\0"
    "EmitChangeModelIndex(QModelIndex)\0"
    "EmitChangeModeIndexMater(QModelIndex)\0"
    "EmitChangeModeIndexPre(QModelIndex)\0"
    ",\0EmitNewCurve(int,int)\0QString\0"
    "EmitCurveEdit(QString)\0EmitCurveDel(QString)\0"
    "contextM\0contextMenuPostSlot(QPoint)\0"
    "p\0contextMenuPreSlot(QPoint)\0TestSlot()\0"
    "NewCurveSlot()\0EditCurveSlot()\0"
    "DelCurveSlot()\0upDataTreeNameSlot(QString,QStringList)\0"
};

void QTreeMangerView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTreeMangerView *_t = static_cast<QTreeMangerView *>(_o);
        switch (_id) {
        case 0: _t->EmitChangeModelIndex((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->EmitChangeModeIndexMater((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->EmitChangeModeIndexPre((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->EmitNewCurve((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: { QString _r = _t->EmitCurveEdit((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->EmitCurveDel((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: _t->contextMenuPostSlot((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: _t->contextMenuPreSlot((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 8: _t->TestSlot(); break;
        case 9: _t->NewCurveSlot(); break;
        case 10: _t->EditCurveSlot(); break;
        case 11: _t->DelCurveSlot(); break;
        case 12: _t->upDataTreeNameSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTreeMangerView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTreeMangerView::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_QTreeMangerView,
      qt_meta_data_QTreeMangerView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTreeMangerView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTreeMangerView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTreeMangerView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTreeMangerView))
        return static_cast<void*>(const_cast< QTreeMangerView*>(this));
    return QTreeView::qt_metacast(_clname);
}

int QTreeMangerView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
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
void QTreeMangerView::EmitChangeModelIndex(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QTreeMangerView::EmitChangeModeIndexMater(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QTreeMangerView::EmitChangeModeIndexPre(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QTreeMangerView::EmitNewCurve(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
QString QTreeMangerView::EmitCurveEdit(QString _t1)
{
    QString _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
    return _t0;
}

// SIGNAL 5
QString QTreeMangerView::EmitCurveDel(QString _t1)
{
    QString _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
    return _t0;
}
QT_END_MOC_NAMESPACE
