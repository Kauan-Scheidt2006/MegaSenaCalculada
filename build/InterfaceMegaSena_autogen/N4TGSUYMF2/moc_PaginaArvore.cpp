/****************************************************************************
** Meta object code from reading C++ file 'PaginaArvore.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/interface/paginas/arvore/PaginaArvore.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PaginaArvore.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12PaginaArvoreE_t {};
} // unnamed namespace

template <> constexpr inline auto PaginaArvore::qt_create_metaobjectdata<qt_meta_tag_ZN12PaginaArvoreE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PaginaArvore",
        "estruturaSelecionada",
        "",
        "TipoEstrutura",
        "tipo",
        "detalhesRegistroSolicitados",
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
        // Signal 'estruturaSelecionada'
        QtMocHelpers::SignalData<void(TipoEstrutura)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'detalhesRegistroSolicitados'
        QtMocHelpers::SignalData<void(apresentacao::arvore::IdentificadorRegistro)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'proximoPassoAvlSolicitado'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reproducaoAvlSolicitada'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pausaAvlSolicitada'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reinicioAvlSolicitado'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'percursoEmOrdemSolicitado'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'percursoPreOrdemSolicitado'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'percursoPosOrdemSolicitado'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'buscaSolicitada'
        QtMocHelpers::SignalData<void(apresentacao::arvore::IdentificadorRegistro)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'remocaoSolicitada'
        QtMocHelpers::SignalData<void(apresentacao::arvore::IdentificadorRegistro)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PaginaArvore, qt_meta_tag_ZN12PaginaArvoreE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PaginaArvore::staticMetaObject = { {
    QMetaObject::SuperData::link<BasePagina::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PaginaArvoreE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PaginaArvoreE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12PaginaArvoreE_t>.metaTypes,
    nullptr
} };

void PaginaArvore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PaginaArvore *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->estruturaSelecionada((*reinterpret_cast<std::add_pointer_t<TipoEstrutura>>(_a[1]))); break;
        case 1: _t->detalhesRegistroSolicitados((*reinterpret_cast<std::add_pointer_t<apresentacao::arvore::IdentificadorRegistro>>(_a[1]))); break;
        case 2: _t->proximoPassoAvlSolicitado(); break;
        case 3: _t->reproducaoAvlSolicitada(); break;
        case 4: _t->pausaAvlSolicitada(); break;
        case 5: _t->reinicioAvlSolicitado(); break;
        case 6: _t->percursoEmOrdemSolicitado(); break;
        case 7: _t->percursoPreOrdemSolicitado(); break;
        case 8: _t->percursoPosOrdemSolicitado(); break;
        case 9: _t->buscaSolicitada((*reinterpret_cast<std::add_pointer_t<apresentacao::arvore::IdentificadorRegistro>>(_a[1]))); break;
        case 10: _t->remocaoSolicitada((*reinterpret_cast<std::add_pointer_t<apresentacao::arvore::IdentificadorRegistro>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
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
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< apresentacao::arvore::IdentificadorRegistro >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)(TipoEstrutura )>(_a, &PaginaArvore::estruturaSelecionada, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)(apresentacao::arvore::IdentificadorRegistro )>(_a, &PaginaArvore::detalhesRegistroSolicitados, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)()>(_a, &PaginaArvore::proximoPassoAvlSolicitado, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)()>(_a, &PaginaArvore::reproducaoAvlSolicitada, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)()>(_a, &PaginaArvore::pausaAvlSolicitada, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)()>(_a, &PaginaArvore::reinicioAvlSolicitado, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)()>(_a, &PaginaArvore::percursoEmOrdemSolicitado, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)()>(_a, &PaginaArvore::percursoPreOrdemSolicitado, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)()>(_a, &PaginaArvore::percursoPosOrdemSolicitado, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)(apresentacao::arvore::IdentificadorRegistro )>(_a, &PaginaArvore::buscaSolicitada, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (PaginaArvore::*)(apresentacao::arvore::IdentificadorRegistro )>(_a, &PaginaArvore::remocaoSolicitada, 10))
            return;
    }
}

const QMetaObject *PaginaArvore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PaginaArvore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PaginaArvoreE_t>.strings))
        return static_cast<void*>(this);
    return BasePagina::qt_metacast(_clname);
}

int PaginaArvore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BasePagina::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void PaginaArvore::estruturaSelecionada(TipoEstrutura _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void PaginaArvore::detalhesRegistroSolicitados(apresentacao::arvore::IdentificadorRegistro _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void PaginaArvore::proximoPassoAvlSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PaginaArvore::reproducaoAvlSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PaginaArvore::pausaAvlSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void PaginaArvore::reinicioAvlSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void PaginaArvore::percursoEmOrdemSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void PaginaArvore::percursoPreOrdemSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void PaginaArvore::percursoPosOrdemSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void PaginaArvore::buscaSolicitada(apresentacao::arvore::IdentificadorRegistro _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void PaginaArvore::remocaoSolicitada(apresentacao::arvore::IdentificadorRegistro _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}
QT_WARNING_POP
