enum PACKET_HEADER{
	HEADER_ZC_SAY_DIALOG = 0xb4,
	HEADER_ZC_MENU_LIST = 0xb7,
};


#pragma pack(push,1)

#pragma warning(push)
#pragma warning(disable : 4200)

struct PACKET_CZ_SAY_DIALOG {
	WORD  PacketType;
	WORD  PacketLength;
	DWORD AID;
	BYTE  Data[];
};

struct PACKET_CZ_MENU_LIST {
	WORD  PacketType;
	WORD  PacketLength;
	DWORD AID;
	BYTE  Data[];
};

#pragma warning(pop)

#pragma pack(pop)
