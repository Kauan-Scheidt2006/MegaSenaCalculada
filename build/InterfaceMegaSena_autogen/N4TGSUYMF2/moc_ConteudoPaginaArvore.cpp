/****************************************************************************
** Meta object code from reading C++ file 'ConteudoPaginaArvore.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/interface/paginas/arvore/ConteudoPaginaArvore.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConteudoPaginaArvore.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN20ConteudoPaginaArvoreE_t {};
} // unnamed namespace

template <> constexpr inline auto ConteudoPaginaArvore::qt_create_metaobjectdata<qt_meta_tag_ZN20ConteudoPaginaArvoreE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ConteudoPaginaArvore",
        "detalhesRegistroSolicitados",
        "",
        "apresentacao::arvore::IdentificadorRegistro",
        "identificador",
        "proximoPassoAvlSolicitado",
        "reproducaoAvlSolicitada",
        "pausaAvlSolicitada",
        "reinicioAvlSolicitado",
        "percursoEmOrdemSolicitado",
        "percursoPreOrdemSolicitado",
        "percursoPosOrdemSolicitado",
        "buscaSolicitada",
        "remocaoSolicitada"
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
        // Signal 'percursoEmOrdemSolicitado'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'percursoPreOrdemSolicitado'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'percursoPosOrdemSolicitado'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'buscaSolicitada'
        QtMocHelpers::SignalData<void(apresentacao::arvore::IdentificadorRegistro)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'remocaoSolicitada'
        QtMocHelpers::SignalData<void(apresentacao::arvore::IdentificadorRegistro)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ConteudoPaginaArvore, qt_meta_tag_ZN20ConteudoPaginaArvoreE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ConteudoPaginaArvore::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseContainer::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ConteudoPaginaArvoreE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ConteudoPaginaArvoreE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20ConteudoPaginaArvoreE_t>.metaTypes,
    nullptr
} };

void ConteudoPaginaArvore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ConteudoPaginaArvore *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->detalhesRegistroSolicitados((*reinterpret_cast<std::add_pointer_t<apresentacao::arvore::IdentificadorRegistro>>(_a[1]))); break;
        case 1: _t->proximoPassoAvlSolicitado(); break;
        case 2: _t->reproducaoAvlSolicitada(); break;
        case 3: _t->pausaAvlSolicitada(); break;
        case 4: _t->reinicioAvlSolicitado(); break;
        case 5: _t->percursoEmOrdemSolicitado(); break;
        case 6: _t->percursoPreOrdemSolicitado(); break;
        case 7: _t->percursoPosOrdemSolicitado(); break;
        case 8: _t->buscaSolicitada((*reinterpret_cast<std::add_pointer_t<apresentacao::arvore::IdentificadorRegistro>>(_a[1]))); break;
        case 9: _t->remocaoSolicitada((*reinterpret_cast<std::add_pointer_t<apresentacao::arvore::IdentificadorRegistro>>(_a[1]))); break;
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
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< apresentacao::arvore::IdentificadorRegistro >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< apresentacao::arvore::IdentificadorRegistro >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)(apresentacao::arvore::IdentificadorRegistro )>(_a, &ConteudoPaginaArvore::detalhesRegistroSolicitados, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)()>(_a, &ConteudoPaginaArvore::proximoPassoAvlSolicitado, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)()>(_a, &ConteudoPaginaArvore::reproducaoAvlSolicitada, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)()>(_a, &ConteudoPaginaArvore::pausaAvlSolicitada, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)()>(_a, &ConteudoPaginaArvore::reinicioAvlSolicitado, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)()>(_a, &ConteudoPaginaArvore::percursoEmOrdemSolicitado, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)()>(_a, &ConteudoPaginaArvore::percursoPreOrdemSolicitado, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)()>(_a, &ConteudoPaginaArvore::percursoPosOrdemSolicitado, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)(apresentacao::arvore::IdentificadorRegistro )>(_a, &ConteudoPaginaArvore::buscaSolicitada, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConteudoPaginaArvore::*)(apresentacao::arvore::IdentificadorRegistro )>(_a, &ConteudoPaginaArvore::remocaoSolicitada, 9))
            return;
    }
}

const QMetaObject *ConteudoPaginaArvore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConteudoPaginaArvore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ConteudoPaginaArvoreE_t>.strings))
        return static_cast<void*>(this);
    return BaseContainer::qt_metacast(_clname);
}

int ConteudoPaginaArvore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseContainer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ConteudoPaginaArvore::detalhesRegistroSolicitados(apresentacao::arvore::IdentificadorRegistro _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ConteudoPaginaArvore::proximoPassoAvlSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ConteudoPaginaArvore::reproducaoAvlSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ConteudoPaginaArvore::pausaAvlSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ConteudoPaginaArvore::reinicioAvlSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ConteudoPaginaArvore::percursoEmOrdemSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ConteudoPaginaArvore::percursoPreOrdemSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ConteudoPaginaArvore::percursoPosOrdemSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ConteudoPaginaArvore::buscaSolicitada(apresentacao::arvore::IdentificadorRegistro _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void ConteudoPaginaArvore::remocaoSolicitada(apresentacao::arvore::IdentificadorRegistro _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}
QT_WARNING_POP
