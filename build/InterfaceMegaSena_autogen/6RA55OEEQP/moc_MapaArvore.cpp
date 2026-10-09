/****************************************************************************
** Meta object code from reading C++ file 'MapaArvore.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/interface/componentes/arvore/MapaArvore.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapaArvore.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MapaArvoreE_t {};
} // unnamed namespace

template <> constexpr inline auto MapaArvore::qt_create_metaobjectdata<qt_meta_tag_ZN10MapaArvoreE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MapaArvore",
        "corAreaVisivel",
        "QColor",
        "corBordaAreaVisivel",
        "larguraBordaAreaVisivel"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'corAreaVisivel'
        QtMocHelpers::PropertyData<QColor>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'corBordaAreaVisivel'
        QtMocHelpers::PropertyData<QColor>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'larguraBordaAreaVisivel'
        QtMocHelpers::PropertyData<qreal>(4, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MapaArvore, qt_meta_tag_ZN10MapaArvoreE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MapaArvore::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MapaArvoreE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MapaArvoreE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MapaArvoreE_t>.metaTypes,
    nullptr
} };

void MapaArvore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MapaArvore *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QColor*>(_v) = _t->corAreaVisivel(); break;
        case 1: *reinterpret_cast<QColor*>(_v) = _t->corBordaAreaVisivel(); break;
        case 2: *reinterpret_cast<qreal*>(_v) = _t->larguraBordaAreaVisivel(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->definirCorAreaVisivel(*reinterpret_cast<QColor*>(_v)); break;
        case 1: _t->definirCorBordaAreaVisivel(*reinterpret_cast<QColor*>(_v)); break;
        case 2: _t->definirLarguraBordaAreaVisivel(*reinterpret_cast<qreal*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *MapaArvore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapaArvore::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MapaArvoreE_t>.strings))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int MapaArvore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
