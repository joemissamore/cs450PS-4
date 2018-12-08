#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <math.h>
#define DEBUG 0
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

struct VirtToPhys
{
    /* useBit | virtualAddress | -> | physicalAddress */
    int useBit;
    int virtualAddress;
    int physicalAddress;
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


unsigned long long CalculatePhysicalAddress(int ppn, int moveIndex, int hexVal, const VirtualPageInfo &virtualPageInfo)
{
    /* Calculates the index for looking up in the virtual page table */ 
    // int bitIndex = virtualPageInfo.numBitsPhysAdd - virtualPageInfo.numBitsVirtAdd + 1; // 2
    // int shift = virtualPageInfo.numBitsVirtAdd - bitIndex; // 5

    unsigned long long tagBits = virtualPageInfo.numBitsVirtAdd - moveIndex; // 3
    unsigned long long insertIntoPhysicalAddress = (ppn << moveIndex);
    /* insertIntoPhysicalAddress = 192 = 11000000
     * all we care about are the first 3 bits = 110
     * 8 bits */ 

    /* Zero's out bits 
     * zero's out the first 2 
     * bits in the entered hex 
     * value. */
    for (int n = moveIndex + 1; n < virtualPageInfo.numBitsVirtAdd; n++)
    {
        hexVal &= ~(1 << n);
    }
    /* hexVal = 10 = 0001010 
     * these first three bits
     * will be masked with the
     * first 3 bits of insertIntoPhysicalAddress */

    unsigned long long physicalAddress = (hexVal | insertIntoPhysicalAddress);
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

    /* Get virtual page information */
    VirtualPageInfo virtualPageInfo;
    file >> temp;
    virtualPageInfo.numBitsVirtAdd = temp;
    file >> temp;
    virtualPageInfo.numBitsPhysAdd = temp;
    file >> temp;
    virtualPageInfo.numByesInPage = temp;
    
    /* Get virtual page information */ 
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
    }
    file.close();

    /* The clock algo executes on this vector */
    vector<VirtToPhys> virtToPhysMap; 

    for (int i = 0; i < virtualPages.size(); i++)
    {
        if (virtualPages[i].valid && virtualPages[i].permission)
        {
            VirtToPhys vtp;
            vtp.useBit = virtualPages[i].useBit;
            vtp.physicalAddress = virtualPages[i].ppn;
            vtp.virtualAddress = i;

            virtToPhysMap.push_back(vtp);
        }
    }

    int moveIndex = log2(virtualPageInfo.numByesInPage);
    double moveIndexCheck = log2(virtualPageInfo.numByesInPage);

    /* Checking to make sure log2 returned an integer value */
    if (moveIndex != moveIndexCheck)
    {
        cout << "incorrect BYTES in a page" << endl;
        exit(1);
    }

    /* CLOCK ALGO position in 
     * the VirtToPhysMap 'table' */
    int clockAlgo = 0;

    while (true)
    {
        unsigned long long hexVal = 0;
        cout << ">>> ";
        cin >> hex >> hexVal;
                
        /* Gets the virtual index */
        unsigned long long indexVirt = (hexVal >> moveIndex);
        bool segfault = false;
        bool disk = false;

        if (virtualPages[indexVirt].valid == 0 && virtualPages[indexVirt].permission == 1)
        {
            #ifdef PROB1
            disk = true;
            #else
            disk = true;
            
            /* CLOCK ALGO */ 
            VirtToPhys * virtToPhys;
            for (; ; clockAlgo++)
            {
                if (clockAlgo > virtToPhysMap.size() - 1)
                {
                    debug << "setting clockAlgo to 0" << endl;
                    clockAlgo = 0;
                }   

                virtToPhys = &virtToPhysMap[clockAlgo];

                if (virtToPhysMap[clockAlgo].useBit == 0)
                {
                    debug << "executing clock algo on idx: " << clockAlgo << endl;

                    /* set original virtual address bits to: */
                    virtualPages[virtToPhys->virtualAddress].valid = 0;
                    virtualPages[virtToPhys->virtualAddress].useBit = 0;
                    virtualPages[virtToPhys->virtualAddress].ppn = -1;

                    /* Mark the page as used */
                    virtToPhys->useBit = 1;

                    /* Change virtual page number */
                    virtToPhys->virtualAddress = indexVirt;
                    /* Set virtual page table ppn */
                    virtualPages[indexVirt].ppn = virtToPhys->physicalAddress;
                    /* Validate the virtual page table entry */
                    virtualPages[indexVirt].valid = 1;
                    /* Just in case permissions was previously set to 0,
                    * now we have permission to access ppn. */
                    virtualPages[indexVirt].permission = 1;
                    virtualPages[indexVirt].useBit = 1;

                    break;
                }
                else
                {
                    virtToPhysMap[clockAlgo].useBit = 0;
                    virtualPages[virtToPhys->virtualAddress].useBit = 0;
                } 
                    
            }

            /* After the clock algo finishes, we must advance its position. 
            * If the clock algo is at the last position move its position
            * to the first element. */
            if (clockAlgo == virtToPhysMap.size() - 1)    
                clockAlgo = 0;
            else
                clockAlgo++;

            debug << "clock algo position (right after for loop): " << clockAlgo << endl;
            #endif
        }

        else if (virtualPages[indexVirt].permission == 0)
            segfault = true;

        if (segfault)
            cout << "Physical Address: SEGFAULT" << endl;
    #ifdef PROB1
        else if (disk)
            cout << "Physical Address: DISK" << endl;
    #else
        else if (disk)
        {
            printf("Physical Address (PAGE FAULT): 0x%X\n", CalculatePhysicalAddress(virtualPages[indexVirt].ppn, moveIndex, hexVal, virtualPageInfo));
        }
    #endif
        else
        {
            printf("Physical Address: 0x%X\n", CalculatePhysicalAddress(virtualPages[indexVirt].ppn, moveIndex, hexVal, virtualPageInfo));
            virtualPages[indexVirt].useBit = 1;
            for (int i = 0; i < virtToPhysMap.size(); i++)
            {
                if (virtToPhysMap[i].virtualAddress == indexVirt)
                    virtToPhysMap[i].useBit = 1;
            }
            
        }

        debug << endl << "Debugging Info: " << endl;
        PrintVector(virtualPages);
        debug << "clock algo position: " << clockAlgo << endl;
        debug << "indexVirt: " << indexVirt << endl;
    }

    return 0;
}