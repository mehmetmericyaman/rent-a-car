#include <iostream>
#include <fstream>
#include <string>
#include <forward_list>
#include <set>
#include <unordered_map> // for customer records

using namespace std;

// Car structure to hold car details
struct Car {
    string type;
    string brand;
    string model;
    int year;
    bool rented;
};
struct Customer {
    string name;
    string contact;
    set<string> renthistory;
};
// Function to load car data from file
void loadCars(forward_list<Car>& cars) {
    ifstream file("cars.txt");
    string line;
    while (getline(file, line)) {
        Car car;
        size_t pos = 0;
        pos = line.find(",");
        car.type = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        car.brand = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        car.model = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        try {
            car.year = stoi(line.substr(0, pos));
        } catch (const invalid_argument& e) {
            cerr << "Error: Invalid year: " << line.substr(0, pos) << endl;
            continue;
        }
        car.rented = false;
        cars.push_front(car);
    }
    file.close();
}
void displayInventory(const forward_list<Car>& cars) {
    cout << "\nCurrent Inventory:\n";
    for (const auto& car : cars) {
        cout << "Type: " << car.type << ", Brand: " << car.brand << ", Model: " << car.model << ", Year: " << car.year;
        if (car.rented)
            cout << " (Rented)";
        cout << endl;
    }
}
//rent a car 2.seçenek
void rentCar(forward_list<Car>& cars, unordered_map<string, Customer>& customers) {
    // Collect available car types, brands, and models
    set<string> availableTypes;
    set<string> availableBrands;
    set<string> availableModels;
    for (const auto& car : cars) {
        if (!car.rented) {
            availableTypes.insert(car.type);
        }
    }
    // mevcut araç türlerini gösterme
    cout << "\nSelect car type: \n";
    int typeChoice = 1;
    for (const auto& type : availableTypes) {
        cout << typeChoice << ". " << type << endl;
        ++typeChoice;
    }
    cout << "Enter your choice: ";
    cin >> typeChoice;
    if (typeChoice < 1 || typeChoice > availableTypes.size()) {
        cout << "Invalid choice\n";
        return;
    }
    // seçilen araç tipi
    auto typeIt = availableTypes.begin();
    advance(typeIt, typeChoice - 1);
    string selectedType = *typeIt;

    // araç tipi için marka bulma
    for (const auto& car : cars) {
        if (car.type == selectedType && !car.rented) {
            availableBrands.insert(car.brand);
        }
    }
    // araç tipi için marka seçme
    cout << "\nSelect brand:\n";
    int brandChoice = 1;
    for (const auto& brand : availableBrands) {
        cout << brandChoice << ". " << brand << endl;
        ++brandChoice;
    }
    cout << "Enter your choice: ";
    cin >> brandChoice;
    if (brandChoice < 1 || brandChoice > availableBrands.size()) {
        cout << "Invalid choice\n";
        return;
    }
    // Seçilen markayı bulma
    auto brandIt = availableBrands.begin();
    advance(brandIt, brandChoice - 1);
    string selectedBrand = *brandIt;

    // araç tipi ve marka için mevcut modeller
    for (const auto& car : cars) {
        if (car.type == selectedType && car.brand == selectedBrand && !car.rented) {
            availableModels.insert(car.model);
        }
    }
    //araç tipi ve marka için mevcut modelleri seçme
    cout << "\nSelect model:\n";
    int modelChoice = 1;
    for (const auto& model : availableModels) {
        cout << modelChoice << ". " << model << endl;
        ++modelChoice;
    }
    cout << "Enter your choice: ";
    cin >> modelChoice;
    if (modelChoice < 1 || modelChoice > availableModels.size()) {
        cout << "Invalid choice\n";
        return;
    }

    // seçilen modeli bulma
    auto modelA = availableModels.begin();
    advance(modelA, modelChoice - 1);
    string selectedModel = *modelA;

    // seçilen arabayı bulup kiralama
    for (auto& car : cars) {
        if (car.type == selectedType && car.brand == selectedBrand && car.model == selectedModel && !car.rented) {
            car.rented = true;
            // Associate rental with customer
            string customerName;
            cout << "Enter your name: ";
            cin.ignore(); // Clear the input buffer
            getline(cin, customerName);
            if (customers.find(customerName) == customers.end()) {
                Customer newCustomer;
                newCustomer.name = customerName;
                cout << "Enter your contact detail: ";
                getline(cin, newCustomer.contact);
                customers[customerName] = newCustomer;
            }
            customers[customerName].renthistory.insert(selectedBrand + " " + selectedModel);
            cout << "Car rented successfully.\n";
            return;
        }
    }
    cout << "No available car your selection.\n";
}

// return a rented car 3.seçenek
void returnCar(forward_list<Car>& cars, unordered_map<string, Customer>& customers) {
    cout << "\nAvailable cars for return :\n";
    int option = 1;
    for (const auto& car : cars) {
        if (car.rented) {
            cout << option << ". " << car.brand << " " << car.model << " - " << car.year << endl;
            ++option;
        }
    }
    if (option == 1) {
        cout << "No cars are currently rented.\n";
        return;
    }
    int choice;
    cout << "Enter the number of the car to return: ";
    cin >> choice;
    if (choice < 1 || choice >= option) {
        cout << "Invalid choice\n";
        return;
    }
    auto it = cars.begin();
    advance(it, choice - 1);
    it->rented = false;
    string brand_model = it->brand + " " + it->model;
    for (auto& pair : customers) {
        auto &customer = pair.second;
        auto &rentalHistory = customer.renthistory;
        rentalHistory.erase(brand_model); // kiralama geçmişinden kaldırma
    }
    cout << "Car returned successfully.\n";
}
// register new customer 4.seçenek
void registerCustomer(unordered_map<string, Customer>& customers) {
    string name;
    cout << "\nEnter your name: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, name);
    if (customers.find(name) == customers.end()) {
        Customer newCustomer;
        newCustomer.name = name;
        cout << "Enter your contact details: ";
        getline(cin, newCustomer.contact);
        customers[name] = newCustomer;
        cout << "Customer registered successfully.\n";
    } else {
        cout << "Customer name already exists.\n";
    }
}
//view cutomer accounts 5.seçenek
void viewCustomerAccounts(const unordered_map<string, Customer>& customers) {
    cout << "\nCustomer Accounts:\n";
    for (const auto& pair : customers) {
        const auto& customer = pair.second;
        cout << "Name: " << customer.name << ", Contact: " << customer.contact << endl;
        cout << "Rental History: \n";
        for (const auto& rental : customer.renthistory) {
            cout << " - " << rental << endl;
        }
        cout << endl;
    }
}
// menü
void displayMenu() {
    cout << "\n***** EliteDrive Rental System *****\n";
    cout << "1. Display current inventory\n";
    cout << "2. Rent a car\n";
    cout << "3. Return a rented car\n";
    cout << "4. Register new customer\n";
    cout << "5. View customer accounts\n";
    cout << "6. Exit\n";
}
int main() {
    forward_list<Car> cars;
    unordered_map<string, Customer> customers;
    loadCars(cars);

    while (true) {
        displayMenu();
        int choice;
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore(); // clear the input buffer

        switch (choice) {
            case 1:
                displayInventory(cars);
                break;
            case 2:
                rentCar(cars, customers);
                break;
            case 3:
                returnCar(cars, customers);
                break;
            case 4:
                registerCustomer(customers);
                break;
            case 5:
                viewCustomerAccounts(customers);
                break;
            case 6:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
