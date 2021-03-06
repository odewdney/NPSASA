// TestNSP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib,"Ws2_32.lib")

const char *radiuspacket = "\x01\x03\x01\x38\xcc\x77\x7d\x57\x83\xe1\x10\xfd\xf1\x9d\xa9\xfe" \
	"\x38\x77\x0b\xee\x01\x06\x75\x73\x65\x72\x05\x06\x00\x59\xf0\x00" \
	"\x1e\x0e\x31\x37\x32\x2e\x31\x36\x2e\x30\x2e\x32\x35\x34\x1f\x11" \
	"\x31\x32\x33\x2e\x31\x32\x33\x2e\x31\x32\x33\x2e\x31\x32\x33\x3d" \
	"\x06\x00\x00\x00\x05\x42\x11\x31\x32\x33\x2e\x31\x32\x33\x2e\x31" \
	"\x32\x33\x2e\x31\x32\x33\x1a\x18\x00\x00\x01\x37\x0b\x12\xe0\x20" \
	"\x6f\x55\xd2\x90\x21\x66\x28\x5f\x22\xcc\x85\xa4\x0e\x48\x1a\x3a" \
	"\x00\x00\x01\x37\x19\x34\x00\x00\xb6\xb7\x98\x72\x88\x90\xee\x92" \
	"\xe9\x0c\x9d\x21\x53\xad\x10\x0a\x10\x20\x00\x00\x00\x00\x00\x00" \
	"\x73\xde\xb3\x55\x0f\xa7\x41\xbf\xeb\xf2\xa0\xdf\x45\x12\x18\xa6" \
	"\xe2\x1d\x69\x67\xd8\xe2\xcc\x91\x04\x06\xac\x10\x01\xfe\x1a\x31" \
	"\x00\x00\x00\x09\x01\x2b\x61\x75\x64\x69\x74\x2d\x73\x65\x73\x73" \
	"\x69\x6f\x6e\x2d\x69\x64\x3d\x61\x63\x31\x30\x30\x31\x66\x65\x30" \
	"\x30\x35\x39\x66\x30\x30\x30\x35\x62\x61\x33\x35\x64\x63\x36\x1a" \
	"\x24\x00\x00\x00\x09\x01\x1e\x69\x70\x3a\x73\x6f\x75\x72\x63\x65" \
	"\x2d\x69\x70\x3d\x31\x32\x33\x2e\x31\x32\x33\x2e\x31\x32\x33\x2e" \
	"\x31\x32\x33\x1a\x14\x00\x00\x0c\x04\x92\x0e\x54\x75\x6e\x6e\x65" \
	"\x6c\x5f\x5f\x54\x65\x73\x74\x1a\x0c\x00\x00\x0c\x04\x96\x06\x00" \
	"\x00\x00\x03\x1a\x15\x00\x00\x00\x09\x01\x0f\x63\x6f\x61\x2d\x70" \
	"\x75\x73\x68\x3d\x74\x72\x75\x65";


struct RadiusTypes
{
	RADIUS_ATTRIBUTE_TYPE attr;
	RADIUS_DATA_TYPE type;
};

