#include <shapelib/shapefil.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#define USE_CASE_A

using namespace std;

int main(int argc, char *argv[]) {
    cout << endl;
    cout << "----------------------------------" << endl;
    cout << "|          copy geometry          |" << endl;
    cout << "----------------------------------" << endl;
    cout << "- arg1 : shape file path" << endl << endl;
    cout << "- arg2 : result file path" << endl << endl;
    cout << "* path1 : " << string(argv[1]) << endl;
    cout << "* path2 : " << string(argv[2]) << endl << endl;

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

    // --------------------------------------------------------------------
    //      Create the .shp file.
    // --------------------------------------------------------------------
    SHPHandle hSHP_res = SHPCreate(argv[2], nShapeType);
    if (hSHP_res == NULL) {
        cout << "Unable to create: " << string(argv[2]) << endl;
        return EXIT_FAILURE;
    }

    for (int iRecord = 0; iRecord < nEntities; iRecord++) {
        SHPObject *psShape = SHPReadObject(hSHP, iRecord);
        if (psShape == NULL) {
            cerr << "Unable to read shape " << iRecord << ", terminating object reading." << endl;
            break;
        }
#ifdef USE_CASE_A
        /************************************************************************/
        /*                              1안                                     */
        /************************************************************************/
        if (SHPWriteObject(hSHP_res, -1, psShape) < 0) {
            cerr << "error writing shapefile!  iRecord: " << iRecord << endl;
            break;
        }
#else
#ifdef USE_CASE_B
        /************************************************************************/
        /*                              2안                                     */
        /************************************************************************/
        SHPObject *psObject = SHPCreateObject(
            nShapeType, -1, psShape->nParts, psShape->panPartStart, psShape->panPartType,
            psShape->nVertices, psShape->padfX, psShape->padfY, psShape->padfZ, psShape->padfM);
        SHPWriteObject(hSHP_res, -1, psObject);
        SHPDestroyObject(psObject);
#else
        /************************************************************************/
        /*                              3안                                     */
        /************************************************************************/
        SHPObject *psObject = SHPCreateSimpleObject(nShapeType, psShape->nVertices, psShape->padfX,
                                                    psShape->padfY, psShape->padfZ);
        SHPWriteObject(hSHP_res, -1, psObject);
        SHPDestroyObject(psObject);
#endif
#endif
        SHPDestroyObject(psShape);
    }
    cout << "Copy Finish!" << endl;

    cout << endl << "GoodBye!" << endl << endl;
    SHPClose(hSHP);
    SHPClose(hSHP_res);

    return EXIT_SUCCESS;
}