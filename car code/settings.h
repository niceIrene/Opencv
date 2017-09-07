#include "init.h"

OVERLAPPED Wol;
int inslen = 6;

class ComPort
{
public:
	HANDLE hCom;
	DWORD dwError;
	BOOL bComOpened;
	OVERLAPPED wrOverlapped;
	ComPort(LPCSTR portName)
	{
		hCom=CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		if (hCom==INVALID_HANDLE_VALUE) {
			dwError=GetLastError();
			cout <<portName <<" Opened failed. Error code: " <<dwError <<endl;
		}
		else {
			bComOpened=TRUE;
			cout <<portName <<" Opened successful." <<endl;
		}
		if (!SetupComm(hCom, 2048, 2048)) {
			cout <<"SetupComm fail! Close Comm!" <<endl;
			CloseHandle(hCom);
		}
		else cout <<"SetupComm OK!" <<endl;
		COMMTIMEOUTS TimeOuts;
		memset(&TimeOuts, 0, sizeof(TimeOuts));
		TimeOuts.ReadIntervalTimeout=1000;
		TimeOuts.ReadTotalTimeoutMultiplier=500;
		TimeOuts.ReadTotalTimeoutConstant=5000;
		TimeOuts.WriteTotalTimeoutMultiplier=500;
		TimeOuts.WriteTotalTimeoutConstant=2000;
		SetCommTimeouts(hCom,&TimeOuts);
		DCB dcb;
		if (!GetCommState(hCom, &dcb)) {
			cout <<"GetCommState fail! Comm close" <<endl;
			CloseHandle(hCom);
		}
		else cout <<"GetCommState OK!" <<endl;
		dcb.DCBlength=sizeof(dcb);
		if (!BuildCommDCB("9600, n, 8, 1", &dcb)) {
			cout <<"BuileCOmmDCB fail, Comm close!" <<endl;
			CloseHandle(hCom);
		}
		if (SetCommState(hCom, &dcb)) cout <<"SetCommState OK!" <<endl;
		ZeroMemory(&wrOverlapped, sizeof(wrOverlapped));
		if (wrOverlapped.hEvent!=NULL) {
			ResetEvent(wrOverlapped.hEvent);
			wrOverlapped.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
		}
		PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
		txIndex=rxIndex=0;
	}
	virtual ~ComPort(void)
	{
		if (bComOpened) CloseHandle(hCom);
	}
	unsigned char rxIndex;
	unsigned char lpInBuffer[1024];
	int Sync(void)
	{
		int i=1000;
		while (rxIndex!=txIndex && txIndex!=0 && (--i)) {
			int size=Read();
			for (int i=0; i<size; i++)
				if (((lpInBuffer[i] & 0x80)==0) && (lpInBuffer[i]!=0)) rxIndex=lpInBuffer[i];
		}
		if (i<1) return -1;
		return rxIndex;
	}
	int Read(void)
	{
		DWORD dwBytesRead=1024;
		COMSTAT ComStat;
		DWORD dwErrorFlags;
		OVERLAPPED m_osRead;
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		m_osRead.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
		ClearCommError(hCom, &dwErrorFlags, &ComStat);
		dwBytesRead=min(dwBytesRead, (DWORD)ComStat.cbInQue);
		if (!dwBytesRead) return FALSE;
		BOOL bReadStatus;
		bReadStatus=ReadFile(hCom, lpInBuffer, dwBytesRead, &dwBytesRead, &m_osRead);
		if (!bReadStatus) {
			if (GetLastError()==ERROR_IO_PENDING) {
				WaitForSingleObject(m_osRead.hEvent, 2000);
				PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				return dwBytesRead;
			}
			return 0;
		}
		PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		return dwBytesRead;
	}
	unsigned char txIndex;
	int Send(unsigned char command)
	{
		++txIndex;
		txIndex &=~0x80;
		DWORD dwError;
		DWORD dwWantSend=1;
		DWORD dwRealSend=0;
		char pSendBuffer[]={char(command)};
		if (ClearCommError(hCom, &dwError, NULL)) {
			PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);
			cout <<"PurgeComm OK!" <<endl;
		}
		if (!WriteFile(hCom, pSendBuffer, dwWantSend, &dwRealSend, &wrOverlapped)) {
			dwError=GetLastError();
			if (dwError==ERROR_IO_PENDING) {
				while (!GetOverlappedResult(hCom, &wrOverlapped, &dwRealSend, FALSE)) {
					if (GetLastError()==ERROR_IO_INCOMPLETE) {
						cout <<"Sending..." <<endl;
						continue;
					}
					else {
						cout <<"Send failed. Error code: " <<dwError <<endl;
						ClearCommError(hCom, &dwError, NULL);
						break;
					}
				}
			}
		}
		return dwRealSend;
	}
};

