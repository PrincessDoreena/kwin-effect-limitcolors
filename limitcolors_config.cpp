/*
    SPDX-FileCopyrightText: 2023 Dorina Kovacs <princessdorinakovacs@gmail.com>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "limitcolors_config.h"
#include "limitcolors_kcfg.h"

#include <kwineffects_interface.h>

#include <KActionCollection>
#include <KGlobalAccel>
#include <KLocalizedString>
#include <KPluginFactory>

#include <QAction>

K_PLUGIN_CLASS(LimitColorsConfig)

LimitColorsConfig::LimitColorsConfig(QObject *parent, const KPluginMetaData &data, const QVariantList &args)
    : KCModule(parent, data, args)
{
    ui.setupUi(widget());
    addConfig(LimitColorsKConfig::self(), widget());

    auto actionCollection = new KActionCollection(this, QStringLiteral("kwin"));
    actionCollection->setComponentDisplayName(i18n("KWin"));
    actionCollection->setConfigGroup(QStringLiteral("limitcolors"));
    actionCollection->setConfigGlobal(true);
}

void LimitColorsConfig::load()
{
    KCModule::load();
}

void LimitColorsConfig::save()
{
    LimitColorsKConfig::self()->save();
    KCModule::save();

    OrgKdeKwinEffectsInterface interface(QStringLiteral("org.kde.KWin"), QStringLiteral("/Effects"), QDBusConnection::sessionBus());
    interface.reconfigureEffect(QStringLiteral("limitcolors"));
}

void LimitColorsConfig::defaults()
{
    KCModule::defaults();
}

#include "limitcolors_config.moc"
