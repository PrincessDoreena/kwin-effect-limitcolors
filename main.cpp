/*
    SPDX-FileCopyrightText: 2023 Dorina Kovacs <princessdorinakovacs@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "limitcolors.h"

namespace KWin
{

KWIN_EFFECT_FACTORY_SUPPORTED(LimitColorsEffect,
                              "metadata.json",
                              return LimitColorsEffect::supported();)

} // namespace KWin

#include "main.moc"
