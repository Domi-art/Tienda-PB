#include <bits/stdc++.h>
using namespace std;

struct Usuario {
    int idUsuario;
    string nombre;
    string correoElectronico;
    string contraseña;
    string direccion;
    string metodoDePago;
};

struct Producto {
    int idProducto;
    string nombre;
    string descripcion;
    double precio;
    int stock;
};

struct Comentario {
    int idComentario;
    string producto;
    string usuario;
    string comentario;
    string fecha;
};

struct ProductoEnCarrito {
    int idProducto;
    string nombre;
    double precio;
    int cantidad;
};

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    string item;
    stringstream ss(s);
    while (getline(ss, item, delim)) elems.push_back(item);
    return elems;
}

vector<Usuario> cargarUsuarios(const string &ruta) {
    vector<Usuario> usuarios;
    ifstream f(ruta);
    if (!f.is_open()) {
        cerr << "No se pudo abrir " << ruta << endl;
        return usuarios;
    }
    string linea;
    getline(f, linea);
    while (getline(f, linea)) {
        if (linea.empty()) continue;
        auto campos = split(linea, ',');
        if (campos.size() < 6) continue;
        Usuario u;
        u.idUsuario = stoi(campos[0]);
        u.nombre = campos[1];
        u.correoElectronico = campos[2];
        u.contraseña = campos[3];
        u.direccion = campos[4];
        if (campos.size() == 6) {
            u.metodoDePago = campos[5];
        } else {
            string mp = campos[5];
            for (size_t i = 6; i < campos.size(); ++i) { mp += "," + campos[i]; }
            u.metodoDePago = mp;
        }
        usuarios.push_back(u);
    }
    f.close();
    return usuarios;
}

vector<Producto> cargarProductos(const string &ruta) {
    vector<Producto> productos;
    ifstream f(ruta);
    if (!f.is_open()) {
        cerr << "No se pudo abrir " << ruta << endl;
        return productos;
    }
    string linea;
    getline(f, linea);
    while (getline(f, linea)) {
        if (linea.empty()) continue;
        auto campos = split(linea, ',');
        if (campos.size() < 5) continue;
        Producto p;
        p.idProducto = stoi(campos[0]);
        p.nombre = campos[1];
        p.descripcion = campos[2];
        p.precio = stod(campos[3]);
        p.stock = stoi(campos[4]);
        productos.push_back(p);
    }
    f.close();
    return productos;
}

vector<Comentario> cargarComentarios(const string &ruta) {
    vector<Comentario> comentarios;
    ifstream f(ruta);
    if (!f.is_open()) {
        cerr << "No se pudo abrir " << ruta << endl;
        return comentarios;
    }
    string linea;
    getline(f, linea);
    while (getline(f, linea)) {
        if (linea.empty()) continue;
        size_t pos1 = linea.find(',');
        if (pos1 == string::npos) continue;
        size_t pos2 = linea.find(',', pos1 + 1);
        if (pos2 == string::npos) continue;
        size_t pos3 = linea.find(',', pos2 + 1);
        if (pos3 == string::npos) continue;
        size_t posLast = linea.rfind(',');
        if (posLast == string::npos || posLast <= pos3) continue;

        Comentario c;
        c.idComentario = stoi(linea.substr(0, pos1));
        c.producto = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        c.usuario = linea.substr(pos2 + 1, pos3 - pos2 - 1);
        c.comentario = linea.substr(pos3 + 1, posLast - pos3 - 1);
        c.fecha = linea.substr(posLast + 1);
        comentarios.push_back(c);
    }
    f.close();
    return comentarios;
}

bool fechaMayorOIgual(const string &a, const string &b) {
    return a >= b;
}

void listarProductosPorStock(const vector<Producto> &productos, int umbral = 15) {
    cout << "\nProductos con stock por debajo de " << umbral << " unidades:\n";
    cout << left << setw(6) << "ID" << setw(25) << "Nombre" << setw(8) << "Precio" << setw(8) << "Stock" << "\n";
    cout << string(60, '-') << "\n";
    for (const auto &p : productos) {
        if (p.stock < umbral) {
            cout << setw(6) << p.idProducto
                 << setw(25) << p.nombre
                 << setw(8) << fixed << setprecision(2) << p.precio
                 << setw(8) << p.stock << "\n";
        }
    }
}

