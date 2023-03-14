#include <shapelib/shapefil.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Shapefile (dbf) field name are at most 10chars + 1 NULL.
constexpr int MAXFIELDNAMELEN = 12;

using namespace std;

int main(int argc, char *argv[]) {
    cout << endl;
    cout << "----------------------------------" << endl;
    cout << "|       modify information       |" << endl;
    cout << "----------------------------------" << endl;
    cout << "- arg1 : shape file path" << endl;
    cout << "- arg2 : result file path" << endl << endl;
    cout << "* path1 : " << string(argv[1]) << endl;
    cout << "* path2 : " << string(argv[2]) << endl << endl;

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
    //      Create the .dbf file.
    // --------------------------------------------------------------------
    DBFHandle hDBF_res = DBFCloneEmpty(hDBF, argv[2]);
    if (hDBF_res == NULL) {
        cout << "DBFCreate (" << string(argv[2]) << ") failes." << endl;
        return EXIT_FAILURE;
    }

    // --------------------------------------------------------------------
    //      Find field length you want to change
    // --------------------------------------------------------------------
    vector<string> fields;
    vector<int> lengths;
    for (int iField = 0; iField < DBFGetFieldCount(hDBF); iField++) {
        /************************************************************************/
        /*                      enum  DBFFieldType                              */
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

        string field;
        for (auto c : szTitle) {
            if (isspace(c) || (int)c == 0)
                continue;
            field += std::tolower(c);
        }
        fields.push_back(field);
        lengths.push_back(nWidth);
    }

    auto itr = find_if(fields.begin(), fields.end(),
                       [](string field) { return field.compare("histtype") == 0; });
    int mod_index =
        accumulate(lengths.begin(), lengths.begin() + distance(fields.begin(), itr), 0) + 1;

    // --------------------------------------------------------------------
    //      Modify & Save to .dbf file.
    // --------------------------------------------------------------------
    for (int iRecord = 0; iRecord < DBFGetRecordCount(hDBF); iRecord++) {
        string DBFRow = string(DBFReadTuple(hDBF, iRecord));
        DBFRow.replace(mod_index, 3, "mod");
        DBFWriteTuple(hDBF_res, iRecord, &DBFRow[0]);
    }

    cout << "Modify Finish!" << endl;

    cout << endl << "GoodBye!" << endl << endl;
    DBFClose(hDBF);
    DBFClose(hDBF_res);

    return EXIT_SUCCESS;
}