#ifndef _Overlook_SimBroker_h_
#define _Overlook_SimBroker_h_

#include <plugin/libmt/libmt.h>

namespace Overlook {
using namespace libmt;

class SimBroker : public Brokerage, Moveable<SimBroker> {
	Vector<double> symbol_profits, prev_symbol_profits, symbol_profit_diffs;
	String currency;
	Time cycle_time;
	double close_sum;
	double profit_sum, loss_sum;
	double collect_limit, collected;
	double prev_equity;
	int order_counter;
	bool lightweight;
	bool do_collect;
	
public:
	SimBroker();
	
	void Serialize(Stream& s) {
		s % symbol_profits % prev_symbol_profits % symbol_profit_diffs
		  % currency
		  % cycle_time
		  % close_sum
		  % profit_sum % loss_sum
		  % collect_limit % collected
		  % prev_equity
		  % order_counter
		  % lightweight
		  % do_collect;
		Brokerage::Serialize(s);
	}
	
	void Init();
	void InitLightweight();
	virtual void Clear();
	virtual void CollectOnce(double d) {balance -= d;}
	void Cycle();
	void CycleChanges();
	void RefreshOrders();
	
	int FindSymbol(const String& symbol) const;
	int GetSignal(int symbol) const;
	int GetOpenOrderCount() const;
	double PopCloseSum();
	double GetInitialBalance() const {return initial_balance;}
	double GetCloseProfit(const Order& o, double volume) const;
	const Vector<double>& GetSymbolProfits() const {return symbol_profits;}
	const Vector<double>& GetSymbolCycleChanges() const {return symbol_profit_diffs;}
	double GetDrawdown() const;
	double GetCollected() const {return collected;}
	
	void SetPrice(int sym, double price);
	void SetPrice(int sym, double ask, double bid);
	void SetTime(const Time& t) {cycle_time = t;}
	void SetCollecting(double d);
	Order& GetSelected();
	
	// MT4-like functions
	virtual int		iBars(String symbol, int timeframe) {Panic("Not implemented"); return 0;}
	virtual int		iBarShift(String symbol, int timeframe, int datetime) {Panic("Not implemented"); return 0;}
	virtual double	iClose(String symbol, int timeframe, int shift) {Panic("Not implemented"); return 0;}
	virtual double	iHigh(String symbol, int timeframe, int shift) {Panic("Not implemented"); return 0;}
	virtual double	iLow(String symbol, int timeframe, int shift) {Panic("Not implemented"); return 0;}
	virtual double	iOpen(String symbol, int timeframe, int shift) {Panic("Not implemented"); return 0;}
	virtual int		iHighest(String symbol, int timeframe, int type, int count, int start) {Panic("Not implemented"); return 0;}
	virtual int		iLowest(String symbol, int timeframe, int type, int count, int start) {Panic("Not implemented"); return 0;}
	virtual int		iTime(String symbol, int timeframe, int shift) {Panic("Not implemented"); return 0;}
	virtual int		iVolume(String symbol, int timeframe, int shift) {Panic("Not implemented"); return 0;}
	virtual int		RefreshRates();
	virtual Time	GetTime() const;
	virtual double	RealtimeAsk(int sym);
	virtual double	RealtimeBid(int sym);
	virtual int		OrderClose(int ticket, double lots, double price, int slippage);
	virtual double	OrderClosePrice();
	virtual Time	OrderCloseTime();
	virtual String	OrderComment();
	virtual double	OrderCommission();
	virtual int		OrderDelete(int ticket);
	virtual int		OrderExpiration();
	virtual double	OrderLots();
	virtual int		OrderMagicNumber();
	virtual int		OrderModify(int ticket, double price, double stoploss, double takeprofit, Time expiration);
	virtual int		OrderModify(int ticket, double price, double stoploss, double takeprofit, int expiration) {Panic("Not implemented"); return 0;}
	virtual double	OrderOpenPrice();
	virtual Time	OrderOpenTime();
	virtual double	OrderProfit();
	virtual int		OrderSelect(int index, int select, int pool=MODE_TRADES);
	virtual int		OrderSend(String symbol, int cmd, double volume, double price, int slippage, double stoploss, double takeprofit, String comment, int magic, int expiry=0);
	virtual int		OrderSend(int    symbol, int cmd, double volume, double price, int slippage, double stoploss, double takeprofit, String comment, int magic, int expiry=0);
	virtual int		OrdersHistoryTotal();
	virtual double	OrderStopLoss();
	virtual int		OrdersTotal();
	virtual int		HistoryTotal();
	virtual double	OrderSwap();
	virtual String	OrderSymbol();
	virtual double	OrderTakeProfit();
	virtual int		OrderTicket();
	virtual int		OrderType();
	virtual bool    IsDemo();
	virtual bool    IsConnected();
	
	bool IsOrderSelected();
};


class Exposure {
	SimBroker* broker;
	
	// Input
	Vector<double> sym_lots;
	Vector<int> sym_types;
	
	// Mid
	Vector<double> cur_volumes, idx_volumes;
	
	// Output
	
public:
	
	Exposure(SimBroker& sb);
	void Forward();
	void Backward();
	
	
};

}

#endif
