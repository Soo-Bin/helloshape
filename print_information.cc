#include <shapelib/shapefil.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// Shapefile (dbf) field name are at most 10chars + 1 NULL.
constexpr int MAXFIELDNAMELEN = 12;

using namespace std;

int main(int argc, char *argv[]) {
    cout << endl;
    cout << "----------------------------------" << endl;
    cout << "|        print information       |" << endl;
    cout << "----------------------------------" << endl;
    cout << "- arg1 : shape file path" << endl << endl;
    cout << "* path : " << string(argv[1]) << endl << endl;

    // --------------------------------------------------------------------
    //      Open the .dbf file.
    // --------------------------------------------------------------------
    DBFHandle hDBF = DBFOpen(argv[1], "rb");
    if (hDBF == NULL) {
        cout << "DBFOpen (" << string(argv[1]) << ") failes." << endl;
        return EXIT_FAILURE;
    }

    // --------------------------------------------------------------------
    //      If there is no data in this file let the user know.
    //      이 파일에 데이터가 없으면 사용자에게 알립니다.
    // --------------------------------------------------------------------
    if (DBFGetFieldCount(hDBF) == 0) {
        cout << "There are no fields in this table!" << endl;
        return EXIT_FAILURE;
    }
    cout << "* field count : " << DBFGetFieldCount(hDBF) << endl;
    cout << "* total record: " << DBFGetRecordCount(hDBF) << endl << endl;

    // --------------------------------------------------------------------
    //      header definitions.
    // --------------------------------------------------------------------
    for (int iField = 0; iField < DBFGetFieldCount(hDBF); iField++) {
        /************************************************************************/
        /*                      enum  DBFFieldType()                            */
        /*                                                                      */
        /*      FTString, FTInteger, FTDouble, FTLogical, FTDate, FTInvalid     */
        /************************************************************************/
        DBFFieldType eType;
        vector<char> szTitle(MAXFIELDNAMELEN, 0);
        string pszTypeName;
        char chNativeType;
        int nWidth, nDecimals;

        /************************************************************************/
        /*                       DBFGetNativeFieldType()                        */
        /*                                                                      */
        /*      Return the DBase field type for the specified field.            */
        /*                                                                      */
        /*      Value can be one of: 'C' (String), 'D' (Date), 'F' (Float),     */
        /*                           'N' (Numeric, with or without decimal),    */
        /*                           'L' (Logical),                             */
        /*                           'M' (Memo: 10 digits .DBT block ptr)       */
        /************************************************************************/
        chNativeType = DBFGetNativeFieldType(hDBF, iField);

        eType = DBFGetFieldInfo(hDBF, iField, &szTitle[0], &nWidth, &nDecimals);
        if (eType == FTString)
            pszTypeName = "String";
        else if (eType == FTInteger)
            pszTypeName = "Integer";
        else if (eType == FTDouble)
            pszTypeName = "Double";
        else if (eType == FTInvalid)
            pszTypeName = "Invalid";

        cout << "Field " << iField << ": Type=" << chNativeType << '/' << pszTypeName
             << ", Title=" << &szTitle[0] << ", Width=" << nWidth << ", Decimals=" << nDecimals
             << endl;
    }

    string checking;
    cout << endl << "View all records? (y/n)" << endl;
    cin >> checking;

    // --------------------------------------------------------------------
    //      Read all the records.
    // --------------------------------------------------------------------
    if (checking == "y") {
        for (int iRecord = 0; iRecord < DBFGetRecordCount(hDBF); iRecord++) {
            cout << iRecord << ". ";

            for (int iField = 0; iField < DBFGetFieldCount(hDBF); iField++) {
                if (DBFIsAttributeNULL(hDBF, iRecord, iField))
                    cout << "(NULL)\t";
                else
                    cout << DBFReadStringAttribute(hDBF, iRecord, iField) << '\t';
            }
            cout << endl;

            if (DBFIsRecordDeleted(hDBF, iRecord))
                cout << "(DELETED)" << endl;
        }
    }

    cout << endl << "GoodBye!" << endl << endl;
    DBFClose(hDBF);

    return EXIT_SUCCESS;
}