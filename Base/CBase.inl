
inline CBase::CBase() : m_dwRefCnt(0)
{

}
inline CBase::~CBase()
{

}

inline unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

inline unsigned long CBase::Release()
{
	if (m_dwRefCnt == 0)
	{
		Free();
		 
		delete this;

		return 0;
	}

	return m_dwRefCnt--;
}
