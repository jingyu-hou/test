/****************************************************************************
** Meta object code from reading C++ file 'QPostWidPlotOptDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QPostWidPlotOptDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QPostWidPlotOptDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QPostWidPlotOptDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   19,   19,   19, 0x0a,
      65,   19,   19,   19, 0x0a,
      77,   19,   19,   19, 0x0a,
      95,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QPostWidPlotOptDlg[] = {
    "QPostWidPlotOptDlg\0\0m_OptDlgS\0"
    "emitParam(OptDlgS)\0BtnCancelSlot()\0"
    "BtnOkSlot()\0BtnDefaultsSlot()\0"
    "BtnApplySlot()\0"
};

void QPostWidPlotOptDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPostWidPlotOptDlg *_t = static_cast<QPostWidPlotOptDlg *>(_o);
        switch (_id) {
        case 0: _t->emitParam((*reinterpret_cast< OptDlgS(*)>(_a[1]))); break;
        case 1: _t->BtnCancelSlot(); break;
        case 2: _t->BtnOkSlot(); break;
        case 3: _t->BtnDefaultsSlot(); break;
        case 4: _t->BtnApplySlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QPostWidPlotOptDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPostWidPlotOptDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QPostWidPlotOptDlg,
      qt_meta_data_QPostWidPlotOptDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPostWidPlotOptDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPostWidPlotOptDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPostWidPlotOptDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPostWidPlotOptDlg))
        return static_cast<void*>(const_cast< QPostWidPlotOptDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QPostWidPlotOptDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QPostWidPlotOptDlg::emitParam(OptDlgS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_QPostWidPlotOpt_Basic[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   23,   22,   22, 0x08,
      48,   23,   22,   22, 0x08,
      66,   23,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPostWidPlotOpt_Basic[] = {
    "QPostWidPlotOpt_Basic\0\0Index\0"
    "setRenderSlot(int)\0setScaleSlot(int)\0"
    "setVisibleSlot(int)\0"
};

void QPostWidPlotOpt_Basic::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPostWidPlotOpt_Basic *_t = static_cast<QPostWidPlotOpt_Basic *>(_o);
        switch (_id) {
        case 0: _t->setRenderSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setScaleSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setVisibleSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QPostWidPlotOpt_Basic::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPostWidPlotOpt_Basic::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPostWidPlotOpt_Basic,
      qt_meta_data_QPostWidPlotOpt_Basic, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPostWidPlotOpt_Basic::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPostWidPlotOpt_Basic::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPostWidPlotOpt_Basic::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPostWidPlotOpt_Basic))
        return static_cast<void*>(const_cast< QPostWidPlotOpt_Basic*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPostWidPlotOpt_Basic::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_QPostWidPlotOpt_Color[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   23,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPostWidPlotOpt_Color[] = {
    "QPostWidPlotOpt_Color\0\0btnId\0"
    "setColorScale(int)\0"
};

void QPostWidPlotOpt_Color::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPostWidPlotOpt_Color *_t = static_cast<QPostWidPlotOpt_Color *>(_o);
        switch (_id) {
        case 0: _t->setColorScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QPostWidPlotOpt_Color::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPostWidPlotOpt_Color::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPostWidPlotOpt_Color,
      qt_meta_data_QPostWidPlotOpt_Color, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPostWidPlotOpt_Color::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPostWidPlotOpt_Color::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPostWidPlotOpt_Color::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPostWidPlotOpt_Color))
        return static_cast<void*>(const_cast< QPostWidPlotOpt_Color*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPostWidPlotOpt_Color::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_QPostWidPlotOpt_Labels[] = {

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
      30,   24,   23,   23, 0x08,
      53,   23,   23,   23, 0x08,
      74,   23,   23,   23, 0x08,
      95,   23,   23,   23, 0x08,
     116,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPostWidPlotOpt_Labels[] = {
    "QPostWidPlotOpt_Labels\0\0btnId\0"
    "setColorScaleSlot(int)\0ChkShowNodeSysSlot()\0"
    "ChkShowElemLabSlot()\0ChkShowFaceLabSlot()\0"
    "ChkShowNodeLabSlot()\0"
};

void QPostWidPlotOpt_Labels::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPostWidPlotOpt_Labels *_t = static_cast<QPostWidPlotOpt_Labels *>(_o);
        switch (_id) {
        case 0: _t->setColorScaleSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ChkShowNodeSysSlot(); break;
        case 2: _t->ChkShowElemLabSlot(); break;
        case 3: _t->ChkShowFaceLabSlot(); break;
        case 4: _t->ChkShowNodeLabSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QPostWidPlotOpt_Labels::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPostWidPlotOpt_Labels::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPostWidPlotOpt_Labels,
      qt_meta_data_QPostWidPlotOpt_Labels, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPostWidPlotOpt_Labels::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPostWidPlotOpt_Labels::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPostWidPlotOpt_Labels::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPostWidPlotOpt_Labels))
        return static_cast<void*>(const_cast< QPostWidPlotOpt_Labels*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPostWidPlotOpt_Labels::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_QPostWidPlotOpt_Normals[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   25,   24,   24, 0x08,
      54,   24,   24,   24, 0x08,
      77,   71,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QPostWidPlotOpt_Normals[] = {
    "QPostWidPlotOpt_Normals\0\0btnId\0"
    "setColorScaleSlot(int)\0ChkShowNomSlot()\0"
    "Index\0setVisibleSlot(int)\0"
};

void QPostWidPlotOpt_Normals::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPostWidPlotOpt_Normals *_t = static_cast<QPostWidPlotOpt_Normals *>(_o);
        switch (_id) {
        case 0: _t->setColorScaleSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ChkShowNomSlot(); break;
        case 2: _t->setVisibleSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QPostWidPlotOpt_Normals::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPostWidPlotOpt_Normals::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPostWidPlotOpt_Normals,
      qt_meta_data_QPostWidPlotOpt_Normals, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPostWidPlotOpt_Normals::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPostWidPlotOpt_Normals::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPostWidPlotOpt_Normals::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPostWidPlotOpt_Normals))
        return static_cast<void*>(const_cast< QPostWidPlotOpt_Normals*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPostWidPlotOpt_Normals::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_QPostWidPlotOpt_Other[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_QPostWidPlotOpt_Other[] = {
    "QPostWidPlotOpt_Other\0"
};

void QPostWidPlotOpt_Other::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QPostWidPlotOpt_Other::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QPostWidPlotOpt_Other::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPostWidPlotOpt_Other,
      qt_meta_data_QPostWidPlotOpt_Other, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPostWidPlotOpt_Other::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPostWidPlotOpt_Other::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPostWidPlotOpt_Other::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPostWidPlotOpt_Other))
        return static_cast<void*>(const_cast< QPostWidPlotOpt_Other*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPostWidPlotOpt_Other::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
