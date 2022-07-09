#pragma once

#include "Utility.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum ColorFunction
{
    Sin,
    Cos,
    Tan
};

struct Theme
{
    std::string name;

    f_type r_modifier;
    ColorFunction r_func;

    f_type g_modifier;
    ColorFunction g_func;

    f_type b_modifier;
    ColorFunction b_func;
};

class ThemeManager
{
public:
    ThemeManager();

    Theme getSelectedTheme();
    bool gui();
private:
    std::vector<Theme> themes;
    unsigned int selected_theme_index;
    std::string theme_name;

    void reload();
    void copyThemeToJson(Theme theme, nlohmann::json& json);
    void writeThemeToFile(Theme theme, const nlohmann::json& json);
    void loadThemes(); 
    void saveNewTheme();
    void deleteSelectedTheme();
    void modifySelectedTheme();
};