#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <math.h>

using namespace std;

struct VirtualPage
{
    int valid;
    int permission;
    int ppn;
    int useBit;
};

struct VirtualPageInfo
{
    int numBitsVirtAdd;
    int numBitsPhysAdd;
    int numByesInPage;
};

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
    vector <VirtualPage> virtualPages;
    ifstream file; 
    file.open(argv[1]);
    /* [0] = # of bits in virtual address
     * [1] = # of bits in physical address
     * [2] = # of BYTES in a page */
    VirtualPageInfo virtualPageInfo;
    for (int i = 0; i < 3; i ++)
    {
        file >> temp;
        if (i % 3 == 0)
            virtualPageInfo.numBitsVirtAdd = temp;
        else if (i % 3 == 1)
            virtualPageInfo.numBitsPhysAdd = temp;
        else
            virtualPageInfo.numByesInPage = temp;
    }
    
    /* Valid?
     * Permission (1 bit)
     * Physical page number
     * Use bit (see Problem 2) */

    int numOfVirtualPages = 0; // 4
    while (file >> temp)
    {
        VirtualPage virtualPage;
        virtualPage.valid = temp;
        file >> temp;
        virtualPage.permission = temp;
        file >> temp;
        virtualPage.ppn = temp;
        file >> temp;
        virtualPage.useBit = temp;

        virtualPages.push_back(virtualPage);

        numOfVirtualPages++;
    }


    int hexVal = 0;
    int bitIndex = virtualPageInfo.numBitsPhysAdd - virtualPageInfo.numBitsVirtAdd + 1; // 2
    int moveIndex = log2(virtualPageInfo.numByesInPage); // 5
    double moveIndexCheck = log2(virtualPageInfo.numByesInPage);

    /* Checking to make sure log2 returned an integer value */
    if (moveIndex != moveIndexCheck)
    {
        cout << "incorrect BYTES in a page" << endl;
        exit(1);
    }

    cout << "Enter hex address: ";
    cin >> hex >> hexVal;
    
    /* hexVal = 0x4A = 1001010 
     * moveIndex = 5 = 101 */
    int indexVirt = (hexVal >> moveIndex);
    /* indexVirt = 2 */
    int ppn = virtualPages[indexVirt].ppn;
    /* ppn = 6 */

    int insertIntoPhysicalAddress = (ppn << moveIndex);
    /* insertIntoPhysicalAddress = 192 = 11000000 
     * 8 bits */ 

    /* Zero's out bits 
     * zero's out the first 2 
     * bits in the entered hex 
     * value. */
    for (int n = virtualPageInfo.numBitsVirtAdd - 1; n < virtualPageInfo.numBitsVirtAdd + bitIndex - 1; n++)
    {
        hexVal &= ~(1 << n);
    }
    /* hexVal = 10 = 0001010 */

    int physicalAddress = (hexVal | insertIntoPhysicalAddress);
    /* physicalAddress = 202 = 11001010 = 0xCA
     * 8 bits */
    
    // When pages are not in physical memory but have a permission bit set to 1, print DISK.
    if (virtualPages[indexVirt].valid == 0 && virtualPages[indexVirt].permission == 1)
    {
        #ifdef PROB1
        cout << "DISK" << endl;
        #else
        int i = 0;
        // int usedBit = ((indexVirt + i) % bitIndex) * bitIndex + 3;
        // while(fileInfo[usedBit] == 1)
        // {
        //     fileInfo[usedBit] = 0;
        //     cout << "executed" << endl;
        //     cout << fileInfo[usedBit - 3] << endl;
        //     cout << fileInfo[usedBit - 2] << endl;
        //     cout << fileInfo[usedBit - 1] << endl;
        //     cout << fileInfo[usedBit] << endl;
        //     i++;
        // }
        #endif

    }

        
    else if (virtualPages[indexVirt].valid == 0)
        cout << "SEGFAULT" << endl;
    else
        printf("0x%X\n", physicalAddress);


    file.close();
    return 0;
}