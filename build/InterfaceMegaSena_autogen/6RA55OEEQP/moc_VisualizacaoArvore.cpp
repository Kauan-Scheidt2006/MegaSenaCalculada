/****************************************************************************
** Meta object code from reading C++ file 'VisualizacaoArvore.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/interface/componentes/arvore/VisualizacaoArvore.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VisualizacaoArvore.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18VisualizacaoArvoreE_t {};
} // unnamed namespace

template <> constexpr inline auto VisualizacaoArvore::qt_create_metaobjectdata<qt_meta_tag_ZN18VisualizacaoArvoreE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "VisualizacaoArvore",
        "registroAtivado",
        "",
        "apresentacao::arvore::IdentificadorRegistro",
        "identificador",
        "zoomAlterado",
        "percentual",
        "viewportAlterado",
        "diametroNo",
        "espacamentoHorizontal",
        "espacamentoVertical",
        "larguraLigacao",
        "larguraBordaNo",
        "larguraBordaNoFoco",
        "corLigacao",
        "QColor",
        "corFundoNo",
        "corBordaNo",
        "corTextoNo",
        "corFundoNoSelecionado",
        "corBordaNoSelecionado",
        "corTextoNoSelecionado",
        "corFundoNoVermelho",
        "corBordaNoVermelho",
        "corTextoNoVermelho",
        "corFundoNoVermelhoSelecionado",
        "corBordaNoVermelhoSelecionado",
        "corTextoNoVermelhoSelecionado",
        "tamanhoFonteNumeroNo",
        "tamanhoFonteFrequenciaNo"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'registroAtivado'
        QtMocHelpers::SignalData<void(apresentacao::arvore::IdentificadorRegistro)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'zoomAlterado'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'viewportAlterado'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'diametroNo'
        QtMocHelpers::PropertyData<qreal>(8, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'espacamentoHorizontal'
        QtMocHelpers::PropertyData<qreal>(9, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'espacamentoVertical'
        QtMocHelpers::PropertyData<qreal>(10, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'larguraLigacao'
        QtMocHelpers::PropertyData<qreal>(11, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'larguraBordaNo'
        QtMocHelpers::PropertyData<qreal>(12, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'larguraBordaNoFoco'
        QtMocHelpers::PropertyData<qreal>(13, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'corLigacao'
        QtMocHelpers::PropertyData<QColor>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corFundoNo'
        QtMocHelpers::PropertyData<QColor>(16, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corBordaNo'
        QtMocHelpers::PropertyData<QColor>(17, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corTextoNo'
        QtMocHelpers::PropertyData<QColor>(18, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corFundoNoSelecionado'
        QtMocHelpers::PropertyData<QColor>(19, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corBordaNoSelecionado'
        QtMocHelpers::PropertyData<QColor>(20, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corTextoNoSelecionado'
        QtMocHelpers::PropertyData<QColor>(21, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corFundoNoVermelho'
        QtMocHelpers::PropertyData<QColor>(22, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corBordaNoVermelho'
        QtMocHelpers::PropertyData<QColor>(23, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corTextoNoVermelho'
        QtMocHelpers::PropertyData<QColor>(24, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corFundoNoVermelhoSelecionado'
        QtMocHelpers::PropertyData<QColor>(25, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corBordaNoVermelhoSelecionado'
        QtMocHelpers::PropertyData<QColor>(26, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corTextoNoVermelhoSelecionado'
        QtMocHelpers::PropertyData<QColor>(27, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'tamanhoFonteNumeroNo'
        QtMocHelpers::PropertyData<qreal>(28, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'tamanhoFonteFrequenciaNo'
        QtMocHelpers::PropertyData<qreal>(29, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<VisualizacaoArvore, qt_meta_tag_ZN18VisualizacaoArvoreE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject VisualizacaoArvore::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18VisualizacaoArvoreE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18VisualizacaoArvoreE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18VisualizacaoArvoreE_t>.metaTypes,
    nullptr
} };

void VisualizacaoArvore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<VisualizacaoArvore *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->registroAtivado((*reinterpret_cast<std::add_pointer_t<apresentacao::arvore::IdentificadorRegistro>>(_a[1]))); break;
        case 1: _t->zoomAlterado((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->viewportAlterado(); break;
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
        if (QtMocHelpers::indexOfMethod<void (VisualizacaoArvore::*)(apresentacao::arvore::IdentificadorRegistro )>(_a, &VisualizacaoArvore::registroAtivado, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (VisualizacaoArvore::*)(int )>(_a, &VisualizacaoArvore::zoomAlterado, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (VisualizacaoArvore::*)()>(_a, &VisualizacaoArvore::viewportAlterado, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<qreal*>(_v) = _t->diametroNo(); break;
        case 1: *reinterpret_cast<qreal*>(_v) = _t->espacamentoHorizontal(); break;
        case 2: *reinterpret_cast<qreal*>(_v) = _t->espacamentoVertical(); break;
        case 3: *reinterpret_cast<qreal*>(_v) = _t->larguraLigacao(); break;
        case 4: *reinterpret_cast<qreal*>(_v) = _t->larguraBordaNo(); break;
        case 5: *reinterpret_cast<qreal*>(_v) = _t->larguraBordaNoFoco(); break;
        case 6: *reinterpret_cast<QColor*>(_v) = _t->corLigacao(); break;
        case 7: *reinterpret_cast<QColor*>(_v) = _t->corFundoNo(); break;
        case 8: *reinterpret_cast<QColor*>(_v) = _t->corBordaNo(); break;
        case 9: *reinterpret_cast<QColor*>(_v) = _t->corTextoNo(); break;
        case 10: *reinterpret_cast<QColor*>(_v) = _t->corFundoNoSelecionado(); break;
        case 11: *reinterpret_cast<QColor*>(_v) = _t->corBordaNoSelecionado(); break;
        case 12: *reinterpret_cast<QColor*>(_v) = _t->corTextoNoSelecionado(); break;
        case 13: *reinterpret_cast<QColor*>(_v) = _t->corFundoNoVermelho(); break;
        case 14: *reinterpret_cast<QColor*>(_v) = _t->corBordaNoVermelho(); break;
        case 15: *reinterpret_cast<QColor*>(_v) = _t->corTextoNoVermelho(); break;
        case 16: *reinterpret_cast<QColor*>(_v) = _t->corFundoNoVermelhoSelecionado(); break;
        case 17: *reinterpret_cast<QColor*>(_v) = _t->corBordaNoVermelhoSelecionado(); break;
        case 18: *reinterpret_cast<QColor*>(_v) = _t->corTextoNoVermelhoSelecionado(); break;
        case 19: *reinterpret_cast<qreal*>(_v) = _t->tamanhoFonteNumeroNo(); break;
        case 20: *reinterpret_cast<qreal*>(_v) = _t->tamanhoFonteFrequenciaNo(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->definirDiametroNo(*reinterpret_cast<qreal*>(_v)); break;
        case 1: _t->definirEspacamentoHorizontal(*reinterpret_cast<qreal*>(_v)); break;
        case 2: _t->definirEspacamentoVertical(*reinterpret_cast<qreal*>(_v)); break;
        case 3: _t->definirLarguraLigacao(*reinterpret_cast<qreal*>(_v)); break;
        case 4: _t->definirLarguraBordaNo(*reinterpret_cast<qreal*>(_v)); break;
        case 5: _t->definirLarguraBordaNoFoco(*reinterpret_cast<qreal*>(_v)); break;
        case 6: _t->definirCorLigacao(*reinterpret_cast<QColor*>(_v)); break;
        case 7: _t->definirCorFundoNo(*reinterpret_cast<QColor*>(_v)); break;
        case 8: _t->definirCorBordaNo(*reinterpret_cast<QColor*>(_v)); break;
        case 9: _t->definirCorTextoNo(*reinterpret_cast<QColor*>(_v)); break;
        case 10: _t->definirCorFundoNoSelecionado(*reinterpret_cast<QColor*>(_v)); break;
        case 11: _t->definirCorBordaNoSelecionado(*reinterpret_cast<QColor*>(_v)); break;
        case 12: _t->definirCorTextoNoSelecionado(*reinterpret_cast<QColor*>(_v)); break;
        case 13: _t->definirCorFundoNoVermelho(*reinterpret_cast<QColor*>(_v)); break;
        case 14: _t->definirCorBordaNoVermelho(*reinterpret_cast<QColor*>(_v)); break;
        case 15: _t->definirCorTextoNoVermelho(*reinterpret_cast<QColor*>(_v)); break;
        case 16: _t->definirCorFundoNoVermelhoSelecionado(*reinterpret_cast<QColor*>(_v)); break;
        case 17: _t->definirCorBordaNoVermelhoSelecionado(*reinterpret_cast<QColor*>(_v)); break;
        case 18: _t->definirCorTextoNoVermelhoSelecionado(*reinterpret_cast<QColor*>(_v)); break;
        case 19: _t->definirTamanhoFonteNumeroNo(*reinterpret_cast<qreal*>(_v)); break;
        case 20: _t->definirTamanhoFonteFrequenciaNo(*reinterpret_cast<qreal*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *VisualizacaoArvore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VisualizacaoArvore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18VisualizacaoArvoreE_t>.strings))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int VisualizacaoArvore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void VisualizacaoArvore::registroAtivado(apresentacao::arvore::IdentificadorRegistro _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void VisualizacaoArvore::zoomAlterado(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void VisualizacaoArvore::viewportAlterado()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
