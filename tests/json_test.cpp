#include "rolechat/background/JsonBackgroundData.h"

void test_basic_json_read() {
    rolechat::background::JsonBackgroundData jsonData;
    jsonData.loadBackground("background.json");
}

int main() {
    test_basic_json_read();
    return 0;
}
