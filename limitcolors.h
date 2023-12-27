/*
    SPDX-FileCopyrightText: 2023 Dorina Kovacs <princessdorinakovacs@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <kwineffects.h>

namespace KWin
{

class GLShader;

/**
 * Compress desktop's colors
 */
class LimitColorsEffect
    : public Effect
{
    Q_OBJECT
    Q_PROPERTY(int colorFormat READ configuredColorFormat)
    Q_PROPERTY(int colorOffset READ configuredColorOffset)
public:
    LimitColorsEffect();
    ~LimitColorsEffect() override;

    void reconfigure(ReconfigureFlags flags) override;
    void paintScreen(int mask, const QRegion & region, KWin::ScreenPaintData & data) override;

    int requestedEffectChainPosition() const override;

    static bool supported();

    int configuredColorFormat() const;
    int configuredColorOffset() const;

protected:
    bool loadData();

private:
    bool m_inited;
    bool m_valid;
    std::unique_ptr<GLShader> m_shader;
    int m_textureLocation;
    int m_modelViewProjectioMatrixLocation;

    enum ColorFormatType {
        RGB565 = 0,
        RGB666 = 1,
    };
    ColorFormatType colorFormat;
    int colorOffset;
};

inline int LimitColorsEffect::requestedEffectChainPosition() const
{
    return 99;
}

} // namespace
