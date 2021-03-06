/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick 3D.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qquick3dmorphtarget_p.h"

#include <QtQuick3DRuntimeRender/private/qssgrendermorphtarget_p.h>
#include <QtQml/QQmlFile>

QT_BEGIN_NAMESPACE

/*!
    \qmltype MorphTarget
    \inherits Object
    \inqmlmodule QtQuick3D
    \brief Define a morph target's properteis.

    Each \e MorphTarget is a morph target for a vertex animation.

    \qml
    MorphTarget {
        id: morphtarget0
        attributes: MorphTarget.Position | MorphTarget.Normal
        weight: 0.5
    }
    \endqml
*/

QQuick3DMorphTarget::QQuick3DMorphTarget(QQuick3DObject *parent)
    : QQuick3DObject(*(new QQuick3DObjectPrivate(QQuick3DObjectPrivate::Type::MorphTarget)), parent) {}

QQuick3DMorphTarget::~QQuick3DMorphTarget()
{
}

/*!
    \qmlproperty float MorphTarget::weight

    Specifies the weight of the current morph target.
*/

float QQuick3DMorphTarget::weight() const
{
    return m_weight;
}

/*!
    \qmlproperty enumeration MorphTarget::attributes

    Specifies the set of attributes of the current morph target.
    In order to animate some of vertex attribues in morphing, a mesh should
    have the target attribute inputs and a morph target should have that
    attributes.

    Following values can be specified by OR-ing together.
    \value MorphTarget.Position animates model's position
    \value MorphTarget.Normal animates model's normal
    \value MorphTarget.Tangent animates model's tangent
    \value MorphTarget.Binormal animates model's binormal
*/

QQuick3DMorphTarget::MorphTargetAttributes QQuick3DMorphTarget::attributes() const
{
    return m_attributes;
}

void QQuick3DMorphTarget::markAllDirty()
{
    m_dirtyAttributes = 0xffffffff;
    QQuick3DObject::markAllDirty();
}

void QQuick3DMorphTarget::setWeight(float weight)
{
    if (m_weight == weight)
        return;

    m_weight = weight;
    emit weightChanged();
    markDirty(WeightDirty);
}

void QQuick3DMorphTarget::setAttributes(MorphTargetAttributes attributes)
{
    if (m_attributes == attributes)
        return;

    m_attributes = attributes;
    m_numAttribs = 0;
    int flags = attributes;
    while (flags) {
        m_numAttribs += flags & 0x1;
        flags >>= 1;
    }
    emit attributesChanged();
    markDirty(MorphTargetAttributesDirty);
}

QSSGRenderGraphObject *QQuick3DMorphTarget::updateSpatialNode(QSSGRenderGraphObject *node)
{
    if (!node) {
        markAllDirty();
        node = new QSSGRenderMorphTarget();
    }

    auto modelNode = static_cast<QSSGRenderMorphTarget *>(node);
    if (m_dirtyAttributes & WeightDirty)
        modelNode->weight = m_weight;
    if (m_dirtyAttributes & MorphTargetAttributesDirty)
        modelNode->attributes = QSSGRenderMorphTarget::InputAttributes(int(m_attributes));

    m_dirtyAttributes = 0;

    return modelNode;
}

void QQuick3DMorphTarget::markDirty(QQuick3DMorphTarget::QSSGMorphTargetDirtyType type)
{
    if (!(m_dirtyAttributes & quint32(type))) {
        m_dirtyAttributes |= quint32(type);
        update();
    }
}

size_t QQuick3DMorphTarget::numAttribs()
{
    return m_numAttribs;
}

QT_END_NAMESPACE
