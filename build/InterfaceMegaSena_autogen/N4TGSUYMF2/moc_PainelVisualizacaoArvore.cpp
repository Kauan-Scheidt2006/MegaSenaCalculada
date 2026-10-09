/****************************************************************************
** Meta object code from reading C++ file 'PainelVisualizacaoArvore.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/interface/paginas/arvore/PainelVisualizacaoArvore.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PainelVisualizacaoArvore.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN24PainelVisualizacaoArvoreE_t {};
} // unnamed namespace

template <> constexpr inline auto PainelVisualizacaoArvore::qt_create_metaobjectdata<qt_meta_tag_ZN24PainelVisualizacaoArvoreE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PainelVisualizacaoArvore",
        "detalhesRegistroSolicitados",
        "",
        "apresentacao::arvore::IdentificadorRegistro",
        "identificador",
        "proximoPassoAvlSolicitado",
        "reproducaoAvlSolicitada",
        "pausaAvlSolicitada",
        "reinicioAvlSolicitado"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'detalhesRegistroSolicitados'
        QtMocHelpers::SignalData<void(apresentacao::arvore::IdentificadorRegistro)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'proximoPassoAvlSolicitado'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reproducaoAvlSolicitada'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pausaAvlSolicitada'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reinicioAvlSolicitado'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PainelVisualizacaoArvore, qt_meta_tag_ZN24PainelVisualizacaoArvoreE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PainelVisualizacaoArvore::staticMetaObject = { {
    QMetaObject::SuperData::link<Painel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24PainelVisualizacaoArvoreE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24PainelVisualizacaoArvoreE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN24PainelVisualizacaoArvoreE_t>.metaTypes,
    nullptr
} };

void PainelVisualizacaoArvore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PainelVisualizacaoArvore *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->detalhesRegistroSolicitados((*reinterpret_cast<std::add_pointer_t<apresentacao::arvore::IdentificadorRegistro>>(_a[1]))); break;
        case 1: _t->proximoPassoAvlSolicitado(); break;
        case 2: _t->reproducaoAvlSolicitada(); break;
        case 3: _t->pausaAvlSolicitada(); break;
        case 4: _t->reinicioAvlSolicitado(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< apresentacao::arvore::IdentificadorRegistro >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PainelVisualizacaoArvore::*)(apresentacao::arvore::IdentificadorRegistro )>(_a, &PainelVisualizacaoArvore::detalhesRegistroSolicitados, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PainelVisualizacaoArvore::*)()>(_a, &PainelVisualizacaoArvore::proximoPassoAvlSolicitado, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PainelVisualizacaoArvore::*)()>(_a, &PainelVisualizacaoArvore::reproducaoAvlSolicitada, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PainelVisualizacaoArvore::*)()>(_a, &PainelVisualizacaoArvore::pausaAvlSolicitada, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (PainelVisualizacaoArvore::*)()>(_a, &PainelVisualizacaoArvore::reinicioAvlSolicitado, 4))
            return;
    }
}

const QMetaObject *PainelVisualizacaoArvore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PainelVisualizacaoArvore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24PainelVisualizacaoArvoreE_t>.strings))
        return static_cast<void*>(this);
    return Painel::qt_metacast(_clname);
}

int PainelVisualizacaoArvore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Painel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void PainelVisualizacaoArvore::detalhesRegistroSolicitados(apresentacao::arvore::IdentificadorRegistro _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void PainelVisualizacaoArvore::proximoPassoAvlSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PainelVisualizacaoArvore::reproducaoAvlSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PainelVisualizacaoArvore::pausaAvlSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PainelVisualizacaoArvore::reinicioAvlSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
