#include "grid.h"
#include "cell.h"
#include "state.h"
#include "window.h"
using namespace std;

int main(int argc, char *argv[]) {

    (void) argc;
    (void) argv;
    
    cin.exceptions(ios::eofbit|ios::failbit);
    string cmd;
    Grid g;

    bool black = true;

    try {
        while (true) {
            cin >> cmd;

            if (cmd == "new") {
                int n; cin >> n;
                if ((n >= 4) && (n % 2 == 0)) {
                    g.init(n);
                    cout << g;
                }
            }
           else if (cmd == "play") {
                int r = 0, c = 0;
                cin >> r >> c;
                if (black) {
                    try {
                        g.setPiece(r, c, Colour::Black);
                        black = false;
                        cout << g;
                    } catch ( ... ) {
                        continue;
                    } 
                } else {
                    try {
                        g.setPiece(r, c, Colour::White);
                        black = true;
                        cout << g;
                    } catch ( ... ) {
                        continue;
                    }
                }
                if (g.isFull()) break;
            }
        }

        Colour c = g.whoWon();
        if (c == Colour::Black) {
            cout << "Black wins!" << endl;
        } else if (c == Colour::White) {
            cout << "White wins!" << endl;
        } else {
            cout << "Tie!" << endl;
        }
    }
    catch (ios::failure &) {} 
}