ComPort port("\\\\.\\COM5");
void sendMessage1(char c)
{
	unsigned char *temp;
	temp=new unsigned char[inslen];
	for (int i=0; i<inslen; i++) temp[i]=c;
	Wol.Internal=0;
	Wol.InternalHigh=0;
	Wol.Offset=0;
	Wol.OffsetHigh=0;
	Wol.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
	long unsigned int len=inslen;
	WriteFile(port.hCom, temp, len, NULL, &Wol);
	Wol.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
	len=inslen;
	WriteFile(port.hCom, temp, len, NULL, &Wol);
}

void GoAhead()
{
	sendMessage1('1');
	cout <<"GoAhead" <<endl;
}
void Park()
{
	sendMessage1('0');
	cout <<"Park" <<endl;
}
void TurnLeft()
{
	sendMessage1('4');
	cout <<"TurnLeft" <<endl;
}
void TurnRight()
{
	sendMessage1('3');
	cout <<"TurnRight" <<endl;
}
void SuperLeft()
{
	sendMessage1('M');
	cout <<"SuperLeft" <<endl;
}
void SuperRight()
{
   sendMessage1('Q');
    cout <<"SuperRight" <<endl;
}

bool defaultDCB(int rate_arg)
{
	DCB dcb;
	int rate=rate_arg;
	memset(&dcb, 0, sizeof(dcb));
	if (!GetCommState(hComm, &dcb)) return false;
	dcb.DCBlength=sizeof(dcb);
	dcb.BaudRate=rate;
	dcb.Parity=NOPARITY;
	dcb.fParity=dcb.fOutxCtsFlow=dcb.fOutxDsrFlow=dcb.fDsrSensitivity=dcb.fOutX=dcb.fInX=dcb.fErrorChar=dcb.fNull=dcb.fAbortOnError=dcb.wReserved=0;
	dcb.StopBits=ONESTOPBIT;
	dcb.ByteSize=8;
	dcb.fDtrControl=DTR_CONTROL_DISABLE;
	dcb.fRtsControl=RTS_CONTROL_DISABLE;
	dcb.fBinary=1;
	dcb.XonLim=2;
	dcb.XoffLim=4;
	dcb.XonChar=0x13;
	dcb.XoffChar=0x19;
	dcb.EvtChar=0;
	if (!SetCommState(hComm, &dcb)) return false;
	else return true;
}

bool timeOut(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD ReadTotalconstant, DWORD WriteTotalMultiplier, DWORD WriteTotalconstant)
{
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout=ReadInterval;
	timeouts.ReadTotalTimeoutConstant=ReadTotalconstant;
	timeouts.ReadTotalTimeoutMultiplier=ReadTotalMultiplier;
	timeouts.WriteTotalTimeoutConstant=WriteTotalconstant;
	timeouts.WriteTotalTimeoutMultiplier=WriteTotalMultiplier;
	if (!SetCommTimeouts(hComm, &timeouts)) return false;
	else return true;
}



void instruction()
{
	if (length(carPos_head,sortcorners[0])<ARRIVE||length(carPos_head,sortcorners[2])<ARRIVE) {
		GoAhead();
		cout <<"go ahead";
		cvWaitKey(25);
	}
	/*else if (Angle>15 && Angle<30) {
		TurnLeft();
		cout <<"L";
		cvWaitKey(25);
	}*/
	else if (length(carPos_head,sortcorners[1])<ARRIVE||length(carPos_head,sortcorners[2])<ARRIVE) {
		TurnRight();
		cout <<"turn right";
		cvWaitKey(25);
	}
	else if (length(carPos_head,sortcorners[3])<ARRIVE) {
        Park();
        cout <<"stop";
        cvWaitKey(25);
	}/*
	else if (Angle>30) {
        SuperLeft();
        cout <<"M";
        cvWaitKey(25);
	}*/
}

