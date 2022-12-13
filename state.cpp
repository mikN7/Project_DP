//dp state

#include <cstdio>
#include <cctype>
#include <sstream>
#include <list>
#include <algorithm>

class CTelevisor;

class TTelevisor_State
{
private:
	CTelevisor * const m_pTv;
protected:
	CTelevisor * const getParent();
public:
	TTelevisor_State(CTelevisor * const pTv);
	virtual ~TTelevisor_State();

	virtual void putInput(char c) = 0;
}; //class TTelevisor_State

class CTelevisor
{
private:
	friend class TTelevisor_State;
	void setState(TTelevisor_State * pState);
	TTelevisor_State * m_pState;
	typedef std::list < TTelevisor_State *> StatesT;
	StatesT m_toDelete;
	void eraseOldStates();
	static void eraseState(TTelevisor_State * pState);
protected:
	//INFO: tu uproszczenie (friendy) po to by nie dodawaæ metod do zarz¹dzania poni¿szymi zmiennymi
	friend class CTVState_Idle;
	friend class CTVState_PowerOnOff;
	friend class CTVState_PickChannel;

	bool m_bIsTurnedOn;
	int m_iChannel;
public:
	CTelevisor();
	~CTelevisor();
	void recvSignal(char c);
	void powerOn();
	void powerOff();
}; //class CTelevisor

class CTVState_PowerOnOff
	: public TTelevisor_State
{
public:
	CTVState_PowerOnOff(CTelevisor * const pTv);
	virtual void putInput(char c);
}; //class CTVState_PowerOnOff

class CTVState_Idle
	: public TTelevisor_State
{
public:
	CTVState_Idle(CTelevisor * const pTv);
	virtual void putInput(char c);
}; //class CTVState_Idle

class CTVState_PickChannel
	: public TTelevisor_State
{
private:
	std::stringstream m_sNewChannel;
public:
	CTVState_PickChannel(CTelevisor * const pTv);
	virtual void putInput(char c);
}; //class CTVState_PickChannel

int main()
{
	CTelevisor tv;
	tv.powerOn();
	tv.recvSignal('1');
	tv.recvSignal('0');
	tv.powerOff();
	tv.powerOn();
	tv.recvSignal('0');
	tv.recvSignal('7');
	tv.powerOff();
	return 0;
}

CTelevisor * const TTelevisor_State::getParent()
{
	return m_pTv;
}

TTelevisor_State::TTelevisor_State(CTelevisor * const pTv)
	: m_pTv(pTv)
{
}

//virtual
TTelevisor_State::~TTelevisor_State()
{
}

void CTelevisor::setState(TTelevisor_State * pState)
{
	if (m_pState)
		m_toDelete.push_back(m_pState);

	m_pState = pState;
}

CTelevisor::CTelevisor()
	: m_pState(NULL)
	, m_bIsTurnedOn(false)
	, m_iChannel(1)
{
}

CTelevisor::~CTelevisor()
{
	eraseOldStates();
}

void CTelevisor::recvSignal(char c)
{
	if (m_pState)
		m_pState->putInput(c);

	eraseOldStates();
}

void CTelevisor::powerOn()
{
	printf("powerOn()\n");
	CTVState_PowerOnOff * pPower = new CTVState_PowerOnOff(this);
	setState(pPower);
	pPower->putInput('*');
	eraseOldStates();
}

void CTelevisor::powerOff()
{
	printf("powerOff()\n");
	CTVState_PowerOnOff * pPower = new CTVState_PowerOnOff(this);
	setState(pPower);
	pPower->putInput('*');
	eraseOldStates();
}

void CTelevisor::eraseOldStates()
{
	std::for_each(m_toDelete.begin(), m_toDelete.end(), eraseState);
	m_toDelete.clear();
}

//static
void CTelevisor::eraseState(TTelevisor_State * pState)
{
	delete pState;
}

CTVState_PowerOnOff::CTVState_PowerOnOff(CTelevisor * const pTv)
	: TTelevisor_State(pTv)
{
	printf("[constructed] CTVState_PowerOnOff()\n");
}

//virtual
void CTVState_PowerOnOff::putInput(char c)
{
	if (c != '*')
		return;

	getParent()->m_bIsTurnedOn = !getParent()->m_bIsTurnedOn;
	if (getParent()->m_bIsTurnedOn)
		getParent()->setState(new CTVState_Idle(getParent()));
	else
		getParent()->setState(NULL);

}

CTVState_Idle::CTVState_Idle(CTelevisor * const pTv)
	: TTelevisor_State(pTv)
{
	printf("[constructed] CTVState_Idle()\n");
	printf("\tTV is idling at channel %d\n", getParent()->m_iChannel);
}

//virtual
void CTVState_Idle::putInput(char c)
{
	if (!::isdigit(c))
		return;

	CTVState_PickChannel * pChannel = new CTVState_PickChannel(getParent());
	getParent()->setState(pChannel);
	pChannel->putInput(c);
}

CTVState_PickChannel::CTVState_PickChannel(CTelevisor * const pTv)
	: TTelevisor_State(pTv)
{
	printf("[constructed] CTVState_PickChannel()\n");
}

//virtual
void CTVState_PickChannel::putInput(char c)
{
	if (!::isdigit(c))
		return;

	m_sNewChannel << c;
	if (m_sNewChannel.str().length() != 2)
		return;

	m_sNewChannel >> getParent()->m_iChannel;
	getParent()->setState(new CTVState_Idle(getParent()));
}

