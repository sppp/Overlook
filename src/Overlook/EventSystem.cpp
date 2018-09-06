#if 0

#include "Overlook.h"


namespace Overlook {

EventSystem::EventSystem() :
	client([](const std::string& strLogMsg) { std::cout << strLogMsg << std::endl;  })
{
	
	
}

EventSystem::~EventSystem() {
	
}
	
void EventSystem::Data() {
	System& sys = GetSystem();
	
	bool mail_enable = Config::email_enable;
	String user = Config::email_user;
	String pass = Config::email_pass;
	String srv = Config::email_server;
	int port = Config::email_port;
	if (mail_enable && !email_init && !user.IsEmpty() && !pass.IsEmpty() && !srv.IsEmpty() && port) {
		
		InitMail();
		
		email_init = true;
	}
	
	if (email_init) {
		GetMailCount();
		
		for(int i = max(0, email_count - 10); i < email_count; i++) {
			
			int j = cached_mail.Find(i);
			if (j == -1) {
				String mail = GetMail(i);
				if (mail.IsEmpty()) {
					InitMail();
					i--;
					continue;
				}
				cached_mail.Add(i, mail);
				//LOG(mail);
				
				if (mail.Find("From: alerts@autochartist.com") != -1) {
					ProcessAutoChartist(mail);
				}
			}
		}
		
		DUMPM(level);
	}
	
	
	
	
}

void EventSystem::InitMail() {
	String user = Config::email_user;
	String pass = Config::email_pass;
	String srv = Config::email_server;
	int port = Config::email_port;
	String srv_str = srv + ":" + IntStr(port);
	client.InitSession(srv_str.Begin(), user.Begin(), pass.Begin(),
		CMailClient::SettingsFlag::ALL_FLAGS, CMailClient::SslTlsFlag::ENABLE_SSL);
}

int EventSystem::GetMailCount() {
	/*
	* FLAGS (\Answered \Flagged \Draft \Deleted \Seen $ATTACHMENT $Junk $MDNSent $NotJunk $NotPhishing $Phishing Junk NonJunk NotJunk receipt-handled)
	* OK [PERMANENTFLAGS ()] Flags permitted.
	* OK [UIDVALIDITY 631009610] UIDs valid.
	* 3387 EXISTS
	* 0 RECENT
	* OK [UIDNEXT 8539] Predicted next UID.
	* OK [HIGHESTMODSEQ 2865771]
	*/
	std::string info, path;
	path = "INBOX";
	client.InfoFolder(path, info);
	String s = info;
	if (s.IsEmpty()) {
		InitMail();
		client.InfoFolder(path, info);
		s = info;
	}
	int b = s.Find(" EXISTS");
	int a = s.ReverseFind(" ", b-1) + 1;
	s = s.Mid(a, b-a);
	email_count = StrInt(s);
	return email_count;
}

String EventSystem::GetMail(int i) {
	std::string strEmail;
	String str = IntStr(i + 1);
	/* retrieve the mail number 1 and store it in strEmail */
	bool bResRcvStr = client.GetString(str.Begin(), strEmail);
	
	return strEmail;
}

void EventSystem::ProcessAutoChartist(String s) {
	int a = s.Find("Date: ");
	int b = s.Find("\n", a);
	if (a == -1 || b == -1) return;
	a += 6;
	String datestr = s.Mid(a, b-a);
	
	Index<String> symbols;
	symbols.Add("EURUSD");
	symbols.Add("GBPUSD");
	symbols.Add("USDCHF");
	symbols.Add("USDJPY");
	symbols.Add("USDCAD");
	symbols.Add("AUDUSD");
	symbols.Add("NZDUSD");
	symbols.Add("EURCHF");
	symbols.Add("EURJPY");
	symbols.Add("EURGBP");
	
	int found = 0;
	for(int i = 0; i < symbols.GetCount(); i++) {
		String sym = symbols[i];
		String findsym = sym.Left(3) + "/" + sym.Right(3);
		
		int j = s.Find(findsym);
		if (j == -1) continue;
		
		int a = s.Find("Target Level: ", j);
		if (a == -1) continue;
		a += 14;
		int b = s.Find("<", a);
		
		String levstr = s.Mid(a, b-a);
		double level = StrDbl(levstr);
		
		if (!found)
			for(int i = 0; i < this->level.GetCount(); i++)
				this->level[i] = 0;
		found++;
		
		this->level.GetAdd(sym) = level;
	}
	
}

}
#endif
