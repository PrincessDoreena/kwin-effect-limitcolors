/*
    SPDX-FileCopyrightText: 2023 Dorina Kovacs <princessdorinakovacs@gmail.com>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include <KCModule>

#include "ui_limitcolors_config.h"

class LimitColorsConfig : public KCModule
{
    Q_OBJECT

public:
    LimitColorsConfig(QObject *parent, const KPluginMetaData &data, const QVariantList &args);
    ~LimitColorsConfig()=default;

public Q_SLOTS:
    void load() override;
    void save() override;
    void defaults() override;

private:
    ::Ui::LimitColorsConfig ui;
};
