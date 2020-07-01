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

/* clang-format off */

#include <QtQuick3DRuntimeRender/private/qssgshadermaterialadapter_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendercontextcore_p.h>

QT_BEGIN_NAMESPACE

QSSGShaderMaterialAdapter::~QSSGShaderMaterialAdapter() = default;

QSSGShaderMaterialAdapter *QSSGShaderMaterialAdapter::create(const QSSGRenderGraphObject &materialNode)
{
    switch (materialNode.type) {
    case QSSGRenderGraphObject::Type::DefaultMaterial:
    case QSSGRenderGraphObject::Type::PrincipledMaterial:
        return new QSSGShaderDefaultMaterialAdapter(static_cast<const QSSGRenderDefaultMaterial &>(materialNode));

    case QSSGRenderGraphObject::Type::CustomMaterial:
        return new QSSGShaderCustomMaterialAdapter(static_cast<const QSSGRenderCustomMaterial &>(materialNode));

    default:
        break;
    }

    return nullptr;
}

bool QSSGShaderMaterialAdapter::isUnshaded()
{
    return false;
}

bool QSSGShaderMaterialAdapter::hasCustomShaderSnippet(QSSGShaderCache::ShaderType)
{
    return false;
}

QByteArray QSSGShaderMaterialAdapter::customShaderSnippet(QSSGShaderCache::ShaderType,
                                                          const QSSGRenderContextInterface &)
{
    return QByteArray();
}

void QSSGShaderMaterialAdapter::setCustomPropertyUniforms(QSSGRef<QSSGRhiShaderStagesWithResources> &,
                                                          const QSSGRenderContextInterface &)
{
}




QSSGShaderDefaultMaterialAdapter::QSSGShaderDefaultMaterialAdapter(const QSSGRenderDefaultMaterial &material)
    : m_material(material)
{
}

bool QSSGShaderDefaultMaterialAdapter::isPrincipled()
{
    return m_material.type == QSSGRenderGraphObject::Type::PrincipledMaterial;
}

bool QSSGShaderDefaultMaterialAdapter::isMetalnessEnabled()
{
    return m_material.isMetalnessEnabled();
}

bool QSSGShaderDefaultMaterialAdapter::isSpecularEnabled()
{
    return m_material.isSpecularEnabled();
}

bool QSSGShaderDefaultMaterialAdapter::isVertexColorsEnabled()
{
    return m_material.isVertexColorsEnabled();
}

bool QSSGShaderDefaultMaterialAdapter::hasLighting()
{
    return m_material.hasLighting();
}

QSSGRenderDefaultMaterial::MaterialSpecularModel QSSGShaderDefaultMaterialAdapter::specularModel()
{
    return m_material.specularModel;
}

QSSGRenderDefaultMaterial::MaterialAlphaMode QSSGShaderDefaultMaterialAdapter::alphaMode()
{
    return m_material.alphaMode;
}

QSSGRenderImage *QSSGShaderDefaultMaterialAdapter::iblProbe()
{
    return m_material.iblProbe;
}

QVector3D QSSGShaderDefaultMaterialAdapter::emissiveColor()
{
    return m_material.emissiveColor;
}

QVector4D QSSGShaderDefaultMaterialAdapter::color()
{
    return m_material.color;
}

QVector3D QSSGShaderDefaultMaterialAdapter::specularTint()
{
    return m_material.specularTint;
}

float QSSGShaderDefaultMaterialAdapter::ior()
{
    return m_material.ior;
}

float QSSGShaderDefaultMaterialAdapter::fresnelPower()
{
    return m_material.fresnelPower;
}

float QSSGShaderDefaultMaterialAdapter::metalnessAmount()
{
    return m_material.metalnessAmount;
}

float QSSGShaderDefaultMaterialAdapter::specularAmount()
{
    return m_material.specularAmount;
}

float QSSGShaderDefaultMaterialAdapter::specularRoughness()
{
    return m_material.specularRoughness;
}

float QSSGShaderDefaultMaterialAdapter::bumpAmount()
{
    return m_material.bumpAmount;
}

