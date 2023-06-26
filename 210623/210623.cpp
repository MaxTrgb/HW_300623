#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

const int MAX_PRODUCTS = 100; //maximum number of products helps prevent potential errors

// base structure
struct Goods {
    char name[100];
    char shop[100];
    double price;
    double amount;
    char measurement[20];
};

// Comparison function to sort products by name
bool compareByName(const Goods& a, const Goods& b) {

    return strcmp(a.name, b.name) < 0; //'strcmp' returns an integer value indicating the result of the comparison
}

// Comparison function to sort products by shop name
bool compareByShop(const Goods& a, const Goods& b) {

    return strcmp(a.shop, b.shop) < 0;
}
// Function to add products to the file
void Create(string fname) {

    ofstream fout(fname, ios::binary | ios::app);
    char ch;
    Goods goods;

    do {
        cout << "Name: ";
        cin >> goods.name;
        cout << "Shop: ";
        cin >> goods.shop;
        cout << "Price: ";
        cin >> goods.price;
        cout << "Amount: ";
        cin >> goods.amount;
        cout << "Measurement: ";
        cin >> goods.measurement;

        if (!fout.write((char*)&goods, sizeof(Goods))) {

            cerr << "Error writing to file." << endl;
        }

        cout << "Continue (y/n): ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');

    cout << endl;
    fout.close();
}
// Function to display all products in the file
void Display(string fname) {

    //'tellg' allows to determine current position of the input pointer within the file stream
    //used with 'seekg' to navigate within the file, calculate file sizes, etc.
    ifstream display(fname, ios::binary);
    display.seekg(0, ios::end); 

    int size = display.tellg(); 
    size = size / sizeof(Goods);

    display.seekg(0, ios::beg);

    Goods goodsList[MAX_PRODUCTS];
    display.read((char*)goodsList, size * sizeof(Goods));

    for (int i = 0; i < size; i++) {

        cout << endl;
        cout << goodsList[i].name << endl;
        cout << goodsList[i].shop << endl;
        cout << goodsList[i].price << endl;
        cout << goodsList[i].amount << endl;
        cout << goodsList[i].measurement << endl;
        cout << "----------------------------" << endl;
    }
    display.close();
}
// Function to display products sold in a particular store
void DisplayByStore(string fname, const string& storeName) {

    ifstream fin(fname, ios::binary);

    Goods goods;
    bool found = false;

    while (fin.read((char*)&goods, sizeof(Goods))) {

        if (string(goods.shop) == storeName) {

            cout << endl;
            cout << goods.name << endl;
            cout << goods.shop << endl;
            cout << goods.price << endl;
            cout << goods.amount << endl;
            cout << goods.measurement << endl;
            cout << "----------------------------" << endl;
            found = true;
        }
    }

    fin.close();

    if (!found) {
        cout << "No products found for store: " << storeName << endl;
    }
}
// Function to delete a product from the file
void Delete(string fname, const string& name) {

    ifstream fin(fname, ios::binary);
    ofstream fout("temp.dat", ios::binary);

    Goods goods;
    while (fin.read((char*)&goods, sizeof(Goods))) {

        if (string(goods.name) != name) {

            fout.write((char*)&goods, sizeof(Goods));
        }
    }

    fin.close();
    fout.close();

    //update the file after deleting a product
    //'c_str' converts the fname string from the std::string type to a C-style string (const char*)
    remove(fname.c_str());
    rename("temp.dat", fname.c_str());
}
// Function to edit details of a product in the file
void Edit(string fname, const string& name) {

    fstream file(fname, ios::binary | ios::in | ios::out); //'in|out' allows both reading and overwriting

    if (!file) {

        cerr << "Error opening file." << endl;
        return;
    }

    Goods goods;
    bool found = false;

    while (file.read((char*)&goods, sizeof(Goods))) {
        if (string(goods.name) == name) {
            cout << "Enter new details for the product:" << endl;
            cout << "Name: ";
            cin >> goods.name;
            cout << "Shop: ";
            cin >> goods.shop;
            cout << "Price: ";
            cin >> goods.price;
            cout << "Amount: ";
            cin >> goods.amount;
            cout << "Measurement: ";
            cin >> goods.measurement;

            file.seekp(-static_cast<int>(sizeof(Goods)), ios::cur); // position the put pointer at the appropriate location before writing 
            file.write((char*)&goods, sizeof(Goods));

            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Product not found." << endl;
    }

    file.close();
}
// Function to sort products in the file based on a given comparison function
void Sort(string fname, bool (*compareFunc)(const Goods&, const Goods&)) {

    ifstream fin(fname, ios::binary);
    fin.seekg(0, ios::end);

    int size = fin.tellg();
    size = size / sizeof(Goods);

    fin.seekg(0, ios::beg);

    Goods goodsList[MAX_PRODUCTS];
    fin.read((char*)goodsList, size * sizeof(Goods));

    fin.close();

    sort(goodsList, goodsList + size, compareFunc);

    ofstream fout(fname, ios::binary | ios::trunc); //'trunc' ensures that the file is cleared before any new content is written to it

    if (!fout) {
        cerr << "Error opening file." << endl;
        return;
    }

    fout.write((char*)goodsList, size * sizeof(Goods));

    fout.close();
}
//Main function
int main() {
    Goods goods;
    int option;
    string fname = "File.dat"; //working file

    //loop for the menu
    while (true) {

        cout << "\n1. Add product\n2. Delete product\n3. Edit product\n";
        cout << "4. Sort by product name\n5. Sort by store name\n";
        cout << "6. Display products\n7. Display information about the products sold in a particular store\n";
        cout << "8. Exit\n";
        cin >> option;

        if (option == 1) {

            Create(fname);
        }
        else if (option == 2) {

            string productName;
            cout << "Enter the name of the product to delete: ";
            cin >> productName;
            Delete(fname, productName);
        }
        else if (option == 3) {

            string productName;
            cout << "Enter the name of the product to edit: ";
            cin >> productName;
            Edit(fname, productName);
        }
        else if (option == 4) {

            Sort(fname, compareByName);
        }
        else if (option == 5) {

            Sort(fname, compareByShop);
        }
        else if (option == 6) {

            Display(fname);
        }
        else if (option == 7) {

            string storeName;
            cout << "Enter the name of the store: ";
            cin >> storeName;
            DisplayByStore(fname, storeName);
        }
        else if (option == 8) {
                        
            return 0;
        }
    }

    return 0;
}
