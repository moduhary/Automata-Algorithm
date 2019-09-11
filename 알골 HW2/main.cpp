#include <iostream>
#include <fstream>
#include <string>
#include "RBtree.h"
using namespace std;

int main() {
    RBtree Tree = RBtree();
    string op;
    int in, count = 0, ret;

    ifstream Inputs;
    Inputs.open("input.txt", ifstream::in);

    while(!Inputs.eof()) {
        Inputs >> op >> in;

        cout << op << " " << in << endl;
    }

    Inputs.close();

    Inputs.open("input.txt", ifstream::in);

    while(!Inputs.eof()) {
        Inputs >> op >> in;

        if(op == "I")
        {

            ret = Tree.Insert(in);

            if(ret == in)
                cout << "Value " << in << " Inserted." << endl;
            else
                cout << "Value " << in << " is already in." << endl;
        }
        else if(op == "D")
        {
            if(Tree.get_node_count() == 0)
                cout << "Deletion on empty tree is not available." << endl;
            else
                ret = Tree.Delete(in);

            if(ret == in)
                cout << "Value " << in << " Deleted." << endl;
            else
                cout << "Value " << in << " is not in tree." << endl;

        }
        else if(op == "S")
        {
            //TODO
        }
        else if(op == "R")
        {
            //TODO
        }
        else
            cout << "Operation " << op << " is not found." << endl;

        count++;
    }

    Inputs.close();

    return 0;
}