#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <math.h>

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
    /* [0] = # of bits in virtual address
     * [1] = # of bits in physical address
     * [2] = # of BYTES in a page */
    for (int i = 0; i < 3; i ++)
    {
        file >> temp;
        addressInfo[i] = temp;
    }
    
    /* Valid?
     * Permission (1 bit)
     * Physical page number
     * Use bit (see Problem 2) */

    while (file >> temp)
        fileInfo.push_back(temp);

    int hexVal = 0;
    int bitIndex = addressInfo[1] - addressInfo[0] + 1; // 2
    int moveIndex = log2(addressInfo[2]); // 5
    double moveIndexCheck = log2(addressInfo[2]);
    if (moveIndex != moveIndexCheck)
    {
        cout << "incorrect BYTES in a page" << endl;
        exit(1);
    }
    // cout << "moveIndex: " << moveIndex << endl;
    cout << "Enter hex address: ";
    cin >> hex >> hexVal;
    
    int indexVirt = (hexVal >> moveIndex);
    int ppn = indexVirt * 4 + 2;
    int insertIntoPhysicalAddress = (fileInfo[ppn] << moveIndex);
    // cout << "insertIntoPhysicalAddress: " << bitset<32>(insertIntoPhysicalAddress) << endl;

    for (int n = addressInfo[0] - 1; n < addressInfo[0] + bitIndex - 1; n++)
    {
        hexVal &= ~(1 << n);
    }

    int physicalAddress = (hexVal | insertIntoPhysicalAddress);
    // cout << bitset<15>(physicalAddress) << endl;
    if (fileInfo[indexVirt * 4] == 0)
        cout << "SEGFAULT" << endl;
    // When pages are not in physical memory but have a permission bit set to 1, print DISK.
    // else if (fileInfo[indexVirt * 4 + 1] == 0 && pages are not in physical memory???)
    //     report an err
    else
        printf("0x%X\n", physicalAddress);




    file.close();
    return 0;
}