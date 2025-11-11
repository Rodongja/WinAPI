#pragma once

struct tEvent
{
	EVENT_TYPE	eEven;
	DWORD_PTR	lParam;	// DWORD_PTR은 운영체제가 32bit이냐 64bit이냐에 따라 타입이 달라짐
	DWORD_PTR	wParam;
};

class CEventMgr
{
	SINGLE(CEventMgr)

private:
	vector<tEvent>		m_vecEvent;

	vector<CObject*>	m_vecDead; // 다음 DT때 삭제 될 오브제트


public:
	void update();

private:
	void Excute(const tEvent& _eve);

public:
	void AddEvent(const tEvent& _eve){m_vecEvent.push_back(_eve);}
};