float QSSGShaderDefaultMaterialAdapter::translucentFallOff()
{
    return m_material.translucentFalloff;
}

float QSSGShaderDefaultMaterialAdapter::diffuseLightWrap()
{
    return m_material.diffuseLightWrap;
}

float QSSGShaderDefaultMaterialAdapter::occlusionAmount()
{
    return m_material.occlusionAmount;
}

float QSSGShaderDefaultMaterialAdapter::alphaCutOff()
{
    return m_material.alphaCutoff;
}



QSSGShaderCustomMaterialAdapter::QSSGShaderCustomMaterialAdapter(const QSSGRenderCustomMaterial &material)
    : m_material(material)
{
}

bool QSSGShaderCustomMaterialAdapter::isPrincipled()
{
    return false;
}

bool QSSGShaderCustomMaterialAdapter::isMetalnessEnabled()
{
    return false;
}

bool QSSGShaderCustomMaterialAdapter::isSpecularEnabled()
{
    return false;
}

bool QSSGShaderCustomMaterialAdapter::isVertexColorsEnabled()
{
    return false;
}

bool QSSGShaderCustomMaterialAdapter::hasLighting()
{
    return true;
}

QSSGRenderDefaultMaterial::MaterialSpecularModel QSSGShaderCustomMaterialAdapter::specularModel()
{
    return QSSGRenderDefaultMaterial::MaterialSpecularModel::Default;
}

QSSGRenderDefaultMaterial::MaterialAlphaMode QSSGShaderCustomMaterialAdapter::alphaMode()
{
    return QSSGRenderDefaultMaterial::MaterialAlphaMode::Default;
}

QSSGRenderImage *QSSGShaderCustomMaterialAdapter::iblProbe()
{
    return m_material.m_iblProbe;
}

QVector3D QSSGShaderCustomMaterialAdapter::emissiveColor()
{
    return QVector3D(0, 0, 0);
}

QVector4D QSSGShaderCustomMaterialAdapter::color()
{
    return QVector4D(1, 1, 1, 1);
}

QVector3D QSSGShaderCustomMaterialAdapter::specularTint()
{
    return QVector3D(1, 1, 1);
}

float QSSGShaderCustomMaterialAdapter::ior()
{
    return 1.45f;
}

float QSSGShaderCustomMaterialAdapter::fresnelPower()
{
    return 0.0f;
}

float QSSGShaderCustomMaterialAdapter::metalnessAmount()
{
    return 0.0f;
}

float QSSGShaderCustomMaterialAdapter::specularAmount()
{
    return 0.0f;
}

float QSSGShaderCustomMaterialAdapter::specularRoughness()
{
    return 50.0f;
}

float QSSGShaderCustomMaterialAdapter::bumpAmount()
{
    return 0.0f;
}

float QSSGShaderCustomMaterialAdapter::translucentFallOff()
{
    return 0.0f;
}

float QSSGShaderCustomMaterialAdapter::diffuseLightWrap()
{
    return 0.0f;
}

float QSSGShaderCustomMaterialAdapter::occlusionAmount()
{
    return 1.0f;
}

float QSSGShaderCustomMaterialAdapter::alphaCutOff()
{
    return 0.5f;
}

bool QSSGShaderCustomMaterialAdapter::isUnshaded()
{
    return m_material.m_shadingMode == QSSGRenderCustomMaterial::ShadingMode::Unshaded;
}

bool QSSGShaderCustomMaterialAdapter::hasCustomShaderSnippet(QSSGShaderCache::ShaderType type)
{
    if (type == QSSGShaderCache::ShaderType::Vertex)
        return m_material.m_customShaderPresence.testFlag(QSSGRenderCustomMaterial::CustomShaderPresenceFlag::Vertex);

    return m_material.m_customShaderPresence.testFlag(QSSGRenderCustomMaterial::CustomShaderPresenceFlag::Fragment);
}

QByteArray QSSGShaderCustomMaterialAdapter::customShaderSnippet(QSSGShaderCache::ShaderType type,
                                                                const QSSGRenderContextInterface &context)
{
    // ###
    if (m_material.m_shadingMode == QSSGRenderCustomMaterial::ShadingMode::Unshaded)
        return context.shaderLibraryManager()->getShaderSource(m_material.m_shaderPathKey, type);

    return QByteArray();
}

