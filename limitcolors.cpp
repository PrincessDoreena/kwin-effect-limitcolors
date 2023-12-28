/*
    SPDX-FileCopyrightText: 2023 Dorina Kovacs <princessdorinakovacs@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "limitcolors.h"
#include "limitcolors_kcfg.h"

#include <KGlobalAccel>
#include <KLocalizedString>
#include <QStringBuilder>
#include <QAction>
#include <kwinglplatform.h>
#include <kwinglutils.h>

#include <QMatrix4x4>

Q_LOGGING_CATEGORY(KWIN_LIMITCOLORS, "kwin_effect_limitcolors", QtWarningMsg)

namespace KWin
{

LimitColorsEffect::LimitColorsEffect()
    : m_inited(false)
    , m_valid(true)
    , m_shader(nullptr)
    , colorFormat(RGB565)
    , colorOffset(0)
{
    QAction *a = new QAction(this);
    a->setObjectName(QStringLiteral("LimitColors"));

    reconfigure(ReconfigureAll);
}

LimitColorsEffect::~LimitColorsEffect() = default;

bool LimitColorsEffect::supported()
{
    if (effects->compositingType() == OpenGLCompositing) {
        effects->makeOpenGLContextCurrent();
        return (glGetString(GL_SHADING_LANGUAGE_VERSION)[0] - '0') >= 3;
    } else {
        return false;
    }
}

void LimitColorsEffect::reconfigure(ReconfigureFlags flags) {
    LimitColorsKConfig::self()->read();

    colorFormat = ColorFormatType(LimitColorsKConfig::format_dropdown());
    colorOffset = LimitColorsKConfig::offset_dropdown();

    loadData();
}

bool LimitColorsEffect::loadData()
{
    m_inited = true;

    QString compressionTemplate;
    switch(colorFormat) {
        case RGB565:
            compressionTemplate = QStringLiteral("    i = ivec3((i.x & n), (i.y & m), (i.z & n)) + ");
            break;
        case RGB666:
            compressionTemplate = QStringLiteral("    i = ivec3((i.x & m), (i.y & m), (i.z & m)) + ");
            break;
    }

    QByteArray vertSource = QByteArrayLiteral("#version 300 es\n"
                                              "\n"
                                              "in vec4 position;\n"
                                              "in vec4 texcoord;\n"
                                              "\n"
                                              "out vec2 texcoord0;\n"
                                              "\n"
                                              "uniform mat4 modelViewProjectionMatrix;\n"
                                              "\n"
                                              "void main()\n"
                                              "{\n"
                                              "    texcoord0 = texcoord.st;\n"
                                              "    gl_Position = modelViewProjectionMatrix * position;\n"
                                              "}");
    QByteArray fragSource = (QStringLiteral("#version 300 es\n"
                                "\n"
                                "precision highp float;\n"
                                "precision highp int;\n"
                                "\n"
                                "uniform sampler2D sampler;\n"
                                "in vec2 texcoord0;\n"
                                "\n"
                                "out vec4 fragColor;\n"
                                "\n"
                                "void main()\n"
                                "{\n"
                                "    int m = 252;\n"
                                "    int n = 248;\n"
                                "    float const1 = 255.0/256.0;\n"
                                "    float const2 = 32768.0;\n"
                                "    float const3 = 1.0/255.0;\n"
                                "    vec4 c = texture(sampler, texcoord0);\n"
                                "    ivec3 i = ivec3(floatBitsToInt(c.x*const1 + const2), floatBitsToInt(c.y*const1 + const2), floatBitsToInt(c.z*const1 + const2));\n")
                 % compressionTemplate % QString::number(colorOffset)
                 % QStringLiteral(";\n"
                                  "    c = vec4(float(i.x)*const3, float(i.y)*const3, float(i.z)*const3, 1.0);\n"
                                  "    fragColor = c;\n"
                                  "}")).toUtf8();
    m_shader = ShaderManager::instance()->generateCustomShader(ShaderTrait::MapTexture, vertSource, fragSource);
    if (!m_shader->isValid()) {
        qCCritical(KWIN_LIMITCOLORS) << "The shader failed to load!";
        return false;
    }
    m_modelViewProjectioMatrixLocation = m_shader->uniformLocation("modelViewProjectionMatrix");
    m_textureLocation = m_shader->uniformLocation("sampler");

    return true;
}

void LimitColorsEffect::prePaintScreen(ScreenPrePaintData &data, std::chrono::milliseconds presentTime) {
    data.mask |= PAINT_SCREEN_TRANSFORMED;
    effects->prePaintScreen(data, presentTime);
}

void LimitColorsEffect::paintScreen(int mask, const QRegion & region, KWin::ScreenPaintData & data) {
    if (m_valid && !m_inited) {
         m_valid = loadData();
    }

    EffectScreen* screen = data.screen();
    m_texture.reset(new GLTexture(GL_RGBA8, screen->geometry().size() * screen->devicePixelRatio()));
    m_frameBuffer.reset(new GLFramebuffer(m_texture.get()));

    GLFramebuffer::pushFramebuffer(m_frameBuffer.get());
    effects->paintScreen(mask, region, data);
    GLFramebuffer::popFramebuffer();

    QMatrix4x4 modelViewProjectionMatrix(data.projectionMatrix());

    glActiveTexture(GL_TEXTURE0);
    m_texture->bind();

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    ShaderManager *sm = ShaderManager::instance();
    sm->pushShader(m_shader.get());
    m_shader->setUniform(m_modelViewProjectioMatrixLocation, modelViewProjectionMatrix);
    m_shader->setUniform(m_textureLocation, 0);

    m_texture->render(screen->geometry(), 1);

    sm->popShader();
    m_texture->unbind();
}

int LimitColorsEffect::configuredColorFormat() const
{
    return colorFormat;
}

int LimitColorsEffect::configuredColorOffset() const
{
    return colorOffset;
}

} // namespace
