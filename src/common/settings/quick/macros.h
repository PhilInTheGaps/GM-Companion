#pragma once

#define SETTINGS_PROPERTY(TYPE, NAME, DEFAULT) SETTINGS_PROPERTY2(TYPE, NAME, DEFAULT, DEFAULT_GROUP)
#define SETTINGS_PROPERTY_VAL(TYPE, NAME, DEFAULT) SETTINGS_PROPERTY_VAL2(TYPE, NAME, DEFAULT, DEFAULT_GROUP)

#define SETTINGS_PROPERTY2(TYPE, NAME, DEFAULT, GROUP) SETTINGS_PROPERTY3(TYPE, NAME, #NAME, DEFAULT, GROUP)
#define SETTINGS_PROPERTY_VAL2(TYPE, NAME, DEFAULT, GROUP) SETTINGS_PROPERTY_VAL3(TYPE, NAME, #NAME, DEFAULT, GROUP)

#define SETTINGS_PROPERTY3(TYPE, NAME, SETTING, DEFAULT, GROUP)                                                        \
    Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME##Changed)                                                    \
public:                                                                                                                \
    [[nodiscard]] static auto NAME()->TYPE                                                                             \
    {                                                                                                                  \
        return Common::Settings::SettingsManager::instance()->get<TYPE>(SETTING, DEFAULT, GROUP);                      \
    }                                                                                                                  \
    void NAME(const TYPE &value)                                                                                       \
    {                                                                                                                  \
        Common::Settings::SettingsManager::instance()->set(SETTING, value, GROUP);                                     \
        emit NAME##Changed(value);                                                                                     \
    }                                                                                                                  \
    Q_SIGNAL void NAME##Changed(TYPE value);

#define SETTINGS_PROPERTY_VAL3(TYPE, NAME, SETTING, DEFAULT, GROUP)                                                    \
    Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME##Changed)                                                    \
public:                                                                                                                \
    [[nodiscard]] static auto NAME()->TYPE                                                                             \
    {                                                                                                                  \
        return Common::Settings::SettingsManager::instance()->get<TYPE>(SETTING, DEFAULT, GROUP);                      \
    }                                                                                                                  \
    void NAME(TYPE value)                                                                                              \
    {                                                                                                                  \
        Common::Settings::SettingsManager::instance()->set(SETTING, value, GROUP);                                     \
        emit NAME##Changed(value);                                                                                     \
    }                                                                                                                  \
    Q_SIGNAL void NAME##Changed(TYPE value);
