#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

using std::string;
using std::exception;

/**
 * @brief
 * Klasa bazowa dla tworzonych w aplikacji wyjątków
 */
class MyException : public exception {
protected:
    string msg;

public:
    /// Konstruktor
    MyException(string msg_) : msg(msg_) {}

    /// Metoda zwraca wiadomość z wyjątku
    const string& getMsg() {
        return msg;
    }

    ~MyException() throw() {}
};

/**
 * @brief
 * Klasa wyjątku powstającego przy nieudanym tworzeniu mapy
 */
class MapException : public MyException {
public:
    MapException(string msg_) : MyException(msg_) {}
    ~MapException() throw() {}
};

/**
 * @brief
 * Klasa wyjątku powstającego przy nieudanych operacjach na pojazdach
 */
class VehicleException : public MyException {
public:
    VehicleException(string msg_) : MyException(msg_) {}
};

#endif // EXCEPTIONS_H
