/****************************************************************************
** Meta object code from reading C++ file 'ControleExecucaoAvl.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/interface/componentes/arvore/ControleExecucaoAvl.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControleExecucaoAvl.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN19ControleExecucaoAvlE_t {};
} // unnamed namespace

template <> constexpr inline auto ControleExecucaoAvl::qt_create_metaobjectdata<qt_meta_tag_ZN19ControleExecucaoAvlE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ControleExecucaoAvl",
        "proximoPassoSolicitado",
        "",
        "reproducaoSolicitada",
        "pausaSolicitada",
        "reinicioSolicitado"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'proximoPassoSolicitado'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reproducaoSolicitada'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pausaSolicitada'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reinicioSolicitado'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ControleExecucaoAvl, qt_meta_tag_ZN19ControleExecucaoAvlE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ControleExecucaoAvl::staticMetaObject = { {
    QMetaObject::SuperData::link<Barra::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ControleExecucaoAvlE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ControleExecucaoAvlE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19ControleExecucaoAvlE_t>.metaTypes,
    nullptr
} };

void ControleExecucaoAvl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ControleExecucaoAvl *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->proximoPassoSolicitado(); break;
        case 1: _t->reproducaoSolicitada(); break;
        case 2: _t->pausaSolicitada(); break;
        case 3: _t->reinicioSolicitado(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ControleExecucaoAvl::*)()>(_a, &ControleExecucaoAvl::proximoPassoSolicitado, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControleExecucaoAvl::*)()>(_a, &ControleExecucaoAvl::reproducaoSolicitada, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControleExecucaoAvl::*)()>(_a, &ControleExecucaoAvl::pausaSolicitada, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControleExecucaoAvl::*)()>(_a, &ControleExecucaoAvl::reinicioSolicitado, 3))
            return;
    }
}

const QMetaObject *ControleExecucaoAvl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControleExecucaoAvl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ControleExecucaoAvlE_t>.strings))
        return static_cast<void*>(this);
    return Barra::qt_metacast(_clname);
}

int ControleExecucaoAvl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Barra::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ControleExecucaoAvl::proximoPassoSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ControleExecucaoAvl::reproducaoSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ControleExecucaoAvl::pausaSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ControleExecucaoAvl::reinicioSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
