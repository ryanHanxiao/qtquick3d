/****************************************************************************
**
** Copyright (C) 2008-2012 NVIDIA Corporation.
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "qssgrenderray_p.h"

#include <QtQuick3DUtils/private/qssgplane_p.h>
#include <QtQuick3DUtils/private/qssgutils_p.h>

QT_BEGIN_NAMESPACE

// http://www.siggraph.org/education/materials/HyperGraph/raytrace/rayplane_intersection.htm

QSSGOption<QVector3D> QSSGRenderRay::intersect(const QSSGPlane &inPlane, const QSSGRenderRay &ray)
{
    float Vd = QVector3D::dotProduct(inPlane.n, ray.direction);
    if (std::abs(Vd) < .0001f)
        return QSSGEmpty();
    float V0 = -1.0f * (QVector3D::dotProduct(inPlane.n, ray.origin) + inPlane.d);
    float t = V0 / Vd;
    return ray.origin + (ray.direction * t);
}

QSSGRenderRay::RayData QSSGRenderRay::createRayData(const QMatrix4x4 &globalTransform,
                                                    const QSSGRenderRay &ray)
{
    using DirectionOp = RayData::DirectionOp;
    QMatrix4x4 originTransform = globalTransform.inverted();
    QVector3D transformedOrigin = mat44::transform(originTransform, ray.origin);
    float *outOriginTransformPtr(originTransform.data());
    outOriginTransformPtr[12] = outOriginTransformPtr[13] = outOriginTransformPtr[14] = 0.0f;
    const QVector3D &transformedDirection = mat44::rotate(originTransform, ray.direction);
    static auto getInverseAndDirOp = [](const QVector3D &dir, QVector3D &invDir, DirectionOp (&dirOp)[3]) {
        for (int i = 0; i != 3; ++i) {
            const float axisDir = dir[i];
            dirOp[i] = qFuzzyIsNull(axisDir) ? DirectionOp::Zero : ((axisDir < -std::numeric_limits<float>::epsilon())
                                                                    ? DirectionOp::Swap
                                                                    : DirectionOp::Normal);
            invDir[i] = qFuzzyIsNull(axisDir) ? 0.0f : (1.0f / axisDir);
        }
    };
    DirectionOp dirOp[3];
    QVector3D transformedDirectionInvers;
    getInverseAndDirOp(transformedDirection, transformedDirectionInvers, dirOp);
    return RayData{ globalTransform, ray, transformedOrigin, transformedDirectionInvers,
                    transformedDirection, { dirOp[0], dirOp[1], dirOp[2] } };
}

QSSGRenderRay::IntersectionResult QSSGRenderRay::createIntersectionResult(const QSSGRenderRay::RayData &data,
                                                                          const HitResult &hit)
{
    Q_ASSERT(hit.intersects());
    Q_ASSERT(hit.bounds != nullptr);
    const QSSGBounds3 &bounds = *hit.bounds;
    // Scene postion
    const QVector3D &scaledDir = data.direction * hit.min;
    const QVector3D &scenePosition = scaledDir + data.origin;
    // ray length squared
    const QVector3D &globalPosition = mat44::transform(data.globalTransform, scenePosition);
    const QVector3D &cameraToLocal = data.ray.origin - globalPosition;
    const float rayLenSquared = vec3::magnitudeSquared(cameraToLocal);
    // UV coordinates
    const auto &boundsMin = bounds.minimum;
    const auto &boundsMax = bounds.maximum;
    const float xRange = boundsMax.x() - boundsMin.x();
    const float yRange = boundsMax.y() - boundsMin.y();
    const QVector2D uvCoords{((scenePosition[0] - boundsMin.x()) / xRange), ((scenePosition[1] - boundsMin.y()) / yRange)};

    return IntersectionResult(rayLenSquared, uvCoords, scenePosition);
}

QSSGRenderRay::HitResult QSSGRenderRay::intersectWithAABBv2(const QSSGRenderRay::RayData &data,
                                                            const QSSGBounds3 &bounds)
{
    // Intersect the origin with the AABB described by bounds.

    // Scan each axis separately.  This code basically finds the distance
    // from the origin to the near and far bbox planes for a given
    // axis.  It then divides this distance by the direction for that axis to
    // get a range of t [near,far] that the ray intersects assuming the ray is
    // described via origin + t*(direction).  Running through all three axis means
    // that you need to min/max those ranges together to find a global min/max
    // that the pick could possibly be in.
    float tmax = std::numeric_limits<float>::max();
    float tmin = std::numeric_limits<float>::min();
    float origin;
    const QVector3D *const barray[] { &bounds.minimum, &bounds.maximum };

    for (int axis = 0; axis != 3; ++axis) {
        origin = data.origin[axis];
        const bool zeroDir = (data.dirOp[axis] == RayData::DirectionOp::Zero);
        if (zeroDir && (origin < bounds.minimum[axis] || origin > bounds.maximum[axis])) {
            // Pickray is roughly parallel to the plane of the slab
            // so, if the origin is not in the range, we have no intersection
            return { -1.0f, -1.0f, nullptr };
        }
        if (!zeroDir) {
            // Shrink the intersections to find the closest hit
            tmax = std::min(((*barray[1-quint8(data.dirOp[axis])])[axis] - origin) * data.directionInvers[axis], tmax);
            tmin = std::max(((*barray[quint8(data.dirOp[axis])])[axis] - origin) * data.directionInvers[axis], tmin);
        }
    }

    return { tmin, tmax, &bounds };
}

QSSGRenderRay::IntersectionResult QSSGRenderRay::intersectWithAABB(const QMatrix4x4 &inGlobalTransform,
                                                                   const QSSGBounds3 &inBounds,
                                                                   const QSSGRenderRay &ray,
                                                                   bool inForceIntersect)
{
    // Intersect the origin with the AABB described by bounds.

    // Scan each axis separately.  This code basically finds the distance
    // distance from the origin to the near and far bbox planes for a given
    // axis.  It then divides this distance by the direction for that axis to
    // get a range of t [near,far] that the ray intersects assuming the ray is
    // described via origin + t*(direction).  Running through all three axis means
    // that you need to min/max those ranges together to find a global min/max
    // that the pick could possibly be in.

    // Transform pick origin and direction into the subset's space.
    QMatrix4x4 theOriginTransform = inGlobalTransform.inverted();

    QVector3D theTransformedOrigin = mat44::transform(theOriginTransform, ray.origin);
    float *outOriginTransformPtr(theOriginTransform.data());
    outOriginTransformPtr[12] = outOriginTransformPtr[13] = outOriginTransformPtr[14] = 0.0f;

    QVector3D theTransformedDirection = mat44::rotate(theOriginTransform, ray.direction);

    static const float KD_FLT_MAX = 3.40282346638528860e+38;
    static const float kEpsilon = 1e-5f;

    float theMinWinner = -KD_FLT_MAX;
    float theMaxWinner = KD_FLT_MAX;

    for (quint32 theAxis = 0; theAxis < 3; ++theAxis) {
        // Extract the ranges and direction for this axis
        float theMinBox = inBounds.minimum[theAxis];
        float theMaxBox = inBounds.maximum[theAxis];
        float theDirectionAxis = theTransformedDirection[theAxis];
        float theOriginAxis = theTransformedOrigin[theAxis];

        float theMinAxis = -KD_FLT_MAX;
        float theMaxAxis = KD_FLT_MAX;
        if (theDirectionAxis > kEpsilon) {
            theMinAxis = (theMinBox - theOriginAxis) / theDirectionAxis;
            theMaxAxis = (theMaxBox - theOriginAxis) / theDirectionAxis;
        } else if (theDirectionAxis < -kEpsilon) {
            theMinAxis = (theMaxBox - theOriginAxis) / theDirectionAxis;
            theMaxAxis = (theMinBox - theOriginAxis) / theDirectionAxis;
        } else if ((theOriginAxis < theMinBox || theOriginAxis > theMaxBox) && !inForceIntersect) {
            // Pickray is roughly parallel to the plane of the slab
            // so, if the origin is not in the range, we have no intersection
            return IntersectionResult();
        }

        // Shrink the intersections to find the closest hit
        theMinWinner = qMax(theMinWinner, theMinAxis);
        theMaxWinner = qMin(theMaxWinner, theMaxAxis);

        if ((theMinWinner > theMaxWinner || theMaxWinner < 0) && !inForceIntersect)
            return IntersectionResult();
    }

    QVector3D scaledDir = theTransformedDirection * theMinWinner;
    QVector3D newPosInLocal = theTransformedOrigin + scaledDir;
    QVector3D newPosInGlobal = mat44::transform(inGlobalTransform, newPosInLocal);
    QVector3D cameraToLocal = ray.origin - newPosInGlobal;

    float rayLengthSquared = vec3::magnitudeSquared(cameraToLocal);

    float xRange = inBounds.maximum.x() - inBounds.minimum.x();
    float yRange = inBounds.maximum.y() - inBounds.minimum.y();

    QVector2D relXY;
    relXY.setX((newPosInLocal[0] - inBounds.minimum.x()) / xRange);
    relXY.setY((newPosInLocal[1] - inBounds.minimum.y()) / yRange);

    return IntersectionResult(rayLengthSquared, relXY, newPosInGlobal);
}

QSSGOption<QVector2D> QSSGRenderRay::relative(const QMatrix4x4 &inGlobalTransform,
                                                     const QSSGBounds3 &inBounds,
                                                     QSSGRenderBasisPlanes inPlane) const
{
    QMatrix4x4 theOriginTransform = inGlobalTransform.inverted();

    QVector3D theTransformedOrigin = mat44::transform(theOriginTransform, origin);
    float *outOriginTransformPtr(theOriginTransform.data());
    outOriginTransformPtr[12] = outOriginTransformPtr[13] = outOriginTransformPtr[14] = 0.0f;
    QVector3D theTransformedDirection = mat44::rotate(theOriginTransform, direction);

    // The XY plane is going to be a plane with either positive or negative Z direction that runs
    // through
    QVector3D theDirection(0, 0, 1);
    QVector3D theRight(1, 0, 0);
    QVector3D theUp(0, 1, 0);
    switch (inPlane) {
    case QSSGRenderBasisPlanes::XY:
        break;
    case QSSGRenderBasisPlanes::XZ:
        theDirection = QVector3D(0, 1, 0);
        theUp = QVector3D(0, 0, 1);
        break;
    case QSSGRenderBasisPlanes::YZ:
        theDirection = QVector3D(1, 0, 0);
        theRight = QVector3D(0, 0, 1);
        break;
    }
    QSSGPlane thePlane(theDirection,
                         QVector3D::dotProduct(theDirection, theTransformedDirection) > 0.0f
                                 ? QVector3D::dotProduct(theDirection, inBounds.maximum)
                                 : QVector3D::dotProduct(theDirection, inBounds.minimum));

    const QSSGRenderRay relativeRay(theTransformedOrigin, theTransformedDirection);
    QSSGOption<QVector3D> localIsect = QSSGRenderRay::intersect(thePlane, relativeRay);
    if (localIsect.hasValue()) {
        float xRange = QVector3D::dotProduct(theRight, inBounds.maximum) - QVector3D::dotProduct(theRight, inBounds.minimum);
        float yRange = QVector3D::dotProduct(theUp, inBounds.maximum) - QVector3D::dotProduct(theUp, inBounds.minimum);
        float xOrigin = xRange / 2.0f + QVector3D::dotProduct(theRight, inBounds.minimum);
        float yOrigin = yRange / 2.0f + QVector3D::dotProduct(theUp, inBounds.minimum);
        return QVector2D((QVector3D::dotProduct(theRight, *localIsect) - xOrigin) / xRange,
                         (QVector3D::dotProduct(theUp, *localIsect) - yOrigin) / yRange);
    }
    return QSSGEmpty();
}

QT_END_NAMESPACE
