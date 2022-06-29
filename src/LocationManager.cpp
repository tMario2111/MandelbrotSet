#include "LocationManager.hpp"

LocationManager::LocationManager()
{
    loadLocations();
}

void LocationManager::loadLocations()
{
    for (const auto entry : std::filesystem::directory_iterator("locations"))
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

        NamedLocation named_location;
        const auto filename = entry.path().filename().string();
        named_location.name = filename.substr(0, filename.find_last_of('.'));

        named_location.location.position.x = json["x"];
        named_location.location.position.y = json["y"];
        named_location.location.zoom = json["zoom"];

        named_locations.push_back(named_location);
    }
}

std::optional<Location> LocationManager::gui(Location current_location)
{
    ImGui::InputText("New location", &new_location_name);
    if (ImGui::Button("Save new location"))
    {
        nlohmann::json json{};
        json["x"] = current_location.position.x;
        json["y"] = current_location.position.y;
        json["zoom"] = current_location.zoom;
        
        std::ofstream file{ "locations/" + new_location_name + ".json" };
        file << std::setw(4) << json;
        file.close();

        named_locations.clear();
        loadLocations();
    }

    ImGui::Dummy(ImVec2{ 0.f, 10.f });

    if (ImGui::BeginListBox("Locations"))
    {
        for (auto i = 0u; i < named_locations.size(); i++)
        {
            const bool is_selected = selected_named_location_index == i;
            if (ImGui::Selectable(named_locations[i].name.c_str(), is_selected))
                selected_named_location_index = i;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (ImGui::Button("Go"))
        return named_locations[selected_named_location_index].location;

    ImGui::SameLine();

    if (ImGui::Button("Delete"))
    {
        std::filesystem::remove("locations/" + named_locations[selected_named_location_index].name + ".json");
        named_locations.erase(named_locations.begin() + selected_named_location_index);
        selected_named_location_index = 0;
    }

    return {}; // Empty optional
}