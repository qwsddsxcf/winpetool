

#include "framework.h"
int daochubiaojiexiinfile(char* filedata, HWND dadjubing)
{
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;

	int dcmemadress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_EXPORT_DIRECTORY factdcaddress = (PIMAGE_EXPORT_DIRECTORY)(filedata + addressmemtofile(filedata, dcmemadress));

    

	TCHAR tmp[60] = _T("");

    // ���DLL����
    char* dllname = (filedata + addressmemtofile(filedata, factdcaddress->Name));
    _sntprintf_s(tmp, _countof(tmp), _T("===========================\n"));
    AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
    _sntprintf_s(tmp, _countof(tmp), _T("DLL����: %hs\n"), dllname);
    AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);

    // ���������������Ϣ
    _sntprintf_s(tmp, _countof(tmp), _T("��������: %d\n"), factdcaddress->Base);
    AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
    _sntprintf_s(tmp, _countof(tmp), _T("������������: %d\n"), factdcaddress->NumberOfFunctions);
    AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
    _sntprintf_s(tmp, _countof(tmp), _T("������������: %d\n"), factdcaddress->NumberOfNames);
    AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);

    DWORD* functionAddressTable = (DWORD*)(filedata + addressmemtofile(filedata, factdcaddress->AddressOfFunctions));
    DWORD* functionNameTable = (DWORD*)(filedata + addressmemtofile(filedata, factdcaddress->AddressOfNames));
    WORD* functionOrdinalTable = (WORD*)(filedata + addressmemtofile(filedata, factdcaddress->AddressOfNameOrdinals));

    for (DWORD i = 0; i < factdcaddress->NumberOfFunctions; i++) {
        DWORD functionRVA = functionAddressTable[i];
        if (functionRVA == 0) {
            continue; // ��������
        }
        else {
            // �����Ƿ��к�����
            BOOL hasName = FALSE;
            char* functionName = NULL;

            for (DWORD j = 0; j < factdcaddress->NumberOfNames; j++) {
                if (functionOrdinalTable[j] == i) {
                    functionName = (char*)(filedata + addressmemtofile(filedata, functionNameTable[j]));
                    hasName = TRUE;
                    break;
                }
            }

            if (hasName) {
                _sntprintf_s(tmp, _countof(tmp), _T("������: %hs, ��ַ: 0x%08X, ���: %d \n"),
                    functionName, functionRVA, factdcaddress->Base + i);
            }
            else {
                _sntprintf_s(tmp, _countof(tmp), _T("��ַ: 0x%08X, ���: %d \n"),
                    functionRVA, factdcaddress->Base + i);
            }
            AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
        }
        }
        MessageBox(NULL, TEXT("�ɹ�����������"), TEXT("�ɹ�"), NULL);
	return 1;
}
