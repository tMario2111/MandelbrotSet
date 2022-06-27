#include "ThemeManager.hpp"

ThemeManager::ThemeManager()
{
    loadThemes();
}

void ThemeManager::loadThemes()
{
    std::string default_theme{};
    {
        std::ifstream file{ "themes/default_theme.json" };
        nlohmann::json json{};
        file >> json;
        default_theme = json["theme"];
        file.close();
    }

    unsigned int i = 0;
    for (const auto& entry : std::filesystem::directory_iterator("themes"))
    {
        if (!std::filesystem::is_regular_file(entry) || 
            entry.path().extension().string() != ".json" ||
            entry.path().filename().string() == "default_theme.json")
            continue;

        nlohmann::json json{};
        {
            std::ifstream file{ entry.path().string() };
            file >> json;
            file.close();
        }

        auto theme = std::make_unique<Theme>();
        
        const auto filename = entry.path().filename().string();
        theme->name = filename.substr(0, filename.find_last_of('.')); // Remove extension

        theme->r_modifier = static_cast<f_type>(json["red"]["modifier"]);
        theme->r_func = static_cast<ColorFunction>(json["red"]["function"]);

        theme->g_modifier = static_cast<f_type>(json["green"]["modifier"]);
        theme->g_func = static_cast<ColorFunction>(json["green"]["function"]);

        theme->b_modifier = static_cast<f_type>(json["blue"]["modifier"]);
        theme->b_func = static_cast<ColorFunction>(json["blue"]["function"]);

        themes.push_back(std::move(theme));

        if (themes.back()->name == default_theme)
        {
            selected_theme = themes.back().get();
            selected_theme_index = i;
        }

        i++;
    }
    if (selected_theme == nullptr)
    {
        std::cerr << "Could not load default theme, aborting";
        exit(-1);
    }
}

Theme* ThemeManager::getSelectedTheme()
{
    return selected_theme;
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
            if (ImGui::Selectable(themes[i]->name.c_str(), is_selected))
            {
                selected_theme_index = i;
                needs_update = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    selected_theme = themes[selected_theme_index].get();

    spacing();

    if (ImGui::InputDouble("Red modifier", &selected_theme->r_modifier, 0.0, 0.0, "%.6f", 
        ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue) || 
            ImGui::ListBox("Red function", reinterpret_cast<int*>(&selected_theme->r_func), functions, 3))
            needs_update = true;

    spacing();

    if (ImGui::InputDouble("Green modifier", &selected_theme->g_modifier, 0.0, 0.0, "%.6f", 
        ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue) || 
            ImGui::ListBox("Green function", reinterpret_cast<int*>(&selected_theme->g_func), functions, 3))
            needs_update = true;

    spacing();

    if (ImGui::InputDouble("Blue modifier", &selected_theme->b_modifier, 0.0, 0.0, "%.6f", 
        ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue) || 
            ImGui::ListBox("Blue function", reinterpret_cast<int*>(&selected_theme->b_func), functions, 3))
            needs_update = true;

    return needs_update;
}