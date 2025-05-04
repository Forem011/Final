#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>  // Added for setprecision
using namespace std;

// Physical constants
const double GRAVITY = 9.81; // m/s²
const double ROCKET_MASS = 1000.0; // kg (initial mass without fuel)
const double FUEL_MASS_RATIO = 0.0; // kg of fuel per unit of fuel

// Struct for tracking the rocket's state
struct RocketState {
  double x;
  double y;             // Coordinates (m)
  double velocityX, velocityY; // Velocity components (m/s)
  double fuel;             // Remaining fuel units

  // Calculate total mass (rocket + remaining fuel)
  double calculateMass() const {
    return ROCKET_MASS + (fuel * FUEL_MASS_RATIO);
  }
};

// Function to calculate rocket movement
RocketState calculateNextState(RocketState currentState, double thrust, double angle, double deltaTime) {
  // Create a new state to return
  RocketState newState = currentState;

  // Convert angle to radians
  double radianAngle = angle * M_PI / 180.0;

  // Calculate current mass
  double mass = currentState.calculateMass();

  // Calculate thrust components
  double thrustX = 0.0;
  double thrustY = 0.0;

  // Only apply thrust if there's fuel left
  if (currentState.fuel > 0) {
    thrustX = thrust * cos(radianAngle);
    thrustY = thrust * sin(radianAngle);

    // Update fuel (fuel consumption proportional to thrust)
    double fuelConsumptionRate = 0.01; // units of fuel per newton-second
    newState.fuel = currentState.fuel - thrust * fuelConsumptionRate * deltaTime;
    if (newState.fuel < 0) newState.fuel = 0; // Ensure fuel does not go negative
  }

  // Calculate acceleration (F = ma)
  double ax = thrustX / mass;
  double ay = (thrustY / mass) - GRAVITY; // Gravity pulls down (negative y)

  // Update velocity (v = v₀ + at)
  newState.velocityX = currentState.velocityX + ax * deltaTime;
  newState.velocityY = currentState.velocityY + ay * deltaTime;

  // Update position (x = x₀ + vt)
  newState.x = currentState.x + newState.velocityX * deltaTime;
  newState.y = currentState.y + newState.velocityY * deltaTime;

  return newState;
}

int main() {
  // 1. Get User Input (thrust, angle, initial fuel)
  double thrust, angle, initialFuel;
  cout << "Enter thrust (Newtons): ";
  cin >> thrust;
  cout << "Enter launch angle (degrees): ";
  cin >> angle;
  cout << "Enter initial fuel amount (kg): ";
  cin >> initialFuel;

  // 2. Initialize Rocket State
  RocketState rocket = {0.0, 63780.0, 0.0, 0.0, initialFuel};

  // 3. Simulation Loop
  const double deltaTime = 0.1; // Smaller time step for better accuracy
  const int maxSteps = 10000;    // More steps for longer simulation

  cout << "Time(s)\tX(m)\tY(m)\tVel-X(m/s)\tVel-Y(m/s)\tFuel(kg)\tMass(kg)" << endl;
  ofstream outputFile("rocket_simulation.csv"); // Open file for output
  outputFile << "Time(s),X(m),Y(m),Vel-X(m/s),Vel-Y(m/s),Fuel(kg),Mass(kg)" << endl;

  for (int i = 0; i < maxSteps; ++i) {
    // Display current rocket state
    cout << fixed << setprecision(2)
         << i * deltaTime << "\t"
         << rocket.x << "\t"
         << rocket.y << "\t"
         << rocket.velocityX << "\t\t"
         << rocket.velocityY << "\t\t"
         << rocket.fuel << "\t"
         << rocket.calculateMass() << endl;
         
         outputFile << fixed << setprecision(2)
         << i * deltaTime << ",   "
         << rocket.x << ",   "
         << rocket.y << ",   "
         << rocket.velocityX << ",   "
         << rocket.velocityY << ",   "
         << rocket.fuel << ",   " 
         << rocket.calculateMass() << endl;

    // Check if the rocket has landed or crashed
    if (rocket.y < 0) {
      cout << "Rocket has landed/crashed at x = " << rocket.x << "m" << endl;
      break;
    }

    // 4. Calculate Next State (important fix: store the result!)
    rocket = calculateNextState(rocket, thrust, angle, deltaTime);
  }
  outputFile.close(); 
  return 0;
}