void QSSGShaderCustomMaterialAdapter::setCustomPropertyUniforms(QSSGRef<QSSGRhiShaderStagesWithResources> &shaderPipeline,
                                                                const QSSGRenderContextInterface &context)
{
    context.customMaterialSystem()->applyRhiShaderPropertyValues(m_material, shaderPipeline);
}

namespace {

// Custom material shader substitution table.
// Must be in sync with the shader generator.
// Watch out for the order. (long names first, to avoid clash)
static std::vector<QSSGCustomMaterialVariableSubstitution> qssg_var_subst_tab = {
    // uniform (block members)
    { "MODELVIEWPROJECTION_MATRIX", "modelViewProjection" },
    { "VIEWPROJECTION_MATRIX", "viewProjectionMatrix" },
    { "MODEL_MATRIX", "modelMatrix" },
    { "VIEW_MATRIX", "viewMatrix" },
    { "NORMAL_MATRIX", "normalMatrix"},
    { "CAMERA_POSITION", "cameraPosition" },
    { "CAMERA_DIRECTION", "cameraDirection" },

    // vertex inputs
    { "VERTEX", "attr_pos" },
    { "NORMAL", "attr_norm" },
    { "UV0", "attr_uv0" },
    { "UV1", "attr_uv1" },

    // outputs
    { "POSITION", "gl_Position" },
    { "FRAGCOLOR", "fragOutput" },
};

// This is based on the Qt Quick shader rewriter (with fixes)
struct Tokenizer {
    enum Token {
        Token_Comment,
        Token_OpenBrace,
        Token_CloseBrace,
        Token_SemiColon,
        Token_Identifier,
        Token_Macro,
        Token_Unspecified,

        Token_EOF
    };

    void initialize(const QByteArray &input);
    Token next();

    const char *stream;
    const char *pos;
    const char *identifier;
};

void Tokenizer::initialize(const QByteArray &input)
{
    stream = input.constData();
    pos = input;
    identifier = input;
}

Tokenizer::Token Tokenizer::next()
{
    while (*pos) {
        char c = *pos++;
        switch (c) {
        case '/':
            if (*pos == '/') {
                // '//' comment
                ++pos;
                while (*pos && *pos != '\n') ++pos;
                if (*pos) ++pos; // skip the newline
                return Token_Comment;
            } else if (*pos == '*') {
                // /* */ comment
                ++pos;
                while (*pos && (*pos != '*' || pos[1] != '/')) ++pos;
                if (*pos) pos += 2;
                return Token_Comment;
            }
            break;

        case '#': {
            while (*pos) {
                if (*pos == '\n') {
                    ++pos;
                    break;
                } else if (*pos == '\\') {
                    ++pos;
                    while (*pos && (*pos == ' ' || *pos == '\t'))
                        ++pos;
                    if (*pos && (*pos == '\n' || (*pos == '\r' && pos[1] == '\n')))
                        pos += 2;
                } else {
                    ++pos;
                }
            }
            break;
        }

        case ';': return Token_SemiColon;
        case '\0': return Token_EOF;
        case '{': return Token_OpenBrace;
        case '}': return Token_CloseBrace;

        case ' ':
        case '\n':
        case '\r': break;
        default:
            // Identifier...
            if ((c >= 'a' && c <= 'z' ) || (c >= 'A' && c <= 'Z' ) || c == '_') {
                identifier = pos - 1;
                while (*pos && ((*pos >= 'a' && *pos <= 'z')
                                     || (*pos >= 'A' && *pos <= 'Z')
                                     || *pos == '_'
                                     || (*pos >= '0' && *pos <= '9'))) {
                    ++pos;
                }
                return Token_Identifier;
            } else {
                return Token_Unspecified;
            }
        }
    }

    return Token_EOF;
}
} // namespace

