#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <math.h>
#define DEBUG 1
#define debug if (DEBUG) cout

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

void PrintVector(vector<VirtualPage> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        debug << "valid: " << v[i].valid << endl;
        debug << "permission: " << v[i].permission << endl;
        debug << "ppn: " << v[i].ppn << endl;
        debug << "useBit: " << v[i].useBit << endl;
    }
}

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        cout << "Forgot file name" << endl;
        exit(1);
    }

    /* Reading in values from file */
    int temp = 0;

    vector <VirtualPage> virtualPages;
    ifstream file; 
    file.open(argv[1]);
    /* [0] = # of bits in virtual address
     * [1] = # of bits in physical address
     * [2] = # of BYTES in a page */

    /* Get virtual page information */
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
    
    /* Get virtual page information */ 
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
    
    
    /* Gets the virtual index
     * hexVal = 0x4A = 1001010 
     * moveIndex = 5 = 101 */
    int indexVirt = (hexVal >> moveIndex);
    /* indexVirt = 2 */


    // When pages are not in physical memory but have a permission bit set to 1, print DISK.
    /* Get PPN value */
    if (virtualPages[indexVirt].valid == 0 && virtualPages[indexVirt].permission == 1)
    {
        #ifdef PROB1
        cout << "DISK" << endl;
        #else
        /* CLOCK ALGO */ 
        for (int i = 0, numIts = 0; 1; i++, numIts++)
        {
            if (virtualPages[i].valid == 0
                && virtualPages[i].permission == 1
                && virtualPages[i].useBit == 0)
            {
                virtualPages[i].valid = 1;
                virtualPages[i].useBit = 1;
                cout << "PAGE FAULT"<< endl;
                // printf("0x%X\n", physicalAddress);
                // break;
            }
            else 
                virtualPages[i].useBit = 0;

            if (i == numOfVirtualPages - 1)
                i = 0;

            if (numIts > 1)
                exit(1);
        }
        #endif

    }
        
//     // else if (virtualPages[indexVirt].valid == 0)
//     //     cout << "SEGFAULT" << endl;
//     // else
//     //     printf("0x%X\n", physicalAddress);


//     int ppn = virtualPages[indexVirt].ppn;
//     /* ppn = 6 */

//     int insertIntoPhysicalAddress = (ppn << moveIndex);
//     /* insertIntoPhysicalAddress = 192 = 11000000
//      * all we care about are the first 3 bits = 110
//      * 8 bits */ 

//     /* Zero's out bits 
//      * zero's out the first 2 
//      * bits in the entered hex 
//      * value. */
//     for (int n = virtualPageInfo.numBitsVirtAdd - 1; n < virtualPageInfo.numBitsVirtAdd + bitIndex - 1; n++)
//     {
//         hexVal &= ~(1 << n);
//     }
//     /* hexVal = 10 = 0001010 
//      * these first three bits
//      * will be masked with the
//      * first 3 bits of insertIntoPhysicalAddress */

//     int physicalAddress = (hexVal | insertIntoPhysicalAddress);
//     /* physicalAddress = 202 = 11001010 = 0xCA
//      * 8 bits */
    
    


//     file.close();
//     return 0;
// }