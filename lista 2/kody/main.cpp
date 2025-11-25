/*
tasks.json: w zadaniu "C/C++: g++ build active file" jest flaga -std=c++23.
tasks.json: "isDefault": true
launch.json: W polu "preLaunchTask" jest poprawne odwołanie do etykiety zadania kompilacji: "C/C++: g++ build active file".
c_cpp_properties.json "cppStandard": "c++23",
*/

#include "testy.h"
#include "algo.h"

int main()
{
    run_tests(); 
    return 0;  
}
