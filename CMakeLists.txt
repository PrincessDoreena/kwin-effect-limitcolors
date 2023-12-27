cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.91")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(GenerateExportHeader)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)
include(KDEGitCommitHooks)

include(ECMInstallIcons)
include(ECMOptionalAddSubdirectory)
include(ECMConfiguredInstall)
include(ECMQtDeclareLoggingCategory)
include(ECMSetupQtPluginMacroNames)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Concurrent
    Core
    DBus
    UiTools
    Widgets
)
ecm_setup_qtplugin_macro_names(
    JSON_ARG2
        "KWIN_EFFECT_FACTORY"
    JSON_ARG3
        "KWIN_EFFECT_FACTORY_ENABLED"
        "KWIN_EFFECT_FACTORY_SUPPORTED"
    JSON_ARG4
        "KWIN_EFFECT_FACTORY_SUPPORTED_ENABLED"
    CONFIG_CODE_VARIABLE
        PACKAGE_SETUP_AUTOMOC_VARIABLES
)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    XmlGui
    ConfigWidgets
    WindowSystem
    CoreAddons
    GlobalAccel
    I18n
    Config
)
find_package(KWinDBusInterface)

#######################################
# Effect
kcoreaddons_add_plugin(kwin4_effect_limitcolors INSTALL_NAMESPACE "kwin/effects/plugins/")
target_sources(kwin4_effect_limitcolors PRIVATE
               limitcolors.cpp
               main.cpp
)
target_link_libraries(kwin4_effect_limitcolors PRIVATE
    kwineffects
    kwinglutils

    Qt5::Core
    KF5::WindowSystem
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::CoreAddons
    KF5::GlobalAccel
    KF5::I18n
    epoxy
)
kconfig_add_kcfg_files(kwin4_effect_limitcolors limitcolors_kcfg.kcfgc)

#######################################
# Config
set(kwin_effect_interface_SRCS dbus.xml)
qt5_add_dbus_interface(kwin_effect_interface_SRCS dbus.xml kwineffects_interface)
add_library(KWinEffectsInterface STATIC ${kwin_effect_interface_SRCS})
target_link_libraries(KWinEffectsInterface
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    KF5::ConfigCore
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::GlobalAccel
    KF5::I18n
    KF5::XmlGui
)

set(kwin_limitcolors_config_SRCS limitcolors_config.cpp)
ki18n_wrap_ui(kwin_limitcolors_config_SRCS limitcolors_config.ui)
kcoreaddons_add_plugin(kwin_limitcolors_config INSTALL_NAMESPACE "kwin/effects/configs" SOURCES ${kwin_limitcolors_config_SRCS})
target_link_libraries(kwin_limitcolors_config
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    KF5::WindowSystem
    KF5::ConfigCore
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::GlobalAccel
    KF5::I18n
    KF5::XmlGui
    KWinEffectsInterface
)
kconfig_add_kcfg_files(kwin_limitcolors_config limitcolors_kcfg.kcfgc)
