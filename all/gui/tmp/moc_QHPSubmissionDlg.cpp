/****************************************************************************
** Meta object code from reading C++ file 'QHPSubmissionDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/HPprocess/QHPSubmissionDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QHPSubmissionDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHPSubmissionDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      34,   17,   17,   17, 0x05,
      49,   17,   17,   17, 0x05,
      61,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      78,   17,   17,   17, 0x0a,
      92,   17,   17,   17, 0x0a,
     108,   17,   17,   17, 0x0a,
     124,   17,   17,   17, 0x0a,
     136,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QHPSubmissionDlg[] = {
    "QHPSubmissionDlg\0\0emitHpInpSave()\0"
    "emitHpInpRun()\0emitCheck()\0emitHipInpKill()\0"
    "SetPathSlot()\0SaveHPInpSlot()\0"
    "RunSolverSlot()\0CheckSlot()\0KillolverSlot()\0"
};

void QHPSubmissionDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QHPSubmissionDlg *_t = static_cast<QHPSubmissionDlg *>(_o);
        switch (_id) {
        case 0: _t->emitHpInpSave(); break;
        case 1: _t->emitHpInpRun(); break;
        case 2: _t->emitCheck(); break;
        case 3: _t->emitHipInpKill(); break;
        case 4: _t->SetPathSlot(); break;
        case 5: _t->SaveHPInpSlot(); break;
        case 6: _t->RunSolverSlot(); break;
        case 7: _t->CheckSlot(); break;
        case 8: _t->KillolverSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QHPSubmissionDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QHPSubmissionDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QHPSubmissionDlg,
      qt_meta_data_QHPSubmissionDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHPSubmissionDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHPSubmissionDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHPSubmissionDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHPSubmissionDlg))
        return static_cast<void*>(const_cast< QHPSubmissionDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int QHPSubmissionDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void QHPSubmissionDlg::emitHpInpSave()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QHPSubmissionDlg::emitHpInpRun()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QHPSubmissionDlg::emitCheck()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QHPSubmissionDlg::emitHipInpKill()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
