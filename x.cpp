#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

using namespace std;

int len = 0, len_h = 0;

class Property {
public:
    float price;
    int rooms;
    int bathrooms;
    int kitchens;

    Property(float p = 0, int r = 0, int b = 0, int k = 0)
        : price(p), rooms(r), bathrooms(b), kitchens(k) {}

    virtual void print() const = 0;
    virtual ~Property() {}
};

class Apartment : public Property {
public:
    int parking_space;

    Apartment(float p = 0, int r = 0, int b = 0, int k = 0, int ps = 0)
        : Property(p, r, b, k), parking_space(ps) {}

    void print() const override {
        cout << GREEN << "Price: $" << price << RESET << '\n';
        cout << "Rooms: " << rooms << '\n';
        cout << "Bathrooms: " << bathrooms << '\n';
        cout << "Kitchens: " << kitchens << '\n';
        cout << "Parking: " << (parking_space ? "Yes" : "No") << '\n';
        cout << "------------------------\n";
    }
};

class House : public Property {
public:
    int yard_size;

    House(float p = 0, int r = 0, int b = 0, int k = 0, int ys = 0)
        : Property(p, r, b, k), yard_size(ys) {}

    void print() const override {
        cout << GREEN << "Price: $" << price << RESET << '\n';
        cout << "Rooms: " << rooms << '\n';
        cout << "Bathrooms: " << bathrooms << '\n';
        cout << "Kitchens: " << kitchens << '\n';
        cout << "Yard size: " << yard_size << " sqm" << '\n';
        cout << "------------------------\n";
    }
};

const int MAX_PROPERTIES = 100;

Apartment app[MAX_PROPERTIES];
House house[MAX_PROPERTIES];
Apartment boughts[MAX_PROPERTIES];
House boughts_houses[MAX_PROPERTIES];
Apartment fav_apts[MAX_PROPERTIES];
House fav_houses[MAX_PROPERTIES];

int boughts_count = 0;
int boughts_houses_count = 0;
int fav_apts_count = 0;
int fav_houses_count = 0;

int get_valid_int(int min, int max) {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
            cin.clear();
            cin.ignore(10000, '\n');
        } else {
            break;
        }
    }
    return value;
}

void load_properties(const char* filename) {
    ifstream filep(filename);
    if (!filep.is_open()) {
        perror("Error opening input.txt");
        exit(1);
    }
    filep >> len >> len_h;
    if(len > MAX_PROPERTIES) len = MAX_PROPERTIES;
    if(len_h > MAX_PROPERTIES) len_h = MAX_PROPERTIES;
    for (int i = 0; i < len; i++) {
        filep >> app[i].price >> app[i].rooms >> app[i].bathrooms >> app[i].kitchens >> app[i].parking_space;
    }
    for (int i = 0; i < len_h; i++) {
        filep >> house[i].price >> house[i].rooms >> house[i].bathrooms >> house[i].kitchens >> house[i].yard_size;
    }
    filep.close();
}

void load_favourites(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    string type;
    while (file >> type) {
        if (type == "A" && fav_apts_count < MAX_PROPERTIES) {
            file >> fav_apts[fav_apts_count].price
                 >> fav_apts[fav_apts_count].rooms
                 >> fav_apts[fav_apts_count].bathrooms
                 >> fav_apts[fav_apts_count].kitchens
                 >> fav_apts[fav_apts_count].parking_space;
            fav_apts_count++;
        } else if (type == "H" && fav_houses_count < MAX_PROPERTIES) {
            file >> fav_houses[fav_houses_count].price
                 >> fav_houses[fav_houses_count].rooms
                 >> fav_houses[fav_houses_count].bathrooms
                 >> fav_houses[fav_houses_count].kitchens
                 >> fav_houses[fav_houses_count].yard_size;
            fav_houses_count++;
        }
    }
    file.close();
}