QByteArray QSSGShaderCustomMaterialAdapter::prepareCustomShader(QByteArray &dst,
                                                                const QByteArray &shaderCode,
                                                                QSSGShaderCache::ShaderType type,
                                                                const UniformList &uniforms)
{
    QByteArrayList inputs;
    QByteArrayList outputs;

    Tokenizer tok;
    tok.initialize(shaderCode);

    QByteArray result;
    result.reserve(1024);
    const char *lastPos = shaderCode.constData();

    Tokenizer::Token t = tok.next();
    while (t != Tokenizer::Token_EOF) {
        switch (t) {
        case Tokenizer::Token_Identifier:
        {
            QByteArray id = QByteArray::fromRawData(lastPos, tok.pos - lastPos);
            if (id.trimmed() == QByteArrayLiteral("VARYING")) {
                QByteArray vtype;
                QByteArray vname;
                lastPos = tok.pos;
                t = tok.next();
                while (t != Tokenizer::Token_EOF) {
                    QByteArray data = QByteArray::fromRawData(lastPos, tok.pos - lastPos);
                    if (t == Tokenizer::Token_Identifier) {
                        if (vtype.isEmpty())
                            vtype = data.trimmed();
                        else if (vname.isEmpty())
                            vname = data.trimmed();
                    }
                    if (t == Tokenizer::Token_SemiColon)
                        break;
                    lastPos = tok.pos;
                    t = tok.next();
                }
                if (type == QSSGShaderCache::ShaderType::Vertex)
                    outputs.append(vtype + " " + vname);
                else
                    inputs.append(vtype + " " + vname);
            } else {
                for (const QSSGCustomMaterialVariableSubstitution &subst : qssg_var_subst_tab)
                    id.replace(subst.builtin, subst.actualName);
                result += id;
            }
        }
            break;
        default:
            if (t != Tokenizer::Token_Comment)
                result += QByteArray::fromRawData(lastPos, tok.pos - lastPos);
            break;
        }
        lastPos = tok.pos;
        t = tok.next();
    }

    result += '\n';

    static const char *metaStart = "#ifdef QQ3D_SHADER_META\n/*{\n  \"uniforms\": [\n";
    static const char *metaEnd = "  ]\n}*/\n#endif\n";
    dst.append(metaStart);
    for (int i = 0, count = uniforms.count(); i < count; ++i) {
        const auto &typeAndName(uniforms[i]);
        dst.append("    { \"type\": \"" + typeAndName.first + "\", \"name\": \"" + typeAndName.second + "\" }");
        if (i < count - 1)
            dst.append(",");
        dst.append("\n");
    }
    dst.append(metaEnd);

    if (!inputs.isEmpty()) {
        static const char *metaStart = "#ifdef QQ3D_SHADER_META\n/*{\n  \"inputs\": [\n";
        static const char *metaEnd = "  ]\n}*/\n#endif\n";
        dst.append(metaStart);
        for (int i = 0, count = inputs.count(); i < count; ++i) {
            const QByteArrayList typeAndName = inputs[i].split(' ');
            if (typeAndName.count() != 2)
                continue;
            dst.append("    { \"type\": \"" + typeAndName[0].trimmed()
                    + "\", \"name\": \"" + typeAndName[1].trimmed()
                    + "\", \"stage\": \"fragment"
                    + "\" }");
            if (i < count - 1)
                dst.append(",");
            dst.append("\n");
        }
        dst.append(metaEnd);
    }

    if (!outputs.isEmpty()) {
        static const char *metaStart = "#ifdef QQ3D_SHADER_META\n/*{\n  \"outputs\": [\n";
        static const char *metaEnd = "  ]\n}*/\n#endif\n";
        dst.append(metaStart);
        for (int i = 0, count = outputs.count(); i < count; ++i) {
            const QByteArrayList typeAndName = outputs[i].split(' ');
            if (typeAndName.count() != 2)
                continue;
            dst.append("    { \"type\": \"" + typeAndName[0].trimmed()
                    + "\", \"name\": \"" + typeAndName[1].trimmed()
                    + "\", \"stage\": \"vertex"
                    + "\" }");
            if (i < count - 1)
                dst.append(",");
            dst.append("\n");
        }
        dst.append(metaEnd);
    }

    return result;
}

QT_END_NAMESPACE
