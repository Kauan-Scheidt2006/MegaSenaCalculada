/****************************************************************************
** Meta object code from reading C++ file 'BarraFerramentasArvore.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/interface/componentes/arvore/BarraFerramentasArvore.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BarraFerramentasArvore.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN22BarraFerramentasArvoreE_t {};
} // unnamed namespace

template <> constexpr inline auto BarraFerramentasArvore::qt_create_metaobjectdata<qt_meta_tag_ZN22BarraFerramentasArvoreE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BarraFerramentasArvore",
        "aumentoZoomSolicitado",
        "",
        "reducaoZoomSolicitada",
        "enquadramentoSolicitado",
        "modoArrastoAlterado",
        "ativo"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'aumentoZoomSolicitado'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reducaoZoomSolicitada'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'enquadramentoSolicitado'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'modoArrastoAlterado'
        QtMocHelpers::SignalData<void(bool)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BarraFerramentasArvore, qt_meta_tag_ZN22BarraFerramentasArvoreE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BarraFerramentasArvore::staticMetaObject = { {
    QMetaObject::SuperData::link<Barra::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22BarraFerramentasArvoreE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22BarraFerramentasArvoreE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN22BarraFerramentasArvoreE_t>.metaTypes,
    nullptr
} };

void BarraFerramentasArvore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BarraFerramentasArvore *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->aumentoZoomSolicitado(); break;
        case 1: _t->reducaoZoomSolicitada(); break;
        case 2: _t->enquadramentoSolicitado(); break;
        case 3: _t->modoArrastoAlterado((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BarraFerramentasArvore::*)()>(_a, &BarraFerramentasArvore::aumentoZoomSolicitado, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BarraFerramentasArvore::*)()>(_a, &BarraFerramentasArvore::reducaoZoomSolicitada, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BarraFerramentasArvore::*)()>(_a, &BarraFerramentasArvore::enquadramentoSolicitado, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (BarraFerramentasArvore::*)(bool )>(_a, &BarraFerramentasArvore::modoArrastoAlterado, 3))
            return;
    }
}

const QMetaObject *BarraFerramentasArvore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BarraFerramentasArvore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22BarraFerramentasArvoreE_t>.strings))
        return static_cast<void*>(this);
    return Barra::qt_metacast(_clname);
}

int BarraFerramentasArvore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void BarraFerramentasArvore::aumentoZoomSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BarraFerramentasArvore::reducaoZoomSolicitada()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BarraFerramentasArvore::enquadramentoSolicitado()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BarraFerramentasArvore::modoArrastoAlterado(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