void comentariosDesdeFecha(const vector<Comentario> &comentarios, const string &fechaDesde) {
    cout << "\nComentarios desde " << fechaDesde << " (inclusive):\n";
    for (const auto &c : comentarios) {
        if (fechaMayorOIgual(c.fecha, fechaDesde)) {
            cout << "[" << c.fecha << "] (" << c.producto << ") " << c.usuario << ": " << c.comentario << "\n";
        }
    }
}

void listarUsuariosSinClave(const vector<Usuario> &usuarios) {
    cout << "\nLista de usuarios (contraseñas ocultas):\n";
    cout << left << setw(6) << "ID" << setw(25) << "Nombre" << setw(30) << "Correo" << setw(30) << "Dirección" << setw(15) << "MétodoPago" << "\n";
    cout << string(110, '-') << "\n";
    for (const auto &u : usuarios) {
        cout << setw(6) << u.idUsuario
             << setw(25) << u.nombre
             << setw(30) << u.correoElectronico
             << setw(30) << u.direccion
             << setw(15) << u.metodoDePago << "\n";
    }
}

void guardarCarritoEnArchivo(int idUsuario, const vector<ProductoEnCarrito> &carrito) {
    string nombreArchivo = "Carrito_" + to_string(idUsuario) + ".txt";
    ofstream f(nombreArchivo);
    if (!f.is_open()) {
        cerr << "No se pudo crear archivo de carrito.\n";
        return;
    }
    f << "idProducto,nombre,precio,cantidad,subtotal\n";
    for (const auto &it : carrito) {
        double subtotal = it.precio * it.cantidad;
        f << it.idProducto << "," << it.nombre << "," << fixed << setprecision(2) << it.precio
          << "," << it.cantidad << "," << fixed << setprecision(2) << subtotal << "\n";
    }
    f.close();
    cout << "Carrito guardado en archivo: " << nombreArchivo << "\n";
}

vector<ProductoEnCarrito> cargarCarritoDesdeArchivo(int idUsuario) {
    vector<ProductoEnCarrito> carrito;
    string nombreArchivo = "Carrito_" + to_string(idUsuario) + ".txt";
    ifstream f(nombreArchivo);
    if (!f.is_open()) {
        return carrito;
    }
    string linea;
    getline(f, linea);
    while (getline(f, linea)) {
        if (linea.empty()) continue;
        auto campos = split(linea, ',');
        if (campos.size() < 5) continue;
        ProductoEnCarrito p;
        p.idProducto = stoi(campos[0]);
        p.nombre = campos[1];
        p.precio = stod(campos[2]);
        p.cantidad = stoi(campos[3]);
        carrito.push_back(p);
    }
    f.close();
    return carrito;
}

void mostrarCarrito(const vector<ProductoEnCarrito> &carrito) {
    if (carrito.empty()) {
        cout << "El carrito está vacío.\n";
        return;
    }
    cout << "\nContenido del carrito:\n";
    cout << left << setw(6) << "ID" << setw(25) << "Nombre" << setw(8) << "Precio" << setw(8) << "Cant" << setw(10) << "Subtotal" << "\n";
    cout << string(60, '-') << "\n";
    double total = 0.0;
    for (const auto &p : carrito) {
        double sub = p.precio * p.cantidad;
        cout << setw(6) << p.idProducto
             << setw(25) << p.nombre
             << setw(8) << fixed << setprecision(2) << p.precio
             << setw(8) << p.cantidad
             << setw(10) << fixed << setprecision(2) << sub << "\n";
        total += sub;
    }
    cout << string(60, '-') << "\n";
    cout << "Total carrito: " << fixed << setprecision(2) << total << "\n";
}

int indiceProductoPorId(vector<Producto> &productos, int id) {
    for (size_t i = 0; i < productos.size(); ++i) {
        if (productos[i].idProducto == id) return (int)i;
    }
    return -1;
}

