#include <iostream>
#include <mysql.h>
  // MySQL Library for C++
#include <vector>
#include <string>
#include <fstream>  // Add this for file handling


using namespace std;

struct Product {
    int id;
    string name;
    int quantity;
    double price;
};
vector<Product> inventory;

MYSQL* connectDB() {
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "your_password", "inventory_db", 3306, NULL, 0);

    if (conn) {
        cout << "Database Connected!\n";
    } else {
        cout << "Failed to connect: " << mysql_error(conn) << endl;
    }
    return conn;
}

void addProduct(MYSQL* conn) {
    string name;
    int quantity;
    double price;

    cout << "Enter Product Name: ";
    cin >> name;
    cout << "Enter Quantity: ";
    cin >> quantity;
    cout << "Enter Price: ";
    cin >> price;

    string query = "INSERT INTO products (name, quantity, price) VALUES ('" + name + "', " + to_string(quantity) + ", " + to_string(price) + ")";
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Product Added Successfully!\n";
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}
void viewProducts(MYSQL* conn) {
    string query = "SELECT * FROM products";
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        
        cout << "\n------------------------------------\n";
        cout << "| ID  | Name   | Quantity | Price |\n";
        cout << "------------------------------------\n";
        
        while ((row = mysql_fetch_row(res))) {
            cout << "| " << row[0] << "  | " << row[1] << "  | " << row[2] << "       | " << row[3] << " |\n";
        }
        cout << "------------------------------------\n";
        mysql_free_result(res);
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}


void searchProduct(MYSQL* conn, int id) {
    string query = "SELECT * FROM products WHERE id = " + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row) {
            cout << "Product Found: " << row[1] << " | Quantity: " << row[2] << " | Price: " << row[3] << endl;
        } else {
            cout << "Product not found!\n";
        }
        mysql_free_result(res);
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}


void loadProducts() {
    ifstream file("inventory.txt");
    if (!file) {
        cout << "No inventory file found, starting fresh...\n";
        return;
    }

    Product p;
    while (file >> p.id >> p.name >> p.quantity >> p.price) {
        inventory.push_back(p);
    }
    file.close();
}

void deleteProduct(MYSQL* conn, int id) {
    string query = "DELETE FROM products WHERE id = " + to_string(id);
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Product Deleted Successfully!\n";
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}



int main() {
    MYSQL* conn = connectDB();
    if (!conn) return 1;

    int choice, id;

    while (true) {
        cout << "\nInventory Management System (MySQL)\n";
        cout << "1. Add Product\n2. View Products\n3. Search Product\n4. Delete Product\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addProduct(conn); break;
            case 2: viewProducts(conn); break;
            case 3:
                cout << "Enter Product ID to search: ";
                cin >> id;
                searchProduct(conn, id);
                break;
            case 4:
                cout << "Enter Product ID to delete: ";
                cin >> id;
                deleteProduct(conn, id);
                break;
            case 5:
                mysql_close(conn);
                exit(0);
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}
