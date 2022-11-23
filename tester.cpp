//
// Created by David Du on 2/3/22.
//

#include <iostream>

#include "RadixTree.h"

using namespace std;

int main(){

    RadixTree<double> radix;
    radix.insert("romane", 1);
    radix.insert("romanus", 2);
    radix.insert("romulus", 3);
    radix.insert("rubens", 4);
    radix.insert("ruber", 5);
    radix.insert("rubicon", 6);
    radix.insert("rubicundus", 7);
    radix.remove("romane");*/

    string search = "romane";

    if(radix.search(search) != nullptr){
        cout << *radix.search(search);
    }
    else{
        cout << "null";
    }

    radix.update("rubicundus", 10);

    search = "ruber";

    if(radix.search(search) != nullptr){
        cout << *radix.search(search);
    }
    else{
        cout << "null";
    }

    radix.remove("ruber");

    if(radix.search(search) != nullptr){
        cout << *radix.search(search);
    }
    else{
        cout << "null";
    }

    search = "rubicon";

    if(radix.search(search) != nullptr){
        cout << *radix.search(search);
    }
    else{
        cout << "null";
    }

}