void guardarProductosArchivo(const string &ruta, const vector<Producto> &productos) {
    ofstream f(ruta);
    if (!f.is_open()) {
        cerr << "No se pudo abrir para guardar productos: " << ruta << endl;
        return;
    }
    f << "idProducto,nombre,descripcion,precio,stock\n";
    for (const auto &p : productos) {
        f << p.idProducto << "," << p.nombre << "," << p.descripcion << "," << fixed << setprecision(2) << p.precio << "," << p.stock << "\n";
    }
    f.close();
    cout << "Productos guardados en " << ruta << " (si editable).\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const string rutaUsuarios = "Usuarios.txt";
    const string rutaProductos = "Productos.txt";
    const string rutaComentarios = "Comentarios.txt";

    auto usuarios = cargarUsuarios(rutaUsuarios);
    auto productos = cargarProductos(rutaProductos);
    auto comentarios = cargarComentarios(rutaComentarios);

    cout << "Iniciar sesión\n";
    string email, pass;
    cout << "Correo: ";
    getline(cin, email);
    cout << "Contraseña: ";
    getline(cin, pass);

    Usuario usuarioActual;
    bool encontrado = false;
    for (const auto &u : usuarios) {
        if (u.correoElectronico == email && u.contraseña == pass) {
            usuarioActual = u;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Usuario inválido.\n";
        return 0;
    }

    cout << "\nBienvenido, " << usuarioActual.nombre << " (ID " << usuarioActual.idUsuario << ")\n";

    auto carrito = cargarCarritoDesdeArchivo(usuarioActual.idUsuario);

    while (true) {
        cout << "\n--- Menú ---\n";
        cout << "1) Listar productos con stock < 15\n";
        cout << "2) Mostrar comentarios desde fecha (YYYY-MM-DD)\n";
        cout << "3) Listar usuarios (no mostrar contraseñas)\n";
        cout << "4) Añadir producto al carrito\n";
        cout << "5) Listar productos del carrito\n";
        cout << "6) Guardar carrito en archivo\n";
        cout << "7) Salir\n";
        cout << "Seleccione una opción: ";
        string opcion;
        getline(cin, opcion);
        if (opcion == "1") {
            listarProductosPorStock(productos, 15);
        } else if (opcion == "2") {
            cout << "Ingrese fecha desde (YYYY-MM-DD): ";
            string fecha;
            getline(cin, fecha);
            comentariosDesdeFecha(comentarios, fecha);
        } else if (opcion == "3") {
            listarUsuariosSinClave(usuarios);
        } else if (opcion == "4") {
            cout << "Ingrese ID del producto a añadir: ";
            string s;
            getline(cin, s);
            int id = stoi(s);
            int idx = indiceProductoPorId(productos, id);
            if (idx == -1) {
                cout << "Producto no encontrado.\n";
                continue;
            }
            Producto &p = productos[idx];
            cout << "Producto: " << p.nombre << " | Stock disponible: " << p.stock << " | Precio: " << fixed << setprecision(2) << p.precio << "\n";
            cout << "Ingrese cantidad: ";
            string sc;
            getline(cin, sc);
            int cantidad = stoi(sc);
            if (cantidad <= 0) {
                cout << "Cantidad inválida.\n";
                continue;
            }
            if (cantidad > p.stock) {
                cout << "No hay suficiente stock. Disponible: " << p.stock << "\n";
                continue;
            }
            bool encontradoEnCarrito = false;
            for (auto &cp : carrito) {
                if (cp.idProducto == p.idProducto) {
                    cp.cantidad += cantidad;
                    encontradoEnCarrito = true;
                    break;
                }
            }
            if (!encontradoEnCarrito) {
                ProductoEnCarrito pc;
                pc.idProducto = p.idProducto;
                pc.nombre = p.nombre;
                pc.precio = p.precio;
                pc.cantidad = cantidad;
                carrito.push_back(pc);
            }
            p.stock -= cantidad;
            cout << "Producto añadido al carrito.\n";
        } else if (opcion == "5") {
            mostrarCarrito(carrito);
        } else if (opcion == "6") {
            guardarCarritoEnArchivo(usuarioActual.idUsuario, carrito);
        } else if (opcion == "7") {
            cout << "Saliendo. ¡Hasta luego!\n";
            guardarCarritoEnArchivo(usuarioActual.idUsuario, carrito);
            break;
        } else {
            cout << "Opción inválida.\n";
        }
    }

    return 0;
}