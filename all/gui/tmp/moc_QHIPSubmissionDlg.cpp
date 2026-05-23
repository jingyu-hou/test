/****************************************************************************
** Meta object code from reading C++ file 'QHIPSubmissionDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/QHIPSubmissionDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QHIPSubmissionDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHIPSubmissionDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      36,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      52,   18,   18,   18, 0x0a,
      69,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QHIPSubmissionDlg[] = {
    "QHIPSubmissionDlg\0\0emitHipInpSave()\0"
    "emitHipInpRun()\0SaveHIPInpSlot()\0"
    "RunSolverSlot()\0"
};

void QHIPSubmissionDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QHIPSubmissionDlg *_t = static_cast<QHIPSubmissionDlg *>(_o);
        switch (_id) {
        case 0: _t->emitHipInpSave(); break;
        case 1: _t->emitHipInpRun(); break;
        case 2: _t->SaveHIPInpSlot(); break;
        case 3: _t->RunSolverSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QHIPSubmissionDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QHIPSubmissionDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QHIPSubmissionDlg,
      qt_meta_data_QHIPSubmissionDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHIPSubmissionDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHIPSubmissionDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHIPSubmissionDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHIPSubmissionDlg))
        return static_cast<void*>(const_cast< QHIPSubmissionDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QHIPSubmissionDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void QHIPSubmissionDlg::emitHipInpSave()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QHIPSubmissionDlg::emitHipInpRun()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
