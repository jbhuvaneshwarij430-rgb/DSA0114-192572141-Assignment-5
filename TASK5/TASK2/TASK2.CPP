#include <iostream>
#include <stdexcept>
using namespace std;

class ClaimError : public runtime_error {
public:
    ClaimError(const char* msg) : runtime_error(msg) {}
};

double approve(double billed, double cover) {

    if (billed < 0 || cover < 0) {
        throw invalid_argument("Negative amount");
    }

    if (billed > cover) {
        throw ClaimError("Exceeds cover");
    }

    return billed;
}

double settle(double billed, double cover) {

    try {
        return approve(billed, cover);
    }
    catch (const ClaimError& e) {
        cout << "Audit: " << e.what() << endl;
        throw;
    }
}

int main() {

    double cases[3][2] = {
        {40000, 50000},
        {60000, 50000},
        {-1, 50000}
    };

    for (int i = 0; i < 3; i++) {

        try {
            double paid = settle(cases[i][0], cases[i][1]);
            cout << "Approved: " << paid << endl;
        }
        catch (const ClaimError& e) {
            cout << "Claim rejected: " << e.what() << endl;
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
