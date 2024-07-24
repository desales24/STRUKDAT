#include <iostream>
#include <mysql/mysql.h>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "uas_123";
const char* dbname = "cpp_toko";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Berhasil terhubung ke database." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

// Function to delete user by username
void delete_user(const string& username) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM users WHERE username = '" << username << "'";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Penghapusan pengguna gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Pengguna berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

// Function to delete admin by username
void delete_admin(const string& username) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM admins WHERE username = '" << username << "'";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Penghapusan admin gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Admin berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

// Function to handle login attempts
bool handle_login(const string& username, const string& password, bool is_admin) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        if (is_admin) {
            query << "SELECT * FROM admins WHERE username = '" << username << "' AND password = '" << password << "'";
        } else {
            query << "SELECT * FROM users WHERE username = '" << username << "' AND password = '" << password << "'";
        }
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Login gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        bool login_success = (res && mysql_num_rows(res) > 0);
        mysql_free_result(res);
        mysql_close(conn);
        return login_success;
    }
    return false;
}

void register_user(const string& username, const string& password) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO users (username, password) VALUES ('" << username << "', '" << password << "')";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Pendaftaran pengguna gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Pengguna berhasil didaftarkan." << endl;
        }
        mysql_close(conn);
    }
}

void register_admin(const string& username, const string& password) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO admins (username, password) VALUES ('" << username << "', '" << password << "')";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Pendaftaran admin gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Admin berhasil didaftarkan." << endl;
        }
        mysql_close(conn);
    }
}

void create_product(const string& name, const string& category, const string& price, const string& stock) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO produk (name, category, price, stock) VALUES ('" << name << "', '" << category << "', '" << price << "', '" << stock << "')";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Penambahan produk gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Produk berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

void get_products() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM produk")) {
            cerr << "SELECT gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Nama: " << row[1] << ", Kategori: " << row[2] << ", Harga: " << row[3] << ", Stok: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_product(int product_id, const string& name, const string& category, const string& price, const string& stock) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE produk SET name = '" << name << "', category = '" << category << "', price = '" << price << "', stock = '" << stock << "' WHERE id = " << product_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Produk berhasil diperbarui." << endl;
        }
        mysql_close(conn);
    }
}

void delete_product(int product_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM produk WHERE id = " << product_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Produk berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

void buy_product(const string& username) {
    int product_id, quantity;
    cout << "Masukkan ID produk yang akan dibeli: ";
    cin >> product_id;
    cout << "Masukkan jumlah yang akan dibeli: ";
    cin >> quantity;

    MYSQL* conn = connect_db();
    if (conn) {
        // Update stock
        stringstream update_stock_query;
        update_stock_query << "UPDATE produk SET stock = stock - " << quantity << " WHERE id = " << product_id;
        if (mysql_query(conn, update_stock_query.str().c_str())) {
            cerr << "Pembelian gagal: " << mysql_error(conn) << endl;
        } else {
            // Insert into penjualan
            stringstream insert_sale_query;
            insert_sale_query << "INSERT INTO penjualan (product_id, username, jumlah) VALUES (" << product_id << ", '" << username << "', " << quantity << ")";
            if (mysql_query(conn, insert_sale_query.str().c_str())) {
                cerr << "Pembelian gagal: " << mysql_error(conn) << endl;
            } else {
                cout << "Produk berhasil dibeli." << endl;
            }
        }
        mysql_close(conn);
    }
}

void edit_purchase(const string& username) {
    int purchase_id, new_quantity;
    cout << "Masukkan ID pembelian yang akan diedit: ";
    cin >> purchase_id;
    cout << "Masukkan jumlah baru: ";
    cin >> new_quantity;

    MYSQL* conn = connect_db();
    if (conn) {
        // Fetch the current quantity of the purchase
        MYSQL_RES* res;
        MYSQL_ROW row;
        int old_quantity;
        int product_id;

        stringstream query;
        query << "SELECT product_id, jumlah FROM penjualan WHERE id = " << purchase_id << " AND username = '" << username << "'";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Gagal mengambil data pembelian: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            row = mysql_fetch_row(res);
            product_id = stoi(row[0]);
            old_quantity = stoi(row[1]);
        } else {
            cerr << "Pembelian tidak ditemukan." << endl;
            mysql_free_result(res);
            mysql_close(conn);
            return;
        }
        mysql_free_result(res);

        // Update the purchase quantity
        query.str("");
        query << "UPDATE penjualan SET jumlah = " << new_quantity << " WHERE id = " << purchase_id << " AND username = '" << username << "'";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Gagal memperbarui data pembelian: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        // Update the stock of the product
        int quantity_change = new_quantity - old_quantity;
        stringstream update_stock_query;
        update_stock_query << "UPDATE produk SET stock = stock - " << quantity_change << " WHERE id = " << product_id;
        if (mysql_query(conn, update_stock_query.str().c_str())) {
            cerr << "Gagal memperbarui stok produk: " << mysql_error(conn) << endl;
        } else {
            cout << "Pembelian berhasil diperbarui dan stok produk telah diperbarui." << endl;
        }

        mysql_close(conn);
    }
}

