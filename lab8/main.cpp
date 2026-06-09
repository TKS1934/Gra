#include <iostream>
#include <vector>

using namespace std;

class Vehicle {
public:
    std::string name() { return name_; }
    int number_of_wheels() { return number_of_wheels_; }
    std::string propulsion_type() { return propulsion_type_; }
    double max_speed() { return max_speed_; }
    virtual void print() {
        std::cout << "Name: " << name() << ", wheels: " << number_of_wheels() << std::endl;
    }
protected:
    Vehicle(const std::string &name, int number_of_wheels, const std::string &propulsion_type, double max_speed)
        : name_(name), number_of_wheels_(number_of_wheels), propulsion_type_(propulsion_type), max_speed_(max_speed) {}

    std::string name_;
    int number_of_wheels_;
    std::string propulsion_type_;
    double max_speed_;
};
class Bike : public Vehicle {
public:
    Bike() : Vehicle("Bike", 2, "Muscles", 30) {}
};
class Car : public Vehicle {
public:
    Car(const std::string &name, const std::string &propulsion_type, double max_speed, bool has_abs)
        : Vehicle(name, 4, propulsion_type, max_speed), has_abs_(has_abs) {}
    void print() {
        std::cout << "Name: " << name() << ", wheels: " << number_of_wheels() << ", abs: " << has_abs() << std::endl;
    }
    bool has_abs() { return has_abs_; }

private:
    bool has_abs_;
};

int main() {
    Bike bike;
    //bike.print();
    Car passat("Volkswagen Passat", "Diesel", 200, true);
    //passat.print();

    std::vector<Vehicle*> vehicles;
    vehicles.push_back(&bike);
    vehicles.push_back(&passat);
    for (auto& v: vehicles) v->print();

    return 0;
}

