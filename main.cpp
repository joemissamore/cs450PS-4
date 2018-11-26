#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        cout << "Forgot file name" << endl;
        exit(1);
    }

    int temp = 0;
    int addressInfo[3] = {0};
    vector <int> fileInfo;
    ifstream file; 
    file.open(argv[1]);
    for (int i = 0; i < 3; i ++)
    {
        file >> temp;
        addressInfo[i] = temp;
    }
    
    while (file >> temp)
        fileInfo.push_back(temp);

    /* valid, permission, physical page number (ppn), use bit */

    file.close();
    return 0;
}