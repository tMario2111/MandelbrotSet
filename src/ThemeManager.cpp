#include "ThemeManager.hpp"

ThemeManager::ThemeManager()
{
    loadThemes();
}

// TODO: Add safety for deletion
void ThemeManager::loadThemes()
{
    for (const auto& entry : std::filesystem::directory_iterator("themes"))
    {
        if (!std::filesystem::is_regular_file(entry) || 
            entry.path().extension().string() != ".json")
            continue;

        nlohmann::json json{};
        {
            std::ifstream file{ entry.path().string() };
            file >> json;
            file.close();
        }

        Theme theme{};
        
        const auto filename = entry.path().filename().string();
        theme.name = filename.substr(0, filename.find_last_of('.')); // Remove extension

        theme.r_modifier = static_cast<f_type>(json["red"]["modifier"]);
        theme.r_func = static_cast<ColorFunction>(json["red"]["function"]);

        theme.g_modifier = static_cast<f_type>(json["green"]["modifier"]);
        theme.g_func = static_cast<ColorFunction>(json["green"]["function"]);

        theme.b_modifier = static_cast<f_type>(json["blue"]["modifier"]);
        theme.b_func = static_cast<ColorFunction>(json["blue"]["function"]);

        themes.push_back(theme);
    }
    selected_theme_index = 0;
    theme_name = themes[0].name;
}

void ThemeManager::reload()
{
    themes.clear();
    loadThemes();
    selected_theme_index = 0;
}

Theme ThemeManager::getSelectedTheme()
{
    return themes[selected_theme_index];
}

void ThemeManager::copyThemeToJson(Theme theme, nlohmann::json& json)
{
    json["red"]["modifier"] = theme.r_modifier;
    json["red"]["function"] = theme.r_func;

    json["green"]["modifier"] = theme.g_modifier;
    json["green"]["function"] = theme.g_func;

    json["blue"]["modifier"] = theme.b_modifier;
    json["blue"]["function"] = theme.b_func;
}

void ThemeManager::saveNewTheme()
{
    auto selected_theme = themes[selected_theme_index];
    selected_theme.name = theme_name;

    nlohmann::json json{};
    copyThemeToJson(selected_theme, json);
    writeThemeToFile(selected_theme, json);
    reload();
}

void ThemeManager::deleteSelectedTheme()
{
    std::filesystem::remove("themes/" + themes[selected_theme_index].name + ".json");
    themes.erase(themes.begin() + selected_theme_index);
    selected_theme_index = 0;
}

void ThemeManager::writeThemeToFile(Theme theme, const nlohmann::json& json)
{
    std::ofstream file{"themes/" + theme.name + ".json"};
    file << std::setw(4) << json;
    file.close();
}

void ThemeManager::modifySelectedTheme()
{
    auto selected_theme = themes[selected_theme_index];
    selected_theme.name = theme_name;
    deleteSelectedTheme();
    nlohmann::json json{};
    copyThemeToJson(selected_theme, json);
    writeThemeToFile(selected_theme, json);
    reload();
}

bool ThemeManager::gui()
{
    const auto spacing = []{ ImGui::Dummy(ImVec2{ 0.f, 10.f }); };
    auto needs_update = false;
    const char* functions[] = { "sin", "cos", "tan" };
    if (ImGui::BeginListBox("Themes"))
    {
        for (auto i = 0u; i < themes.size(); i++)
        {
            const bool is_selected = selected_theme_index == i;
            if (ImGui::Selectable(themes[i].name.c_str(), is_selected))
            {
                selected_theme_index = i;
                theme_name = themes[i].name;
                needs_update = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::InputText("Name", &theme_name);

    if (ImGui::Button("Save new"))
    {
        saveNewTheme();
        needs_update = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Modify"))
        modifySelectedTheme();

    ImGui::SameLine();

    if (ImGui::Button("Delete"))
    {
        deleteSelectedTheme();
        needs_update = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Reload"))
    {
        reload();
        needs_update = true;
    }

    spacing();

    auto& selected_theme = themes[selected_theme_index];
    if (ImGui::InputDouble("Red modifier", &selected_theme.r_modifier, 0.0, 0.0, "%.6f", 
        ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue) || 
            ImGui::ListBox("Red function", reinterpret_cast<int*>(&selected_theme.r_func), functions, 3))
            needs_update = true;

    spacing();

    if (ImGui::InputDouble("Green modifier", &selected_theme.g_modifier, 0.0, 0.0, "%.6f", 
        ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue) || 
            ImGui::ListBox("Green function", reinterpret_cast<int*>(&selected_theme.g_func), functions, 3))
            needs_update = true;

    spacing();

    if (ImGui::InputDouble("Blue modifier", &selected_theme.b_modifier, 0.0, 0.0, "%.6f", 
        ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue) || 
            ImGui::ListBox("Blue function", reinterpret_cast<int*>(&selected_theme.b_func), functions, 3))
            needs_update = true;

    return needs_update;
}