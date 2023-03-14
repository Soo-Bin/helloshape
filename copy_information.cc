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
    cout << "|         copy information        |" << endl;
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

#ifdef USE_CASE_A
    /************************************************************************/
    /*                              1안                                     */
    /************************************************************************/

    // --------------------------------------------------------------------
    //      Create the .dbf file.
    // --------------------------------------------------------------------
    DBFHandle hDBF_res = DBFCreate(argv[2]);
    if (hDBF_res == NULL) {
        cout << "DBFCreate (" << string(argv[2]) << ") failes." << endl;
        return EXIT_FAILURE;
    }

    for (int iRecord = 0; iRecord < DBFGetRecordCount(hDBF); iRecord++) {
        for (int iField = 0; iField < DBFGetFieldCount(hDBF); iField++) {
            /************************************************************************/
            /*                      enum  DBFFieldType                              */
            /*                                                                      */
            /*      FTString, FTInteger, FTDouble, FTLogical, FTDate, FTInvalid     */
            /************************************************************************/
            DBFFieldType eType;
            vector<char> szTitle(MAXFIELDNAMELEN, 0);
            int nWidth, nDecimals;

            eType = DBFGetFieldInfo(hDBF, iField, &szTitle[0], &nWidth, &nDecimals);

            if (iRecord == 0) {
                if (DBFAddField(hDBF_res, &szTitle[0], eType, nWidth, nDecimals) == -1) {
                    cout << "DBFAddField(" << &szTitle[0] << ")," << eType << "," << nWidth << ","
                         << nDecimals << ") failed." << endl;
                    break;
                }
            }

            if (DBFIsAttributeNULL(hDBF, iRecord, iField))
                DBFWriteNULLAttribute(hDBF_res, iRecord, iField);
            else if (eType == FTString)
                DBFWriteStringAttribute(hDBF_res, iRecord, iField,
                                        DBFReadStringAttribute(hDBF, iRecord, iField));
            else if (eType == FTInteger)
                DBFWriteIntegerAttribute(hDBF_res, iRecord, iField,
                                         DBFReadIntegerAttribute(hDBF, iRecord, iField));
            else if (eType == FTDouble)
                DBFWriteDoubleAttribute(hDBF_res, iRecord, iField,
                                        DBFReadDoubleAttribute(hDBF, iRecord, iField));

            // FTLogical Read는 const char *로 반환하는데
            // Write는 char를 입력하도록 되어있음. 왜?
            // else if (eType == FTLogical)
            //     DBFWriteLogicalAttribute(hDBF_res, iRecord, iField,
            //                              DBFReadLogicalAttribute(hDBF, iRecord, iField));

            // 아래 Type은 관련 함수 없음
            // else if(eType == FTDate)
            // else if(eType == FTInvalid)
        }
    }
#else
    /************************************************************************/
    /*                              2안                                     */
    /************************************************************************/

    // --------------------------------------------------------------------
    //      Create the .dbf file.
    // --------------------------------------------------------------------
    DBFHandle hDBF_res = DBFCloneEmpty(hDBF, argv[2]);
    if (hDBF_res == NULL) {
        cout << "DBFCreate (" << string(argv[2]) << ") failes." << endl;
        return EXIT_FAILURE;
    }

    char *DBFRow = (char *)malloc(hDBF->nRecordLength + 15);

    // --------------------------------------------------------------------
    //      Save to .dbf file.
    // --------------------------------------------------------------------
    for (int iRecord = 0; iRecord < DBFGetRecordCount(hDBF); iRecord++) {
        memcpy(DBFRow, DBFReadTuple(hDBF, iRecord), hDBF->nRecordLength);
        DBFWriteTuple(hDBF_res, iRecord, DBFRow);
    }

#endif
    cout << "Copy Finish!" << endl;

    cout << endl << "GoodBye!" << endl << endl;
    DBFClose(hDBF);
    DBFClose(hDBF_res);

    return EXIT_SUCCESS;
}
