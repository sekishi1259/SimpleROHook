class CSearchCode
{
private:
	typedef struct StFindMemInfo{
		unsigned char x;
		char          flag;
	}StFindMemInfo;


	std::vector<StFindMemInfo> m_FindInfo;
	std::map<char,int> m_MakerIndex;

	char ahex2i(char code)
	{
		if( code >= '0' && code <= '9' ){
			return (code - '0');
		}else
		if( code >= 'a' && code <= 'z' ){
			return (code - 'a' + 0x0a );
		}else
		if( code >= 'A' && code <= 'Z' ){
			return (code - 'A' + 0x0a );
		}
		return -1;
	}

public:
	CSearchCode(char *pattern)
	{
		while( *pattern != 0 )
		{
			StFindMemInfo tempInfo;
			char h1,h2;
			h1 = *pattern++;
			h2 = *pattern++;
			if(!h2)break;

			if( h1 == '*' ){
				// wildcard
				tempInfo.x    = 0x00;
				tempInfo.flag = 0;
				m_FindInfo.push_back( tempInfo );
				if( h2 != '*' )
					m_MakerIndex[ h2 ] = m_FindInfo.size() - 1;
			}else{
				tempInfo.x = (ahex2i(h1)<<4) | ahex2i(h2);
				tempInfo.flag = 1;
				m_FindInfo.push_back( tempInfo );
			}
		}
	}
	~CSearchCode(){}

	int GetMakerIndex(char code)
	{
		return m_MakerIndex[ code ];
	}
	BOOL PatternMatcher(LPBYTE address)
	{
		int nums = m_FindInfo.size();
		for(int ii = 0;ii < nums;ii ++)
		{
			if( m_FindInfo[ii].flag && ( m_FindInfo[ii].x != address[ii] ) )
				return FALSE;
		}
		return TRUE;
	}
	DWORD GetImmediateDWORD(LPBYTE address,char code)
	{
		return *(DWORD*)(address + GetMakerIndex( code ));
	}
	DWORD Get4BIndexDWORD(LPBYTE address,char code)
	{
		LPBYTE targetaddress = address + GetMakerIndex( code ); 
		return (*(DWORD*)targetaddress) + (DWORD)(targetaddress + 4);
	}
	DWORD GetNearJmpAddress(LPBYTE address,char code)
	{
		DWORD im_calladdress = *(DWORD*)(address + GetMakerIndex( code ));
		// convert to immediate address 
		im_calladdress += (DWORD)address + GetMakerIndex( code ) + 4;

		return im_calladdress;
	}

	BOOL NearJmpAddressMatcher(LPBYTE address,char code,DWORD calladdress)
	{
		DWORD im_calladdress = *(DWORD*)(address + GetMakerIndex( code ));
		// convert to immediate address 
		im_calladdress += (DWORD)address + GetMakerIndex( code ) + 4;

		if( im_calladdress == calladdress )
			return TRUE;

		return FALSE;
	}

	int GetSize(){ return m_FindInfo.size(); }
};
