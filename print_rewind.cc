#include <shapelib/shapefil.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

/************************************************************************/
/*                     only support polygon                             */
/*                                                                      */
/************************************************************************/
int main(int argc, char *argv[]) {
    cout << endl;
    cout << "----------------------------------" << endl;
    cout << "|           print rewind          |" << endl;
    cout << "----------------------------------" << endl;
    cout << "- arg1 : shape file path" << endl << endl;
    cout << "* path : " << string(argv[1]) << endl << endl;

    // --------------------------------------------------------------------
    //      Open the .shp file.
    // --------------------------------------------------------------------
    SHPHandle hSHP = SHPOpen(argv[1], "rb");
    if (hSHP == NULL) {
        cout << "Unable to open: " << string(argv[1]) << endl;
        return EXIT_FAILURE;
    }

    // --------------------------------------------------------------------
    //      Get shape info
    // --------------------------------------------------------------------
    int nShapeType, nEntities;
    double adfMinBound[4], adfMaxBound[4];
    SHPGetInfo(hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound);

    int nInvalidCount = 0;

    cout.precision(15);
    cout << fixed;
    for (int iRecord = 0; iRecord < nEntities; iRecord++) {
        SHPObject *psShape = SHPReadObject(hSHP, iRecord);

        vector<double> x, y, z;
        for (int iVertex = 0; iVertex < psShape->nVertices; iVertex++) {
            x.push_back(psShape->padfX[iVertex]);
            y.push_back(psShape->padfY[iVertex]);
            z.push_back(psShape->padfZ[iVertex]);
        }

        int nAltered = SHPRewindObject(hSHP, psShape);
        if (nAltered > 0) {
            cout << endl << nAltered << " rings wound in the wrong direction." << endl;

            cout << "bef: (";
            for (size_t i = 0; i < x.size(); i++)
                cout << x[i] << ' ' << y[i] << ' ' << z[i] << ',';
            cout << ")" << endl;

            x.clear();
            y.clear();
            z.clear();
            for (int iVertex = 0; iVertex < psShape->nVertices; iVertex++) {
                x.push_back(psShape->padfX[iVertex]);
                y.push_back(psShape->padfY[iVertex]);
                z.push_back(psShape->padfZ[iVertex]);
            }

            cout << "aft: (";
            for (size_t i = 0; i < x.size(); i++)
                cout << x[i] << ' ' << y[i] << ' ' << z[i] << ',';
            cout << ")" << endl;

            nInvalidCount++;
        }

        SHPDestroyObject(psShape);
    }

    cout << endl << nInvalidCount << " objects rewound." << endl;

    cout << endl << "GoodBye!" << endl << endl;
    SHPClose(hSHP);

    return EXIT_SUCCESS;
}