
# ☕ Coffee Shop Management System

This is a C++ console-based application designed to simulate basic operations of a coffee shop.  
It was developed as a university-level project with a strong focus on **Object-Oriented Programming (OOP)** principles.

---

## 📌 Features

- Manage employees (add, remove, list)
- Handle product stock (add, remove, list)
- Process customer orders and generate receipts
- Apply loyalty discounts automatically
- Generate daily financial reports
- Manage special events with associated costs

---

## 🧠 Object-Oriented Design Highlights

This project is structured around key OOP concepts:

### ✅ **Inheritance & Polymorphism**
- `Angajat` is an abstract base class.
- `Barista`, `Manager`, and `Ospatar` inherit from `Angajat` and implement their own behavior.

### ✅ **Abstract Factory Pattern**
- `AngajatFactory` is used to instantiate the correct employee type based on the role.
- Ensures separation between object creation and usage.

### ✅ **Singleton Pattern**
- Classes like `ActiuniAngajati`, `Produse`, and `EvenimenteSpeciale` are singletons.
- Guarantees a single point of control for each domain (employees, products, events).

### ✅ **Separation of Concerns**
- `CSVWriter` handles file I/O operations, abstracting data persistence logic.
- Each domain (employees, products, events) is encapsulated in its own class.

---

## 🗃️ File Structure

```
/src/                     → Main source code
/data/                    → CSV files for storage
/res/                     → Resources (images, videos)
README.md
```

## 📸 Demo

![screenshot](res/image.jpg)  
🎥 [Demo Video](res/demo.mp4)

---

## 📂 Data Files

- `angajati.csv`, `salarii.csv` – employee info and salary mapping
- `produse.csv` – product names, prices, costs
- `comenzi.csv` – orders placed
- `fidelitate.csv` – customer loyalty tracking
- `rapoarte_financiare.csv` – daily report logs
- `evenimente_speciale.csv` – special events


