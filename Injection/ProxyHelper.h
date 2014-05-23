#pragma once

#define PROXY0(name)	{ DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::" #name "()"));	return m_Instance->name();							}
#define PROXY1(name)	{ DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::" #name "()"));	return m_Instance->name(p1);							}
#define PROXY2(name)	{ DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::" #name "()"));	return m_Instance->name(p1, p2);						}
#define PROXY3(name)	{ DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::" #name "()"));	return m_Instance->name(p1, p2, p3);					}
#define PROXY4(name)	{ DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::" #name "()"));	return m_Instance->name(p1, p2, p3, p4);				}
#define PROXY5(name)	{ DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::" #name "()"));	return m_Instance->name(p1, p2, p3, p4, p5);			}
#define PROXY6(name)	{ DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::" #name "()"));	return m_Instance->name(p1, p2, p3, p4, p5 ,p6);		}
#define PROXY7(name)	{ DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::" #name "()"));	return m_Instance->name(p1, p2, p3, p4, p5 ,p6 ,p7);	}

#define PROXY_RELEASE													\
	{																	\
		ULONG Count = m_Instance->Release();								\
		DEBUG_LOGGING_MORE_DETAIL((CLASSNAME "::Release()  RefCount = %d", Count));	\
		if(Count == 0)													\
			delete this;												\
		return Count;													\
	}

