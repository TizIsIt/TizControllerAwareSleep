#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include <ctime>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>

#pragma comment(lib, "Xinput.lib")

// Function to monitor controller input with optimized polling
void monitorController(std::atomic<int>& countdownSeconds) {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    DWORD dwResult;
    int buttonState[16] = { 0 };

    while (true) {
        dwResult = XInputGetState(0, &state);

        if (dwResult == ERROR_SUCCESS) {
            for (int i = 0; i < 16; ++i) {
                int newState = (state.Gamepad.wButtons & (1 << i)) ? 1 : 0;
                if (newState != buttonState[i]) {
                    buttonState[i] = newState;
                    if (newState == 1) {
                        // Minimize console output
                        std::cout << "Button " << i << " pressed. Timer reset to 5 minutes." << std::endl;
                        countdownSeconds.store(300); // Reset the timer to 300 seconds (5 minutes)
                    }
                }
            }
        }

        // Introduce a delay to reduce polling rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60Hz polling rate
    }
}

// Optimized function to hibernate the computer
void hibernateComputer(std::atomic<bool>& hibernateFlag) {
    while (!hibernateFlag.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Efficient wait
    }

    std::cout << "Hibernating computer..." << std::endl;
    system("shutdown /h /f");
}

int main() {
    std::atomic<int> countdownSeconds(300); // 5 minutes
    std::atomic<bool> hibernateFlag(false);

    std::cout << "Detecting gamepad..." << std::endl;

    std::thread inputThread(monitorController, std::ref(countdownSeconds));
    std::thread hibernateThread(hibernateComputer, std::ref(hibernateFlag));

    int lastDisplayedTime = -1;

    while (countdownSeconds > 0) {
        int currentTime = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count();

        if (currentTime != lastDisplayedTime && (countdownSeconds % 2 == 0)) {
            int minutes = countdownSeconds / 60;
            int seconds = countdownSeconds % 60;

            std::cout << "Time remaining: " << minutes << " minutes " << seconds << " seconds" << std::endl;
            lastDisplayedTime = currentTime;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        countdownSeconds--;
    }

    hibernateFlag.store(true);

    std::cout << "Exiting..." << std::endl;

    inputThread.join();
    hibernateThread.join();

    return 0;
}
