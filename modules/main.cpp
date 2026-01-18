import engine;

// Simple infinte loop until cin.eof() is detected, which will
// trigger process_input() to return false.
int main() {
    GlobalState global;
    while (global.process_input())
        ;
}