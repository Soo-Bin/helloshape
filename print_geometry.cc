#include <shapelib/shapefil.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    cout << endl;
    cout << "----------------------------------" << endl;
    cout << "|          print geometry         |" << endl;
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
    //      Print out the file bounds.
    // --------------------------------------------------------------------
    int nShapeType, nEntities;
    double adfMinBound[4], adfMaxBound[4];
    SHPGetInfo(hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound);

    cout << "* Shapefile Type: " << SHPTypeName(nShapeType) << "  #  of Shapes: " << nEntities
         << endl;
    cout << endl << "* File Bounds: " << endl;
    cout.precision(15);
    cout << fixed;
    cout << "x: " << adfMinBound[0] << endl;
    cout << "y: " << adfMinBound[1] << endl;
    cout << "z: " << adfMinBound[2] << endl;
    cout << "m: " << adfMinBound[3] << endl;
    cout << "*          to: " << endl;
    cout << "x: " << adfMaxBound[0] << endl;
    cout << "y: " << adfMaxBound[1] << endl;
    cout << "z: " << adfMaxBound[2] << endl;
    cout << "m: " << adfMaxBound[3] << endl;
    cout << endl;

    string checking;
    cout << endl << "View all records? (y/n)" << endl;
    cin >> checking;

    // --------------------------------------------------------------------
    //      Skim over the list of shapes, printing all the vertices.
    //      도형 목록을 훑어보고 모든 정점을 인쇄합니다.
    // --------------------------------------------------------------------
    if (checking == "y") {
        for (int iRecord = 0; iRecord < nEntities; iRecord++) {
            SHPObject *psShape = SHPReadObject(hSHP, iRecord);
            if (psShape == NULL) {
                cerr << "Unable to read shape " << iRecord << ", terminating object reading."
                     << endl;
                break;
            }

            // x,y,z,m에서 m 값이 있는지 확인하고 싶으면
            // if (psShape->bMeasureIsUsed)
            // 로 체크하면 됨
            cout << "Shape:" << iRecord << "(" << SHPTypeName(psShape->nSHPType)
                 << ")   nVertices=" << psShape->nVertices << ", nParts=" << psShape->nParts
                 << endl;

            if (psShape->nParts > 0 && psShape->panPartStart[0] != 0)
                cerr << "panPartStart[0] = " << psShape->panPartStart[0]
                     << ", not zero as expected." << endl;

            for (int iVertex = 0, iPart = 1; iVertex < psShape->nVertices; iVertex++) {
                const char *pszPartType = "";
                char pszPlus;
                if (iVertex == 0 && psShape->nParts > 0)
                    pszPartType = SHPPartTypeName(psShape->panPartType[0]);

                if (iPart < psShape->nParts && psShape->panPartStart[iPart] == iVertex) {
                    pszPartType = SHPPartTypeName(psShape->panPartType[iPart]);
                    iPart++;
                    pszPlus = '+';
                } else
                    pszPlus = ' ';

                cout << pszPlus << "(" << psShape->padfX[iVertex] << ',' << psShape->padfY[iVertex]
                     << ',' << psShape->padfZ[iVertex] << ") " << pszPartType << endl;
            }
            SHPDestroyObject(psShape);
        }
    }

    cout << endl << "GoodBye!" << endl << endl;
    SHPClose(hSHP);

    return EXIT_SUCCESS;
}