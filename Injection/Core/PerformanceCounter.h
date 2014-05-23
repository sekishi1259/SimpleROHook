class CPerformanceCounter
{
private:
	LONGLONG m_OldCounter;
	LARGE_INTEGER m_Counter;
	double m_dFreq;

	LONGLONG m_OldInstantCounter;


	double *m_dticklist;
	int     m_ticklist_index;
	int     m_ticklist_size;

	int		m_FrameCounter;
	int		m_FrameCount;
	int		m_SampleTerm;
	int		m_FrameList[1024];
	int		m_MonitorRefreshRate;
	double  m_FrameRate;

public:
	CPerformanceCounter(int step) :  m_OldCounter(0),m_ticklist_index(0),m_FrameCounter(0),m_FrameCount(0),m_SampleTerm(1000),m_MonitorRefreshRate(60),m_FrameRate(0.0f)
	{
		if( QueryPerformanceCounter( &m_Counter ) != 0){
			m_OldCounter = m_Counter.QuadPart;
			LARGE_INTEGER Freq;
			QueryPerformanceFrequency( &Freq );
			m_dFreq = (double)Freq.QuadPart;
		}else{
			m_OldCounter = (LONGLONG)::timeGetTime();
		}
		m_ticklist_size = step;
		m_dticklist = new double[m_ticklist_size];
		for(int ii = 0;ii < m_ticklist_size;ii++)
			m_dticklist[ ii ] = 0.0;
	}
	virtual ~CPerformanceCounter()
	{
		delete[] m_dticklist;
	}
	void InitInstaltPerformance(void)
	{
		if( QueryPerformanceCounter( &m_Counter ) != 0){
			m_OldInstantCounter = m_Counter.QuadPart;
		}else{
			m_OldInstantCounter = (LONGLONG)::timeGetTime();;
		}
	}
	void SetMonitorRefreshRate(int value)
	{
		m_MonitorRefreshRate = value;
	}
	int GetMonitorRefreshRate(void)
	{
		return m_MonitorRefreshRate;
	}

	int GetFrameRate(void)
	{
		return (int)(m_FrameRate + 0.9);
	}
	void ModifiFrameRate(void)
	{
		int Tick = timeGetTime();
		for(int index = m_FrameCount - 1; index >= 0; index --){
			if(m_FrameList[index] + m_SampleTerm <= Tick){
				m_FrameCount = index;
			}else{
				break;
			}
		}
		memmove(m_FrameList + 1, m_FrameList, sizeof(m_FrameList[0]) * m_FrameCount);
		m_FrameList[0] = Tick;
		m_FrameCount ++;
		if(m_FrameCount > sizeof(m_FrameList) / sizeof(m_FrameList[0]) - 10){
			m_FrameCount = sizeof(m_FrameList) / sizeof(m_FrameList[0]) - 10;
			m_FrameRate = -1.0;
			return;
		}else{
			int Time = Tick - m_FrameList[m_FrameCount - 1];
			if(Time > 0 && m_FrameCount >= 2){
				m_FrameRate = (double)(m_FrameCount - 1) * (double)m_SampleTerm / (double)Time;

				if(m_FrameRate > (double)m_MonitorRefreshRate)
					m_FrameRate = (double)m_MonitorRefreshRate;
			}else{
				m_FrameRate = -1.0;
			}
		}
	}
	double CalcInstaltPerformance(void)
	{
		double tick;
		if( QueryPerformanceCounter( &m_Counter ) != 0){
			LONGLONG m_temptick = m_Counter.QuadPart;
			tick = ((double)(m_temptick - m_OldInstantCounter))*1000 / m_dFreq;
			m_OldInstantCounter = m_temptick;
		}else{
			LONGLONG m_temptick = (LONGLONG)::timeGetTime();
			tick = (double)(m_temptick - m_OldCounter);
			m_OldInstantCounter = m_temptick;
		}
		return tick;
	}

	void SetCounter(double tick)
	{
		m_dticklist[ m_ticklist_index ] = tick;
		m_ticklist_index = (m_ticklist_index+1) % m_ticklist_size;
	}
	void ModifiCounter(void)
	{
		if( QueryPerformanceCounter( &m_Counter ) != 0){
			LONGLONG m_temptick = m_Counter.QuadPart;
			double tick = ((double)(m_temptick - m_OldCounter))*1000 / m_dFreq;
			m_OldCounter = m_temptick;
			SetCounter(tick);
		}else{
			LONGLONG m_temptick = (LONGLONG)::timeGetTime();
			double tick = (double)(m_temptick - m_OldCounter);
			m_OldCounter = m_temptick;
			SetCounter(tick);
		}
	}
	double GetTotalTick(void)
	{
		double tick = 0.0;
		for(int ii = 0;ii < m_ticklist_size;ii++)
			tick += m_dticklist[ ii ];
		tick /= (double)m_ticklist_size;
		return tick;
	}
};
