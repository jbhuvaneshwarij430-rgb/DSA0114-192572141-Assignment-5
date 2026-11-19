#include <iostream>
#include <string>
using namespace std;

class Bill {
protected:
    string patient;

public:
    Bill(string p) {
        patient = p;
    }

    virtual ~Bill() {}

    virtual double total() const = 0;

    virtual string type() const {
        return "Bill";
    }

    void print() const {
        cout << type() << " " << patient
             << ": Rs. " << total() << endl;
    }
};

class OutPatient : public Bill {
    double consultation, tests;

public:
    OutPatient(string p, double c, double t)
        : Bill(p) {
        consultation = c;
        tests = t;
    }

    double total() const override {
        return consultation + tests;
    }

    string type() const override {
        return "OP";
    }
};

class InPatient : public Bill {
    int days;
    double perDay, surgery;

public:
    InPatient(string p, int d, double rate, double s)
        : Bill(p) {
        days = d;
        perDay = rate;
        surgery = s;
    }

    double total() const override {
        return days * perDay + surgery;
    }

    string type() const override {
        return "IP";
    }
};

int main() {

    Bill* bills[2];

    bills[0] = new OutPatient("Ravi", 500, 1200);
    bills[1] = new InPatient("Meena", 3, 2500, 40000);

    double sum = 0;

    for (int i = 0; i < 2; i++) {
        bills[i]->print();
        sum += bills[i]->total();
    }

    cout << "Total: Rs. " << sum << endl;

    for (int i = 0; i < 2; i++) {
        delete bills[i];
    }

    return 0;
}
