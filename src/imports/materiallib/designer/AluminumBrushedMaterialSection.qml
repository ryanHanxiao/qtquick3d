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

import QtQuick 2.15
import HelperWidgets 2.0
import QtQuick.Layouts 1.12

Column {
    id: materialRoot
    width: parent.width

    property int labelWidth: 10
    property int labelSpinBoxSpacing: 0
    property int spinBoxMinimumWidth: 120

    Section {
        caption: qsTr("Environment Map")
        width: parent.width

        SectionLayout {
            Label {
                text: qsTr("Enabled")
                tooltip: qsTr("Specifies if the environment map is enabled.")
            }
            SecondColumnLayout {
                CheckBox {
                    text: backendValues.uEnvironmentMappingEnabled.valueToString
                    backendValue: backendValues.uEnvironmentMappingEnabled
                    Layout.fillWidth: true
                }
            }

// TODO: Proper support for TextureInput properties
//            Label {
//                text: qsTr("Texture")
//                tooltip: qsTr("Defines a texture for environment map.")
//            }
//            SecondColumnLayout {
//                IdComboBox {
//                    typeFilter: "QtQuick3D.Texture"
//                    Layout.fillWidth: true
//                    backendValue: backendValues.uEnvironmentTexture.texture
//                }
//            }
        }
    }

    Section {
        caption: qsTr("Shadow Map")
        width: parent.width

        SectionLayout {
            Label {
                text: qsTr("Enabled")
                tooltip: qsTr("Specifies if the shadow map is enabled.")
            }
            SecondColumnLayout {
                CheckBox {
                    text: backendValues.uShadowMappingEnabled.valueToString
                    backendValue: backendValues.uShadowMappingEnabled
                    Layout.fillWidth: true
                }
            }

// TODO: Proper support for TextureInput properties
//            Label {
//                text: qsTr("Texture")
//                tooltip: qsTr("Defines a texture for shadow map.")
//            }
//            SecondColumnLayout {
//                IdComboBox {
//                    typeFilter: "QtQuick3D.Texture"
//                    Layout.fillWidth: true
//                    backendValue: backendValues.uBakedShadowTexture.texture
//                }
//            }
        }
    }

    Section {
        caption: qsTr("Metal Color")
        width: parent.width
        ColorEditor {
            caption: qsTr("Metal Color")
            backendValue: backendValues.metal_color
            supportGradient: false
            isVector3D: true
            Layout.fillWidth: true
        }
    }

//    Section {
//        caption: qsTr("Roughness")
//        width: parent.width
//        SectionLayout {
// TODO: Proper support for TextureInput properties
//            Label {
//                text: qsTr("Texture")
//                tooltip: qsTr("Defines a horizontal texture for roughness map.")
//            }
//            SecondColumnLayout {
//                IdComboBox {
//                    typeFilter: "QtQuick3D.Texture"
//                    Layout.fillWidth: true
//                    backendValue: backendValues.roughness_texture_u.texture
//                }
//            }
// TODO: Proper support for TextureInput properties
//            Label {
//                text: qsTr("Texture")
//                tooltip: qsTr("Defines a vertical texture for roughness map.")
//            }
//            SecondColumnLayout {
//                IdComboBox {
//                    typeFilter: "QtQuick3D.Texture"
//                    Layout.fillWidth: true
//                    backendValue: backendValues.roughness_texture_v.texture
//                }
//            }
//        }
//    }
    Section {
        caption: qsTr("Reflection")
        width: parent.width

        SectionLayout {
            Label {
                text: qsTr("Stretch")
                tooltip: qsTr("Set the material reflection stretch.")
            }
            SecondColumnLayout {
                SpinBox {
                    maximumValue: 1
                    minimumValue: 0
                    decimals: 2
                    backendValue: backendValues.reflection_stretch
                    Layout.fillWidth: true
                }
            }
// TODO: Proper support for TextureInput properties
//            Label {
//                text: qsTr("Texture")
//                tooltip: qsTr("Defines a vertical texture for roughness map.")
//            }
//            SecondColumnLayout {
//                IdComboBox {
//                    typeFilter: "QtQuick3D.Texture"
//                    Layout.fillWidth: true
//                    backendValue: backendValues.roughness_texture_v.texture
//                }
//            }
        }
    }

    Section {
        caption: qsTr("Brush")
        width: parent.width

        ColumnLayout {
            width: parent.width
            SectionLayout {
                Label {
                    text: qsTr("Strenght")
                    tooltip: qsTr("Set the strength of the brush strokes.")
                }
                SecondColumnLayout {
                    SpinBox {
                        maximumValue: 1
                        minimumValue: 0
                        decimals: 2
                        backendValue: backendValues.brushing_strength
                        Layout.fillWidth: true
                    }
                }
            }
            ColumnLayout {
                width: parent.width - 16

                ColumnLayout {
                    width: parent.width
                    Label {
                        width: 100
                        text: qsTr("Tiling")
                        tooltip: qsTr("Sets the tiling repeat of the brush map.")
                    }

                    RowLayout {
                        spacing: materialRoot.labelSpinBoxSpacing

                        Label {
                            text: qsTr("X")
                            width: materialRoot.labelWidth
                        }
                        SpinBox {
                            maximumValue: 100
                            minimumValue: 1
                            decimals: 0
                            backendValue: backendValues.tiling_x
                            Layout.fillWidth: true
                            Layout.minimumWidth: materialRoot.spinBoxMinimumWidth
                        }
                    }
                    RowLayout {
                        spacing: materialRoot.labelSpinBoxSpacing

                        Label {
                            text: qsTr("Y")
                            width: materialRoot.labelWidth
                        }
                        SpinBox {
                            maximumValue: 100
                            minimumValue: 1
                            decimals: 0
                            backendValue: backendValues.tiling_y
                            Layout.fillWidth: true
                            Layout.minimumWidth: materialRoot.spinBoxMinimumWidth
                        }
                    }
                    RowLayout {
                        spacing: materialRoot.labelSpinBoxSpacing

                        Label {
                            text: qsTr("Z")
                            width: materialRoot.labelWidth
                        }
                        SpinBox {
                            maximumValue: 100
                            minimumValue: 1
                            decimals: 0
                            backendValue: backendValues.tiling_z
                            Layout.fillWidth: true
                            Layout.minimumWidth: materialRoot.spinBoxMinimumWidth
                        }
                    }
                }
            }
        }
    }

    Section {
        caption: qsTr("Bump")
        width: parent.width
        SectionLayout {
            Label {
                text: qsTr("Amount")
                tooltip: qsTr("Set the bump map bumpiness.")
            }
            SecondColumnLayout {
                SpinBox {
                    maximumValue: 2
                    minimumValue: 0
                    decimals: 2
                    backendValue: backendValues.bump_amount
                    Layout.fillWidth: true
                }
            }
// TODO: Proper support for TextureInput properties
//            Label {
//                text: qsTr("Texture")
//                tooltip: qsTr("Defines a texture for bump map.")
//            }
//            SecondColumnLayout {
//                IdComboBox {
//                    typeFilter: "QtQuick3D.Texture"
//                    Layout.fillWidth: true
//                    backendValue: backendValues.bump_texture.texture
//                }
//            }
        }
    }
}