void view_purchase_history(const string& username) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "SELECT p.id, pr.name, pr.category, pr.price, p.jumlah FROM penjualan p JOIN produk pr ON p.product_id = pr.id WHERE p.username = '" << username << "'";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "Gagal melihat riwayat pembelian: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID Pembelian: " << row[0] << ", Nama Produk: " << row[1] << ", Kategori: " << row[2] << ", Harga: " << row[3] << ", Jumlah: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void user_menu(const string& username) {
    int choice;
    while (true) {
        cout << "\nMenu Pengguna:\n";
        cout << "1. Lihat Produk\n";
        cout << "2. Beli Produk\n";
        cout << "3. Lihat Riwayat Pembelian\n";
        cout << "4. Edit Pembelian\n";
        cout << "5. Logout\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1:
                get_products();
                break;
            case 2:
                buy_product(username);
                break;
            case 3:
                view_purchase_history(username);
                break;
            case 4:
                edit_purchase(username);
                break;
            case 5:
                return;
            default:
                cout << "Pilihan tidak valid. Coba lagi." << endl;
        }
    }
}

void admin_menu() {
    int choice;
    while (true) {
        cout << "\nMenu Admin:\n";
        cout << "1. Lihat Produk\n";
        cout << "2. Tambah Produk\n";
        cout << "3. Update Produk\n";
        cout << "4. Hapus Produk\n";
        cout << "5. Logout\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1:
                get_products();
                break;
            case 2: {
                string name, category, price, stock;
                cout << "Masukkan nama produk: ";
                cin >> name;
                cout << "Masukkan kategori produk: ";
                cin >> category;
                cout << "Masukkan harga produk: ";
                cin >> price;
                cout << "Masukkan stok produk: ";
                cin >> stock;
                create_product(name, category, price, stock);
                break;
            }
            case 3: {
                int product_id;
                string name, category, price, stock;
                cout << "Masukkan ID produk yang akan diperbarui: ";
                cin >> product_id;
                cout << "Masukkan nama produk baru: ";
                cin >> name;
                cout << "Masukkan kategori produk baru: ";
                cin >> category;
                cout << "Masukkan harga produk baru: ";
                cin >> price;
                cout << "Masukkan stok produk baru: ";
                cin >> stock;
                update_product(product_id, name, category, price, stock);
                break;
            }
            case 4: {
                int product_id;
                cout << "Masukkan ID produk yang akan dihapus: ";
                cin >> product_id;
                delete_product(product_id);
                break;
            }
            case 5:
                return;
            default:
                cout << "Pilihan tidak valid. Coba lagi." << endl;
        }
    }
}

void login_menu() {
    int choice;
    while (true) {
        cout << "\nLogin:\n";
        cout << "1. Login Pengguna\n";
        cout << "2. Login Admin\n";
        cout << "3. Kembali ke Menu Utama\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            string username, password;
            bool is_admin = (choice == 2);
            int attempts = 0;
            const int max_attempts = 3;
            while (attempts < max_attempts) {
                cout << "Masukkan username: ";
                cin >> username;
                cout << "Masukkan password: ";
                cin >> password;
                if (handle_login(username, password, is_admin)) {
                    if (is_admin) {
                        admin_menu();
                    } else {
                        user_menu(username);
                    }
                    break;
                } else {
                    attempts++;
                    int remaining_attempts = max_attempts - attempts;
                    cout << "Login gagal. Username atau password salah. Percobaan ke-" << attempts << " dari " << max_attempts << "." << endl;
                    if (remaining_attempts > 0) {
                        cout << "Sisa percobaan: " << remaining_attempts << endl;
                    }
                    if (attempts >= max_attempts) {
                        if (is_admin) {
                            delete_admin(username);
                        } else {
                            delete_user(username);
                        }
                        cout << "Akun telah dihapus setelah " << max_attempts << " kali percobaan login gagal." << endl;
                    }
                }
            }
        } else if (choice == 3) {
            break;
        } else {
            cout << "Pilihan tidak valid. Coba lagi." << endl;
        }
    }
}

void main_menu() {
    int choice;
    while (true) {
        cout << "\nMenu Utama:\n";
        cout << "1. Login\n";
        cout << "2. Daftar Pengguna\n";
        cout << "3. Daftar Admin\n";
        cout << "4. Exit\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1:
                login_menu();
                break;
            case 2: {
                string username, password;
                cout << "Masukkan username: ";
                cin >> username;
                cout << "Masukkan password: ";
                cin >> password;
                register_user(username, password);
                break;
            }
            case 3: {
                string username, password;
                cout << "Masukkan username: ";
                cin >> username;
                cout << "Masukkan password: ";
                cin >> password;
                register_admin(username, password);
                break;
            }
            case 4:
                exit(0);
            default:
                cout << "Pilihan tidak valid. Coba lagi." << endl;
        }
    }
}

int main() {
    main_menu();
    return 0;
}
