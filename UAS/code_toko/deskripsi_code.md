# Penjelasan Umum

Program ini menggunakan MySQL C API untuk berinteraksi dengan database. Fungsi utamanya termasuk login, pendaftaran, manajemen produk, dan riwayat pembelian. Berikut adalah penjelasan setiap bagian kode:

### 1. **Header dan Deklarasi**

```cpp
#include <iostream>
#include <mysql/mysql.h>
#include <sstream>
#include <iomanip>
#include <string>
```

- `#include <iostream>`: Untuk input dan output standar.
- `#include <mysql/mysql.h>`: Header untuk library MySQL C API.
- `#include <sstream>`: Untuk manipulasi string menggunakan `stringstream`.
- `#include <iomanip>`: Untuk manipulasi format output (tidak digunakan dalam kode ini).
- `#include <string>`: Untuk penggunaan tipe data `std::string`.

### 2. **Konstanta Database**

```cpp
const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "uas_123";
const char* dbname = "cpp_toko";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;
```

- Konstanta yang digunakan untuk mengatur koneksi ke database MySQL, termasuk hostname, user, password, nama database, port, dan flag klien.

### 3. **Fungsi `connect_db`**

```cpp
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
```

- Menginisialisasi koneksi ke database dan mengembalikan pointer ke objek koneksi `MYSQL`.
- Menggunakan `mysql_real_connect` untuk menghubungkan ke database dengan kredensial yang telah ditentukan.
- Menampilkan pesan sukses atau error berdasarkan hasil koneksi.

### 4. **Fungsi untuk Mengelola Pengguna dan Admin**

#### `delete_user` dan `delete_admin`

```cpp
void delete_user(const string& username) {
    // Connect to database and delete user
}

void delete_admin(const string& username) {
    // Connect to database and delete admin
}
```

- Menghapus pengguna atau admin berdasarkan `username` apabila terjadi kesalahan input password sebanyak tiga kali. 
- Menggunakan query SQL `DELETE` untuk menghapus entri dari tabel `users` atau `admins`.

#### `handle_login`

```cpp
bool handle_login(const string& username, const string& password, bool is_admin) {
    // Connect to database and verify login credentials
}
```

- Memeriksa kredensial login untuk pengguna atau admin.
- Menggunakan query SQL `SELECT` untuk memeriksa keberadaan entri yang cocok dalam tabel `users` atau `admins`.

#### `register_user` dan `register_admin`

```cpp
void register_user(const string& username, const string& password) {
    // Connect to database and register user
}

void register_admin(const string& username, const string& password) {
    // Connect to database and register admin
}
```

- Mendaftarkan pengguna atau admin baru dengan menggunakan query SQL `INSERT`.

### 5. **Fungsi untuk Mengelola Produk**

#### `create_product`

```cpp
void create_product(const string& name, const string& category, const string& price, const string& stock) {
    // Connect to database and insert new product
}
```

- Menambahkan produk baru ke tabel `produk` dengan query SQL `INSERT`.

#### `get_products`

```cpp
void get_products() {
    // Connect to database and retrieve products
}
```

- Mengambil dan menampilkan daftar produk dari tabel `produk` dengan query SQL `SELECT`.

#### `update_product`

```cpp
void update_product(int product_id, const string& name, const string& category, const string& price, const string& stock) {
    // Connect to database and update product details
}
```

- Memperbarui detail produk berdasarkan `product_id` dengan query SQL `UPDATE`.

#### `delete_product`

```cpp
void delete_product(int product_id) {
    // Connect to database and delete product
}
```

- Menghapus produk dari tabel `produk` berdasarkan `product_id` dengan query SQL `DELETE`.

### 6. **Fungsi untuk Mengelola Pembelian**

#### `buy_product`

```cpp
void buy_product(const string& username) {
    // Connect to database and process product purchase
}
```

- Mengurangi stok produk dan mencatat pembelian ke tabel `penjualan`.

#### `edit_purchase`

```cpp
void edit_purchase(const string& username) {
    // Connect to database and edit purchase details
}
```

- Mengubah jumlah pembelian dan memperbarui stok produk sesuai perubahan.

#### `view_purchase_history`

```cpp
void view_purchase_history(const string& username) {
    // Connect to database and view purchase history for a user
}
```

- Menampilkan riwayat pembelian pengguna dari tabel `penjualan`.

### 7. **Menu Utama dan Login**

#### `user_menu` dan `admin_menu`

```cpp
void user_menu(const string& username) {
    // Display user menu and handle user choices
}

void admin_menu() {
    // Display admin menu and handle admin choices
}
```

- Menampilkan menu sesuai jenis pengguna (user/admin) dan menangani pilihan dari menu tersebut.

#### `login_menu`

```cpp
void login_menu() {
    // Display login options and handle login attempts
}
```

- Menyediakan opsi login untuk pengguna atau admin dan mengelola upaya login yang gagal.

#### `main_menu`

```cpp
void main_menu() {
    // Display main menu and handle user choices
}
```

- Menu utama aplikasi yang memungkinkan login, pendaftaran pengguna atau admin, dan keluar dari aplikasi.

### 8. **Fungsi `main`**

```cpp
int main() {
    main_menu();
    return 0;
}
```

- Fungsi utama yang memulai aplikasi dengan memanggil `main_menu`.

### Kesimpulan

Kode ini adalah aplikasi berbasis teks untuk mengelola pengguna, admin, dan produk dalam sistem toko. Ia menggunakan MySQL untuk manajemen data dan memiliki menu untuk berbagai operasi seperti login, pendaftaran, manajemen produk, dan pembelian.