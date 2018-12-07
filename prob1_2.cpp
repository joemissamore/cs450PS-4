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
        debug << "------------------" << endl;
        debug << "pos #: " << i << endl;
        debug << "-------" << endl;
        debug << "valid: " << v[i].valid << endl;
        debug << "permission: " << v[i].permission << endl;
        debug << "ppn: " << v[i].ppn << endl;
        debug << "useBit: " << v[i].useBit << endl;
        debug << "------------------" << endl;
    }
}


int CalculatePhysicalAddress(int ppn, int moveIndex, int hexVal, const VirtualPageInfo &virtualPageInfo)
{
    /* Calculates the index for looking up in the virtual page table */ 
    // int bitIndex = virtualPageInfo.numBitsPhysAdd - virtualPageInfo.numBitsVirtAdd + 1; // 2
    // int shift = virtualPageInfo.numBitsVirtAdd - bitIndex; // 5

    int tagBits = virtualPageInfo.numBitsVirtAdd - moveIndex; // 3
    int insertIntoPhysicalAddress = (ppn << moveIndex);
    /* insertIntoPhysicalAddress = 192 = 11000000
     * all we care about are the first 3 bits = 110
     * 8 bits */ 

    /* Zero's out bits 
     * zero's out the first 2 
     * bits in the entered hex 
     * value. */
    for (int n = moveIndex + 1; n < virtualPageInfo.numBitsVirtAdd - 1; n++)
    {
        hexVal &= ~(1 << n);
    }
    /* hexVal = 10 = 0001010 
     * these first three bits
     * will be masked with the
     * first 3 bits of insertIntoPhysicalAddress */

    int physicalAddress = (hexVal | insertIntoPhysicalAddress);
    /* physicalAddress = 202 = 11001010 = 0xCA
     * 8 bits */
    return physicalAddress;
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
    file >> temp;
    virtualPageInfo.numBitsVirtAdd = temp;
    file >> temp;
    virtualPageInfo.numBitsPhysAdd = temp;
    file >> temp;
    virtualPageInfo.numByesInPage = temp;
    
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
    file.close();


    debug << "numOfVirtualPages: " << numOfVirtualPages << endl;

    int clockAlgo = 0;
    int moveIndex = log2(virtualPageInfo.numByesInPage); // 5
    double moveIndexCheck = log2(virtualPageInfo.numByesInPage);
    /* ppn = 6 */

    /* Checking to make sure log2 returned an integer value */
    if (moveIndex != moveIndexCheck)
    {
        cout << "incorrect BYTES in a page" << endl;
        exit(1);
    }

    while (true)
    {
        int hexVal = 0;
        cout << ">>> ";
        cin >> hex >> hexVal;
        
        
        /* Gets the virtual index
        * hexVal = 0x4A = 1001010 
        * moveIndex = 5 = 101 */
        int indexVirt = (hexVal >> moveIndex);
        /* indexVirt = 2 */
        int ppn = virtualPages[indexVirt].ppn;

        bool segfault = false;
        bool disk = false;


        // When pages are not in physical memory but have a permission bit set to 1, print DISK.
        /* Get PPN value */
        if (virtualPages[indexVirt].valid == 0 && virtualPages[indexVirt].permission == 1)
        {
            #ifdef PROB1
            disk = true;
            #else
            disk = true;
            /* CLOCK ALGO */ 
            // debug << "executing clock ago..." << endl;
            // debug << "executing clock algo on idx: " << clockAlgo << endl;
            for (; ; clockAlgo++)
            {
                if (clockAlgo > numOfVirtualPages)
                {
                    debug << "setting clockAlgo to 0" << endl;
                    clockAlgo = 0;
                }   

                if (virtualPages[clockAlgo].permission == 1
                    && virtualPages[clockAlgo].useBit == 0)
                {
                    debug << "executing clock algo on idx: " << clockAlgo << endl;
                    /* Map to DISK */
                    virtualPages[clockAlgo].valid = 0;
                    virtualPages[clockAlgo].useBit = 0;

                    /* Set virtual page table index */
                    virtualPages[indexVirt].ppn = virtualPages[clockAlgo].ppn;
                    /* Validate the virtual page table entry */
                    virtualPages[indexVirt].valid = 1;
                    /* Just in case permissions was previously set to 0,
                    * now we have permission to access ppn. */
                    virtualPages[indexVirt].permission = 1;
                    virtualPages[indexVirt].useBit = 1;
                    break;
                }
                else 
                    virtualPages[clockAlgo].useBit = 0;
            }
            debug << "clock algo position (right after for loop): " << clockAlgo << endl;

            /* After the clock algo finishes, we must advance its position. 
            * If the clock algo is at the last position move its position
            * to the first element. */
            if (clockAlgo == numOfVirtualPages - 1)    
                clockAlgo = 0;
            else
                clockAlgo++;
            #endif

        }
        else if (virtualPages[indexVirt].permission == 0)
            segfault = true;

        if (segfault)
            cout << "SEGFAULT" << endl;
    #ifdef PROB1
        else if (disk)
        {
            cout << "DISK" << endl;
        }
    #else
        else if (disk)
        {
            cout << "PAGE FAULT" << endl;
            printf("0x%X\n", CalculatePhysicalAddress(ppn, moveIndex, hexVal, virtualPageInfo));
        }
    #endif
        else
        {
            printf("0x%X\n", CalculatePhysicalAddress(ppn, moveIndex, hexVal, virtualPageInfo));
            virtualPages[indexVirt].useBit = 1;
        }

        debug << endl << "Debugging Info: " << endl;
        PrintVector(virtualPages);
        debug << "clock algo position: " << clockAlgo << endl;
        debug << "indexVirt: " << indexVirt << endl;
    }

    return 0;
}