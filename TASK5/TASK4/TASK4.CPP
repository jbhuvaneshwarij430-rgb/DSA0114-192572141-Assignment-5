#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

using namespace std;

// BASE BILL CLASS

class Bill {
protected:
    string patient;

public:
    Bill(string p) {
        if (p.empty())
            throw invalid_argument("Patient name cannot be empty");

        patient = p;
    }

    virtual ~Bill() {}

    virtual double total() const = 0;
    virtual string type() const = 0;

    string getPatient() const {
        return patient;
    }
};

// OUTPATIENT BILL

class OutPatient : public Bill {
private:
    double consultation, tests;

public:
    OutPatient(string p, double c, double t)
        : Bill(p) {
        if (c < 0 || t < 0)
            throw invalid_argument("Negative OP charge");

        consultation = c;
        tests = t;
    }

    double total() const {
        return consultation + tests;
    }

    string type() const {
        return "OutPatient";
    }
};

// INPATIENT BILL

class InPatient : public Bill {
private:
    int days;
    double perDay, surgery;

public:
    InPatient(string p, int d, double r, double s)
        : Bill(p) {
        if (d < 0 || r < 0 || s < 0)
            throw invalid_argument("Invalid IP charge");

        days = d;
        perDay = r;
        surgery = s;
    }

    double total() const {
        return days * perDay + surgery;
    }

    string type() const {
        return "InPatient";
    }
};

// DAY CARE BILL

class DayCareBill : public Bill {
private:
    double packagePrice;

public:
    DayCareBill(string p, double price)
        : Bill(p) {
        if (price < 0)
            throw invalid_argument("Negative day-care charge");

        packagePrice = price;
    }

    double total() const {
        return packagePrice;
    }

    string type() const {
        return "DayCare";
    }
};

// HOME CARE BILL

class HomeCareBill : public Bill {
private:
    int visits;
    double chargePerVisit;
    double equipmentCharge;

public:
    HomeCareBill(string p, int v, double r, double e)
        : Bill(p) {
        if (v < 0 || r < 0 || e < 0)
            throw invalid_argument("Invalid home-care charge");

        visits = v;
        chargePerVisit = r;
        equipmentCharge = e;
    }

    double total() const {
        return visits * chargePerVisit + equipmentCharge;
    }

    string type() const {
        return "HomeCare";
    }
};

// BASE PAYER CLASS

class Payer {
public:
    virtual ~Payer() {}

    virtual double amountCovered(const Bill& b) const = 0;
    virtual string name() const = 0;
};

// SELF PAY

class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const {
        return 0.0;
    }

    string name() const {
        return "SelfPay";
    }
};

// INSURANCE

class Insurance : public Payer {
private:
    double coverLimit;
    double coPayPercentage;

public:
    Insurance(double limit, double copay) {
        if (limit < 0 || copay < 0 || copay > 100)
            throw invalid_argument("Invalid insurance policy");

        coverLimit = limit;
        coPayPercentage = copay;
    }

    double amountCovered(const Bill& b) const {
        double eligible =
            b.total() * (1.0 - coPayPercentage / 100.0);

        return min(eligible, coverLimit);
    }

    string name() const {
        return "Insurance";
    }
};

// GOVERNMENT SCHEME

class GovernmentScheme : public Payer {
public:
    double amountCovered(const Bill& b) const {
        if (b.type() == "OutPatient")
            return min(b.total(), 1000.0);

        if (b.type() == "InPatient")
            return min(b.total(), 30000.0);

        if (b.type() == "DayCare")
            return min(b.total(), 15000.0);

        return 0.0;
    }

    string name() const {
        return "GovernmentScheme";
    }
};

// CORPORATE PAYER

class CorporatePayer : public Payer {
private:
    double coveragePercentage;
    double maximumCover;

public:
    CorporatePayer(double p, double m) {
        if (p < 0 || p > 100 || m < 0)
            throw invalid_argument("Invalid corporate policy");

        coveragePercentage = p;
        maximumCover = m;
    }

    double amountCovered(const Bill& b) const {
        double eligible =
            b.total() * coveragePercentage / 100.0;

        return min(eligible, maximumCover);
    }

    string name() const {
        return "CorporatePayer";
    }
};

// EXCEPTION HANDLING

class ClaimError : public runtime_error {
public:
    ClaimError(const char* msg) : runtime_error(msg) {}
};

void validateClaim(double billed, double cover) {

    if (billed < 0 || cover < 0)
        throw invalid_argument("Negative claim or cover amount");

    if (billed > cover)
        throw ClaimError("Claim exceeds approved cover");
}

// PROCESS BILL

void processBill(const Bill& b, const Payer& p) {

    double billAmount = b.total();

    double covered = p.amountCovered(b);

    double patientPayment = billAmount - covered;

    cout << left
         << setw(15) << b.type()
         << setw(12) << b.getPatient()
         << setw(20) << p.name()
         << "Bill: " << setw(10) << billAmount
         << "Covered: " << setw(10) << covered
         << "Patient pays: " << patientPayment
         << endl;
}

// MAIN FUNCTION

int main() {

    cout << fixed << setprecision(2);

    vector<Bill*> bills;

    try {

        // CREATE BILLS

        bills.push_back(new OutPatient("Ravi", 500, 1200));

        bills.push_back(new InPatient("Meena", 3, 2500, 40000));

        bills.push_back(new DayCareBill("Arun", 12000));

        bills.push_back(new HomeCareBill("Priya", 5, 800, 2000));

        // CREATE PAYERS

        SelfPay selfPay;

        Insurance insurance(50000, 20);

        GovernmentScheme government;

        CorporatePayer corporate(80, 30000);

        vector<Payer*> payers;

        payers.push_back(&selfPay);
        payers.push_back(&insurance);
        payers.push_back(&government);
        payers.push_back(&corporate);

        // DISPLAY BILLING DETAILS

        cout << "============================================\n";
        cout << "          HOSPITAL BILLING SYSTEM           \n";
        cout << "============================================\n";

        for (size_t i = 0; i < bills.size(); i++) {

            for (size_t j = 0; j < payers.size(); j++) {

                processBill(*bills[i], *payers[j]);

            }
        }

        // EXCEPTION TEST

        cout << "\n============================================\n";
        cout << "          EXCEPTION HANDLING TEST           \n";
        cout << "============================================\n";

        try {

            validateClaim(60000, 50000);

            cout << "Claim approved\n";

        }
        catch (const ClaimError& e) {

            cout << "Caught exception: " << e.what() << endl;

        }

        // VALIDATION TEST

        try {

            OutPatient invalidBill("Kumar", -500, 1000);

            cout << "Invalid bill created\n";

        }
        catch (const exception& e) {

            cout << "Validation error: " << e.what() << endl;

        }

        cout << "\nProgram completed successfully.\n";

    }
    catch (const exception& e) {

        cout << "Error: " << e.what() << endl;

    }

    // RELEASE MEMORY

    for (size_t i = 0; i < bills.size(); i++) {
        delete bills[i];
    }

    bills.clear();

    return 0;
}
