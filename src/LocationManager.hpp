#pragma once

#include "Utility.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <SFML/System.hpp>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <optional>
#include <string>
#include <utility>
#include <vector>

struct Location
{
    sf::Vector2<f_type> position;
    f_type zoom;
};

class LocationManager
{
public:
    LocationManager();

    std::optional<Location> gui(Location current_location);
private:
    struct NamedLocation
    {
        Location location;
        std::string name;
    };

    std::vector<NamedLocation> named_locations;
    unsigned int selected_named_location_index = 0;
    std::string new_location_name;

    void loadLocations();
};