RadiusTypes radiusTypes[] = {
	{ ratUserName, rdtString},
	{ratUserPassword, rdtString },
	{ ratCHAPPassword, rdtString },
	{ ratNASIPAddress, rdtAddress },
	{ratNASPort, rdtInteger },
	{ratServiceType, rdtInteger },
	{ratFramedProtocol, rdtInteger },
	{ratFramedIPAddress, rdtAddress },
	{ratFramedIPNetmask, rdtAddress },
	{ratFramedRouting, rdtInteger },
	{ratFilterId, rdtString },
	{ratFramedMTU, rdtInteger },
	{ratFramedCompression, rdtInteger },
	{ratLoginIPHost, rdtAddress },
	{ratLoginService, rdtInteger },
	{ratLoginPort, rdtInteger },
	{ratReplyMessage, rdtString },
	{ratCallbackNumber, rdtString },
	{ratCallbackId, rdtString },
	{	ratFramedRoute, rdtString },
	{	ratFramedIPXNetwork, rdtInteger },
	{	ratState, rdtString },
	{	ratClass, rdtString },
	{	ratVendorSpecific, rdtString },
	{	ratSessionTimeout, rdtInteger },
	{	ratIdleTimeout, rdtInteger },
	{	ratTerminationAction, rdtInteger },
	{	ratCalledStationId, rdtString },
	{	ratCallingStationId, rdtString },
	{	ratNASIdentifier, rdtString },
	{	ratProxyState, rdtString },
	{	ratLoginLATService, rdtString },
	{	ratLoginLATNode, rdtString },
	{	ratLoginLATGroup, rdtString },
	{	ratFramedAppleTalkLink, rdtInteger },
	{	ratFramedAppleTalkNetwork, rdtInteger },
	{	ratFramedAppleTalkZone, rdtString },
	{	ratAcctStatusType, rdtString },
	{	ratAcctDelayTime, rdtString },
	{	ratAcctInputOctets, rdtString },
	{	ratAcctOutputOctets, rdtString },
	{	ratAcctSessionId, rdtString },
	{	ratAcctAuthentic, rdtString },
	{	ratAcctSessionTime, rdtString },
	{	ratAcctInputPackets, rdtString },
	{	ratAcctOutputPackets, rdtString },
	{	ratAcctTerminationCause, rdtString },
	{	ratCHAPChallenge, rdtString },
	{	ratNASPortType, rdtInteger },
	{	ratPortLimit, rdtInteger },
	{	ratTunnelType, rdtString },
	{	ratMediumType, rdtString },
	{(RADIUS_ATTRIBUTE_TYPE)66, rdtString},
	{	ratTunnelPassword, rdtString },
	{	ratTunnelPrivateGroupID, rdtString },
	{	ratNASIPv6Address, rdtString },
	{	ratFramedInterfaceId, rdtString },
	{	ratFramedIPv6Prefix, rdtString },
	{	ratLoginIPv6Host, rdtString },
	{	ratFramedIPv6Route, rdtString },
	{	ratFramedIPv6Pool, rdtString },

};

#pragma pack(push,1)
struct RadiusAttr
{
	char type;
	unsigned char len;
	char data[1];
};

struct RadiusPacket
{
	char code;
	char pktid;
	__int16 len;
	char auth[16];
	RadiusAttr attrs[1];
};
#pragma pack(pop)


struct MyAttrArray
{
	RADIUS_ATTRIBUTE_ARRAY ary;
	std::vector<RADIUS_ATTRIBUTE> attrs;
};

struct MyControlBlock
{
	RADIUS_EXTENSION_CONTROL_BLOCK ecb;
	MyAttrArray request;
};


PRADIUS_ATTRIBUTE_ARRAY WINAPI MyGetRequest(IN struct _RADIUS_EXTENSION_CONTROL_BLOCK *This)
{
	MyControlBlock *pBlk = (MyControlBlock*)This;
	return &pBlk->request.ary;
}

PRADIUS_ATTRIBUTE_ARRAY WINAPI MyGetResponse(IN struct _RADIUS_EXTENSION_CONTROL_BLOCK *This, IN RADIUS_CODE rcResponseType)
{
	return NULL;
}

DWORD WINAPI MySetResponseType(IN struct _RADIUS_EXTENSION_CONTROL_BLOCK *This, IN RADIUS_CODE rcResponseType)
{
	return NULL;
}

void CloneAttr(RADIUS_ATTRIBUTE &dst, IN const RADIUS_ATTRIBUTE *pAttr)
{
	dst = *pAttr;
	switch (dst.fDataType)
	{
	case rdtIpv6Address:
	case rdtUnknown:
	case rdtString:
		BYTE * pValue;
		dst.lpValue = pValue = new BYTE[dst.cbDataLength+1];
		memcpy(pValue, pAttr->lpValue, dst.cbDataLength);
		pValue[dst.cbDataLength] = 0;
		break;
	}
}

DWORD WINAPI MyAddAttr(IN struct _RADIUS_ATTRIBUTE_ARRAY *This, IN const RADIUS_ATTRIBUTE *pAttr)
{
	MyAttrArray *pAry = (MyAttrArray*)This;

	RADIUS_ATTRIBUTE clone;
	CloneAttr(clone, pAttr);

	pAry->attrs.push_back(clone);
	return 0;
}

const RADIUS_ATTRIBUTE *WINAPI MyAttributeAt(IN const struct _RADIUS_ATTRIBUTE_ARRAY *This, IN DWORD dwIndex)
{
	MyAttrArray *pAry = (MyAttrArray*)This;
	if (dwIndex >= pAry->attrs.size())
		return NULL;
	return &pAry->attrs[dwIndex];
}

