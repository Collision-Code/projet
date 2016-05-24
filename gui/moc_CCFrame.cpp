/****************************************************************************
** Meta object code from reading C++ file 'CCFrame.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CCFrame.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CCFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Worker_t {
    QByteArrayData data[6];
    char stringdata0[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Worker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Worker_t qt_meta_stringdata_Worker = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Worker"
QT_MOC_LITERAL(1, 7, 8), // "finished"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 6), // "doWork"
QT_MOC_LITERAL(4, 24, 11), // "StdCmdView*"
QT_MOC_LITERAL(5, 36, 3) // "cmd"

    },
    "Worker\0finished\0\0doWork\0StdCmdView*\0"
    "cmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Worker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void Worker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Worker *_t = static_cast<Worker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->doWork((*reinterpret_cast< StdCmdView*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Worker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Worker::finished)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Worker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Worker.data,
      qt_meta_data_Worker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Worker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Worker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Worker.stringdata0))
        return static_cast<void*>(const_cast< Worker*>(this));
    return QObject::qt_metacast(_clname);
}

int Worker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Worker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_CCFrame_t {
    QByteArrayData data[40];
    char stringdata0[800];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCFrame_t qt_meta_stringdata_CCFrame = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CCFrame"
QT_MOC_LITERAL(1, 8, 11), // "totalPoints"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 5), // "value"
QT_MOC_LITERAL(4, 27, 22), // "changeProgressBarValue"
QT_MOC_LITERAL(5, 50, 27), // "changeProgressBarVisibility"
QT_MOC_LITERAL(6, 78, 14), // "disableWidgets"
QT_MOC_LITERAL(7, 93, 13), // "changeResults"
QT_MOC_LITERAL(8, 107, 3), // "str"
QT_MOC_LITERAL(9, 111, 16), // "resultHasChanged"
QT_MOC_LITERAL(10, 128, 6), // "method"
QT_MOC_LITERAL(11, 135, 5), // "index"
QT_MOC_LITERAL(12, 141, 16), // "callWorkerThread"
QT_MOC_LITERAL(13, 158, 11), // "StdCmdView*"
QT_MOC_LITERAL(14, 170, 3), // "cmd"
QT_MOC_LITERAL(15, 174, 16), // "openChemicalFile"
QT_MOC_LITERAL(16, 191, 14), // "openChargeFile"
QT_MOC_LITERAL(17, 206, 17), // "openAtomInfosFile"
QT_MOC_LITERAL(18, 224, 11), // "saveResults"
QT_MOC_LITERAL(19, 236, 5), // "about"
QT_MOC_LITERAL(20, 242, 31), // "updateModelShouldPABeCalculated"
QT_MOC_LITERAL(21, 274, 33), // "updateModelShouldEHSSBeCalcul..."
QT_MOC_LITERAL(22, 308, 31), // "updateModelShouldTMBeCalculated"
QT_MOC_LITERAL(23, 340, 28), // "updateModelLaunchCalculation"
QT_MOC_LITERAL(24, 369, 27), // "updateModelMaxNumberThreads"
QT_MOC_LITERAL(25, 397, 38), // "updateModelNbPointsMCIntegrat..."
QT_MOC_LITERAL(26, 436, 22), // "updateModelTemperature"
QT_MOC_LITERAL(27, 459, 38), // "updateModelEnergyConservation..."
QT_MOC_LITERAL(28, 498, 27), // "updateModelNbCompleteCycles"
QT_MOC_LITERAL(29, 526, 27), // "updateModelNbVelocityPoints"
QT_MOC_LITERAL(30, 554, 34), // "updateModelNbPointsMCIntegrat..."
QT_MOC_LITERAL(31, 589, 31), // "updateModelPotentialEnergyStart"
QT_MOC_LITERAL(32, 621, 40), // "updateModelPotentialEnergyClo..."
QT_MOC_LITERAL(33, 662, 24), // "updateModelTimeStepStart"
QT_MOC_LITERAL(34, 687, 33), // "updateModelTimeStepCloseColli..."
QT_MOC_LITERAL(35, 721, 14), // "expandAllNodes"
QT_MOC_LITERAL(36, 736, 16), // "updateResultList"
QT_MOC_LITERAL(37, 753, 12), // "printResults"
QT_MOC_LITERAL(38, 766, 15), // "resultsAreReady"
QT_MOC_LITERAL(39, 782, 17) // "killThreadAndExit"

    },
    "CCFrame\0totalPoints\0\0value\0"
    "changeProgressBarValue\0"
    "changeProgressBarVisibility\0disableWidgets\0"
    "changeResults\0str\0resultHasChanged\0"
    "method\0index\0callWorkerThread\0StdCmdView*\0"
    "cmd\0openChemicalFile\0openChargeFile\0"
    "openAtomInfosFile\0saveResults\0about\0"
    "updateModelShouldPABeCalculated\0"
    "updateModelShouldEHSSBeCalculated\0"
    "updateModelShouldTMBeCalculated\0"
    "updateModelLaunchCalculation\0"
    "updateModelMaxNumberThreads\0"
    "updateModelNbPointsMCIntegrationEHSSPA\0"
    "updateModelTemperature\0"
    "updateModelEnergyConservationThreshold\0"
    "updateModelNbCompleteCycles\0"
    "updateModelNbVelocityPoints\0"
    "updateModelNbPointsMCIntegrationTM\0"
    "updateModelPotentialEnergyStart\0"
    "updateModelPotentialEnergyCloseCollision\0"
    "updateModelTimeStepStart\0"
    "updateModelTimeStepCloseCollision\0"
    "expandAllNodes\0updateResultList\0"
    "printResults\0resultsAreReady\0"
    "killThreadAndExit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  174,    2, 0x06 /* Public */,
       4,    1,  177,    2, 0x06 /* Public */,
       5,    1,  180,    2, 0x06 /* Public */,
       6,    1,  183,    2, 0x06 /* Public */,
       7,    1,  186,    2, 0x06 /* Public */,
       9,    3,  189,    2, 0x06 /* Public */,
      12,    1,  196,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,  199,    2, 0x0a /* Public */,
      16,    0,  200,    2, 0x0a /* Public */,
      17,    0,  201,    2, 0x0a /* Public */,
      18,    0,  202,    2, 0x0a /* Public */,
      19,    0,  203,    2, 0x0a /* Public */,
      20,    1,  204,    2, 0x0a /* Public */,
      21,    1,  207,    2, 0x0a /* Public */,
      22,    1,  210,    2, 0x0a /* Public */,
      23,    0,  213,    2, 0x0a /* Public */,
      24,    1,  214,    2, 0x0a /* Public */,
      25,    1,  217,    2, 0x0a /* Public */,
      26,    1,  220,    2, 0x0a /* Public */,
      27,    1,  223,    2, 0x0a /* Public */,
      28,    1,  226,    2, 0x0a /* Public */,
      29,    1,  229,    2, 0x0a /* Public */,
      30,    1,  232,    2, 0x0a /* Public */,
      31,    1,  235,    2, 0x0a /* Public */,
      32,    1,  238,    2, 0x0a /* Public */,
      33,    1,  241,    2, 0x0a /* Public */,
      34,    1,  244,    2, 0x0a /* Public */,
      35,    1,  247,    2, 0x0a /* Public */,
      36,    3,  250,    2, 0x0a /* Public */,
      37,    1,  257,    2, 0x0a /* Public */,
      38,    0,  260,    2, 0x0a /* Public */,
      39,    0,  261,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Double,   10,   11,    3,
    QMetaType::Void, 0x80000000 | 13,   14,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Double,   10,   11,    3,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CCFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCFrame *_t = static_cast<CCFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->totalPoints((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->changeProgressBarValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeProgressBarVisibility((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->disableWidgets((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->changeResults((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->resultHasChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 6: _t->callWorkerThread((*reinterpret_cast< StdCmdView*(*)>(_a[1]))); break;
        case 7: _t->openChemicalFile(); break;
        case 8: _t->openChargeFile(); break;
        case 9: _t->openAtomInfosFile(); break;
        case 10: _t->saveResults(); break;
        case 11: _t->about(); break;
        case 12: _t->updateModelShouldPABeCalculated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->updateModelShouldEHSSBeCalculated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->updateModelShouldTMBeCalculated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->updateModelLaunchCalculation(); break;
        case 16: _t->updateModelMaxNumberThreads((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->updateModelNbPointsMCIntegrationEHSSPA((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->updateModelTemperature((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 19: _t->updateModelEnergyConservationThreshold((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 20: _t->updateModelNbCompleteCycles((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->updateModelNbVelocityPoints((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->updateModelNbPointsMCIntegrationTM((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->updateModelPotentialEnergyStart((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 24: _t->updateModelPotentialEnergyCloseCollision((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 25: _t->updateModelTimeStepStart((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 26: _t->updateModelTimeStepCloseCollision((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 27: _t->expandAllNodes((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->updateResultList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 29: _t->printResults((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 30: _t->resultsAreReady(); break;
        case 31: _t->killThreadAndExit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CCFrame::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCFrame::totalPoints)) {
                *result = 0;
            }
        }
        {
            typedef void (CCFrame::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCFrame::changeProgressBarValue)) {
                *result = 1;
            }
        }
        {
            typedef void (CCFrame::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCFrame::changeProgressBarVisibility)) {
                *result = 2;
            }
        }
        {
            typedef void (CCFrame::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCFrame::disableWidgets)) {
                *result = 3;
            }
        }
        {
            typedef void (CCFrame::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCFrame::changeResults)) {
                *result = 4;
            }
        }
        {
            typedef void (CCFrame::*_t)(QString , int , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCFrame::resultHasChanged)) {
                *result = 5;
            }
        }
        {
            typedef void (CCFrame::*_t)(StdCmdView * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCFrame::callWorkerThread)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject CCFrame::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CCFrame.data,
      qt_meta_data_CCFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CCFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CCFrame.stringdata0))
        return static_cast<void*>(const_cast< CCFrame*>(this));
    if (!strcmp(_clname, "Observer"))
        return static_cast< Observer*>(const_cast< CCFrame*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CCFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void CCFrame::totalPoints(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CCFrame::changeProgressBarValue(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CCFrame::changeProgressBarVisibility(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CCFrame::disableWidgets(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CCFrame::changeResults(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CCFrame::resultHasChanged(QString _t1, int _t2, double _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CCFrame::callWorkerThread(StdCmdView * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
