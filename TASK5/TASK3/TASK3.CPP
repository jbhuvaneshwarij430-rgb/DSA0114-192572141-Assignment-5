#include <iostream>
using namespace std;

struct OutPatientBill {
    double consultation, tests;

    double total() const {
        return consultation + tests;
    }
};

struct InPatientBill {
    int days;
    double rate, surgery;

    double total() const {
        return days * rate + surgery;
    }
};

int main() {

    OutPatientBill op = {500, 1200};

    InPatientBill ip = {3, 2500, 40000};

    double sum = 0;

    sum += op.total();
    sum += ip.total();

    cout << "OP Bill: " << op.total() << endl;
    cout << "IP Bill: " << ip.total() << endl;

    cout << "Total: " << sum << endl;

    return 0;
}
