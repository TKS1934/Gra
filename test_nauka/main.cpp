#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>
#include <numeric>

using namespace std;

int main() {
    // PUNKTY DODATKOWE (maksymalnie 4, lacznie nie wiecej niz 40)
    // 1: definicje typow z wykorzystaniem using albo typedef
    // 1: poprawne przekazywanie przez referencje
    // 1: poprawne uzycie modyfikatora const
    // 1: poprawne wykorzystanie algorytmow STL albo wyrazen lambda

    using StrVec = vector<string>;

    StrVec rows = {
        "id=A1 temp=21.5 status=OK",
        "id=A2 temp=24.0 status=WARN",
        "id=B1 temp=19.5 status=OK",
        "id=B2 temp=31.0 status=FAIL",
        "id=C1 temp=28.0 status=WARN"
    };

    // Wybierz z rows tylko linie ze statusem FAIL albo WARN.
    // Uzyj std::regex tylko w tym miejscu i zapisz wynik w wektorze selected.
    // PUNKTY: 7
    StrVec selected;
    regex stat_reg(R"(.*(WARN|FAIL))");
    for (const auto& r: rows) {
        if (regex_match(r, stat_reg)) {
            selected.push_back(r);
            cout << r << endl;
        }
    }

    // Z kazdej linii wyodrebnij identyfikator po "id=" i zapisz w wektorze ids.
    // Wystarczy uzyc find() oraz substr().
    // PUNKTY: 7
    StrVec ids;

    // Z kazdej linii wyodrebnij temperature po "temp=" i zapisz jako double w wektorze temps.
    // Mozesz uzyc stod().
    // PUNKTY: 8
    using DblVec = vector<double>;
    DblVec temps;

    // Oblicz liczbe wystapien kazdego statusu z uzyciem std::map<string,int>.
    // PUNKTY: 8
    using StrInpMap = map<string,int>;
    StrInpMap status_count;

    return 0;
}
