#include <iostream>
#include <string.h>
#include <string>
#include <mavsdk/mavsdk.h> 
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <cstdint>
#include <mavsdk/plugins/mission/mission.h>
#include <future>
#include <random>
#include <math.h>
#define PI 3.14158265358979323846
using namespace std;

typedef double custem_t;
custem_t degreetoradian(custem_t degree)
{
    return ((PI / 180) * degree);
}
int main(int argc, char** argv)
{
    Mavsdk mavsdk;
    ConnectionResult connection_result;
    bool discovered_system = false;
    connection_result = mavsdk.add_any_connection(argv[1]);
    if (connection_result != ConnectionResult::Success) {
        return 1;
    } // Failed
    mavsdk.subscribe_on_new_system([&mavsdk, &discovered_system]() {
        const auto system = mavsdk.systems().at(0);
        if (system->is_connected()) {
            discovered_system = true;
        } // Discovered system
    }); // Waiting to discover system...
    std::this_thread::sleep_for(std::chrono::seconds(2)); // heartbeats at 1Hz
    if (!discovered_system) {
        return 1;
    } // No system found
    const auto system = mavsdk.systems().at(0);
    system->register_component_discovered_callback([](ComponentType com_type) -> void {
        std::cout << unsigned(com_type);
    }); // Register a callback components (camera, gimbal) etc are found



    //regist_telemetry
    auto telemetry = std::make_shared<Telemetry>(system);
    // We want to listen to the altitude of the drone at 1 Hz.
    const Telemetry::Result set_rate_result = telemetry->set_rate_position(1.0);
    if (set_rate_result != Telemetry::Result::Success) {
        return 1;
    } // Set rate failed
    telemetry->subscribe_position([](Telemetry::Position position) {
        std::cout << "Altitude: " << position.relative_altitude_m << " m" << std::endl;
    }); // Set up callback to monitor altitude
    while (telemetry->health_all_ok() != true) {
        std::cout << "Vehicle is getting ready to arm" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } // Check if vehicle is ready to arm


//mission1
    std::vector<Mission::MissionItem> mission_items;
    

    for (int i = 0; i < 360; i++){ // }
    custem_t current_x = 47.397691, current_y = 8.545796, distance = 0.0001;
    custem_t radian = degreetoradian(i);
    custem_t target_x = current_x + distance * cos(radian);
    custem_t target_y = current_y + distance * sin(radian);
    std::cout << "target X:" << target_x << ", target Y:" << target_y << std::endl;
    
    Mission::MissionItem mission_item;
    mission_item.latitude_deg = target_x; 
    mission_item.longitude_deg = target_y; 
    mission_item.relative_altitude_m = 10.0f; // takeoff altitude
    mission_item.speed_m_s = 150.0f;
    mission_item.is_fly_through = false;
    mission_items.push_back(mission_item);

    }
    
        
    return 0;
}