DWORD WINAPI MyGetSizeAttr(IN const struct _RADIUS_ATTRIBUTE_ARRAY *This)
{
	MyAttrArray *pAry = (MyAttrArray*)This;
	return (DWORD)pAry->attrs.size();
}

DWORD WINAPI MyAttrSetAt(IN struct _RADIUS_ATTRIBUTE_ARRAY *This, IN DWORD dwIndex, IN const RADIUS_ATTRIBUTE *pAttr)
{
	MyAttrArray *pAry = (MyAttrArray*)This;
	if (dwIndex >= pAry->attrs.size())
		return NULL;

	RADIUS_ATTRIBUTE clone;
	CloneAttr(clone, pAttr);

	pAry->attrs[dwIndex] = clone;

	return 0;
}


RADIUS_DATA_TYPE GetTypeForType(RADIUS_ATTRIBUTE_TYPE t)
{
	for (int n = 0; n < _countof(radiusTypes); n++)
	{
		if (radiusTypes[n].attr == t)
			return radiusTypes[n].type;
	}
	return rdtUnknown;
}

int main()
{
	RadiusPacket *pkt = (RadiusPacket*)radiuspacket;

	printf("Pkt type %d\n", pkt->code);

	int len = ntohs(pkt->len);

	printf("Pkt len %d\n", len);

	std::vector<RADIUS_ATTRIBUTE> attrs;

	RadiusAttr *pAttrs = pkt->attrs;
	int remain = len - sizeof(RadiusPacket);
	while (remain > 0)
	{
		RADIUS_ATTRIBUTE rattr;

		rattr.dwAttrType = pAttrs->type;
		rattr.fDataType = GetTypeForType((RADIUS_ATTRIBUTE_TYPE)pAttrs->type);
		rattr.cbDataLength = pAttrs->len - 2;

		switch (rattr.fDataType)
		{
		case rdtAddress:
			rattr.dwValue = *(DWORD*)pAttrs->data;
			break;
		case rdtInteger:
		case rdtTime:
			rattr.dwValue = ntohl( *(DWORD*)pAttrs->data);
			break;
		case rdtUnknown:
		case rdtString:
		case rdtIpv6Address:
			rattr.lpValue = (BYTE*)pAttrs->data;
			break;
		default:
			printf("eik! unknown type");
			exit(100);
		}
		
		attrs.push_back(rattr);

		remain -= pAttrs->len;

		pAttrs = (RadiusAttr*)(((char*)pAttrs) + pAttrs->len);
	}

	printf("Found %d attrs", (int)attrs.size());


	MyControlBlock blk;
	blk.request.attrs = attrs;
	blk.request.ary.cbSize = sizeof(RADIUS_ATTRIBUTE_ARRAY);
	blk.request.ary.Add = MyAddAttr;
	blk.request.ary.GetSize = MyGetSizeAttr;
	blk.request.ary.AttributeAt = MyAttributeAt;
	blk.request.ary.SetAt = MyAttrSetAt;

	RADIUS_EXTENSION_CONTROL_BLOCK &ecb = blk.ecb;

	ecb.cbSize = sizeof(RADIUS_EXTENSION_CONTROL_BLOCK);
	ecb.dwVersion = 1;
	ecb.repPoint = repAuthentication;
	ecb.rcRequestType = rcAccessRequest;

	ecb.rcResponseType = rcUnknown;

	ecb.GetRequest = MyGetRequest;
	ecb.GetResponse = MyGetResponse;
	ecb.SetResponseType = MySetResponseType;


	HMODULE hModule = LoadLibrary(_T("npsasa.dll"));
	if (hModule == NULL)
	{
		printf("Failed to load lib %08x\n", GetLastError());
		return 1;
	}

	PRADIUS_EXTENSION_PROCESS_2 proc = (PRADIUS_EXTENSION_PROCESS_2)GetProcAddress(hModule, RADIUS_EXTENSION_PROCESS2);

	if (proc == NULL)
	{
		printf("Failed to get proc %08x\n", GetLastError());
		return 1;
	}

	DWORD ret = proc(&ecb);

	if (ret != ERROR_SUCCESS)
	{
		printf("Failed proc %08x\n", ret);
		return 1;
	}

    return 0;
}

