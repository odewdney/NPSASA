// NPSASA.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#pragma comment(lib,"Ws2_32.lib")


EXTERN_C DWORD WINAPI RadiusExtensionProcess2(IN OUT PRADIUS_EXTENSION_CONTROL_BLOCK pECB)
{
#pragma comment(linker, "/EXPORT:" RADIUS_EXTENSION_PROCESS2 "=" __FUNCDNAME__)

	 if (!pECB || pECB->cbSize != sizeof(RADIUS_EXTENSION_CONTROL_BLOCK) || pECB->dwVersion != RADIUS_EXTENSION_VERSION)
	 {
		 return ERROR_INVALID_PARAMETER;
	 }

	 if (pECB->repPoint != repAuthentication || pECB->rcRequestType != rcAccessRequest)
	 {
		 return NO_ERROR;
	 }

	 PRADIUS_ATTRIBUTE_ARRAY request = pECB->GetRequest(pECB);

	 if (!request)
		 return ERROR_INVALID_PARAMETER;

	 DWORD numAttr = request->GetSize(request);

	 if (numAttr == ERROR_INVALID_PARAMETER) // odd, but as per MSDN
		 return ERROR_INVALID_PARAMETER;

	 char *szTunnelName = NULL;
	 DWORD calledStationIndex = -1;
	 char *szCalledStationId = NULL;

	 for (DWORD attributeIndex = 0; attributeIndex < numAttr; attributeIndex++)
	 {
		 const RADIUS_ATTRIBUTE *pAttr = request->AttributeAt(request, attributeIndex);
		 if (!pAttr) continue;
		 if (pAttr->dwAttrType == ratVendorSpecific)
		 {
			 RADIUS_VSA_FORMAT *pVsa = (RADIUS_VSA_FORMAT *)pAttr->lpValue;
			 DWORD vendor = ntohl(*(DWORD*)pVsa->VendorId);
			 if (vendor == 3076 && pVsa->VendorType == 146)
			 { // ASA-TunnelGroupName
				 char *szTunnel = (char*)pVsa->AttributeSpecific;
				 if (pVsa->VendorLength < 3 || pVsa->VendorLength != pAttr->cbDataLength - 4) continue;
				 szTunnelName = (char*)alloca(pVsa->VendorLength - 1);
				 memcpy(szTunnelName, pVsa->AttributeSpecific, pVsa->VendorLength - 2);
				 szTunnelName[pVsa->VendorLength - 2] = 0;
			 }
		 }
		 else if (pAttr->dwAttrType == ratCalledStationId)
		 {
			 calledStationIndex = attributeIndex;
			 szCalledStationId = (char*)alloca(pAttr->cbDataLength + 1);
			 memcpy(szCalledStationId, pAttr->lpValue, pAttr->cbDataLength);
			 szCalledStationId[pAttr->cbDataLength] = 0;
		 }
	 }

	 if (szTunnelName != NULL)
	 {
		 size_t len = ((szCalledStationId == NULL) ? 0 : strlen(szCalledStationId)) + strlen(szTunnelName) + 2;
		 char *szNewCalledStationId = (char*)alloca(len);
		 strcpy_s(szNewCalledStationId, len, szCalledStationId == NULL ? "" : szCalledStationId);
		 strcat_s(szNewCalledStationId, len, "/");
		 strcat_s(szNewCalledStationId, len, szTunnelName);

		 RADIUS_ATTRIBUTE newAttr;
		 newAttr.dwAttrType = ratCalledStationId;
		 newAttr.fDataType = rdtString;
		 newAttr.cbDataLength = (DWORD)(len - 1);
		 newAttr.lpValue = (BYTE*)szNewCalledStationId;

		 if (calledStationIndex == -1)
		 {
			 request->Add(request, &newAttr);
		 }
		 else
		 {
			 request->SetAt(request, calledStationIndex, &newAttr);
		 }
	 }
	 return NO_ERROR;
}

