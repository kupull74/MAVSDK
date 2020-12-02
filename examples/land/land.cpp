#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>
#include <thread>

int main(int argc, char** argv) {

std::this_thread::sleep_for(std::chrono::seconds(10)); // stuck air
std::cout << "Landing..." << std::endl;
const Action::Result land_result = action->land();
if (land_result != Action::Result::Success) { return 1; } // Land failed
while (telemetry->in_air()) { // Check if vehicle is still in air
std::this_thread::sleep_for(std::chrono::seconds(1)); // Vehicle is landing...
}
std::cout << "Landed!" << std::endl;
// Relying on auto-disarming but let's keep watching the telemetry for a bit longer.
std::this_thread::sleep_for(std::chrono::seconds(3));
std::cout << "Finished..." << std::endl;
return 0;
}
