#pragma once

#include <imgui.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using f_type = double; // Should probably use a template

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

    Theme* getSelectedTheme();
    bool gui();
private:
    std::vector<std::unique_ptr<Theme>> themes;
    Theme* selected_theme = nullptr;
    unsigned int selected_theme_index;

    void loadThemes(); 
};