void save_favourites(const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) return;
    for (int i = 0; i < fav_apts_count; i++) {
        file << "A " << fav_apts[i].price << ' ' << fav_apts[i].rooms << ' ' << fav_apts[i].bathrooms << ' '
             << fav_apts[i].kitchens << ' ' << fav_apts[i].parking_space << '\n';
    }
    for (int i = 0; i < fav_houses_count; i++) {
        file << "H " << fav_houses[i].price << ' ' << fav_houses[i].rooms << ' ' << fav_houses[i].bathrooms << ' '
             << fav_houses[i].kitchens << ' ' << fav_houses[i].yard_size << '\n';
    }
    file.close();
}

void add_favourite_apartment(int index) {
    if (index >= 0 && index < len && fav_apts_count < MAX_PROPERTIES) {
        fav_apts[fav_apts_count++] = app[index];
        save_favourites("fav.txt");
        cout << GREEN << "Apartment added to favourites.\n" << RESET;
    }
}

void add_favourite_house(int index) {
    if (index >= 0 && index < len_h && fav_houses_count < MAX_PROPERTIES) {
        fav_houses[fav_houses_count++] = house[index];
        save_favourites("fav.txt");
        cout << GREEN << "House added to favourites.\n" << RESET;
    }
}

void show_favourites() {
    cout << YELLOW << "--- Favourite Apartments ---\n" << RESET;
    for (int i = 0; i < fav_apts_count; i++) {
        cout << "[" << i << "]\n";
        fav_apts[i].print();
    }
    cout << YELLOW << "--- Favourite Houses ---\n" << RESET;
    for (int i = 0; i < fav_houses_count; i++) {
        cout << "[" << i << "]\n";
        fav_houses[i].print();
    }
}

void show_apartments() {
    if (len == 0) {
        cout << "No apartments available.\n";
        return;
    }
    cout << BLUE << "Available Apartments:\n" << RESET;
    for (int i = 0; i < len; i++) {
        cout << "[" << i << "]\n";
        app[i].print();
    }
}

void show_houses() {
    if (len_h == 0) {
        cout << "No houses available.\n";
        return;
    }
    cout << BLUE << "Available Houses:\n" << RESET;
    for (int i = 0; i < len_h; i++) {
        cout << "[" << i << "]\n";
        house[i].print();
    }
}

void buy_apartment(int index) {
    if (index < 0 || index >= len || boughts_count >= MAX_PROPERTIES) {
        cout << RED << "Invalid purchase.\n" << RESET;
        return;
    }
    boughts[boughts_count++] = app[index];
    cout << GREEN << "Apartment purchased successfully!\n" << RESET;
}

void buy_house(int index) {
    if (index < 0 || index >= len_h || boughts_houses_count >= MAX_PROPERTIES) {
        cout << RED << "Invalid purchase.\n" << RESET;
        return;
    }
    boughts_houses[boughts_houses_count++] = house[index];
    cout << GREEN << "House purchased successfully!\n" << RESET;
}

void menu() {
    int option;
    do {
        cout << MAGENTA << "\n--- Property Marketplace ---\n" << RESET;
        cout << "1. Show available apartments\n";
        cout << "2. Show available houses\n";
        cout << "3. Buy apartment\n";
        cout << "4. Buy house\n";
        cout << "5. Add apartment to favourites\n";
        cout << "6. Add house to favourites\n";
        cout << "7. Show favourites\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        option = get_valid_int(0, 7);

        switch (option) {
            case 1: show_apartments(); break;
            case 2: show_houses(); break;
            case 3:
                show_apartments();
                cout << "Index to buy: ";
                buy_apartment(get_valid_int(0, len - 1));
                break;
            case 4:
                show_houses();
                cout << "Index to buy: ";
                buy_house(get_valid_int(0, len_h - 1));
                break;
            case 5:
                show_apartments();
                cout << "Index to favorite: ";
                add_favourite_apartment(get_valid_int(0, len - 1));
                break;
            case 6:
                show_houses();
                cout << "Index to favorite: ";
                add_favourite_house(get_valid_int(0, len_h - 1));
                break;
            case 7: show_favourites(); break;
            case 0: cout << "Exiting...\n"; break;
        }

    } while (option != 0);
}

int main() {
    load_properties("input.txt");
    load_favourites("fav.txt");
    menu();
    return 0;
}
