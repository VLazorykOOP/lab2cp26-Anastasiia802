#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

const int WIDTH = 100;
const int HEIGHT = 30;
const int SPEED = 1;
const int DELAY_MS = 100;
const int MAX_STEPS = 20;

mutex printMutex;

class Vehicle {
public:
    string name;
    int x, y;
    int directionX;
    int directionY;

    Vehicle(string name, int x, int y, int dx, int dy)
        : name(name), x(x), y(y), directionX(dx), directionY(dy) {
    }

    void moveCar() {
        for (int i = 0; i < MAX_STEPS; ++i) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << "[Car] " << name << " at X=" << x << " (Y=" << y << ")" << endl;
            }
            x += directionX * SPEED;

            if (x >= WIDTH || x <= 0) {
                directionX *= -1;
            }

            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }

    void moveMotorcycle() {
        for (int i = 0; i < MAX_STEPS; ++i) {
            {
                lock_guard<mutex> lock(printMutex);
                cout << "[Moto] " << name << " at Y=" << y << " (X=" << x << ")" << endl;
            }

            y += directionY * SPEED;

            if (y >= HEIGHT || y <= 0) {
                directionY *= -1;
            }

            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }
};

int main() {
    vector<thread> threads;

    threads.emplace_back(&Vehicle::moveCar, Vehicle("Auto_1", 0, 10, 1, 0));
    threads.emplace_back(&Vehicle::moveCar, Vehicle("Auto_2", WIDTH, 20, -1, 0));

    threads.emplace_back(&Vehicle::moveMotorcycle, Vehicle("Moto_1", 30, 0, 0, 1));
    threads.emplace_back(&Vehicle::moveMotorcycle, Vehicle("Moto_2", 70, HEIGHT, 0, -1));

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    cout << "Simulation finished." << endl;
    return 0;
}