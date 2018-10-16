#ifndef _AsmProto_Generator_h_
#define _AsmProto_Generator_h_




struct PricePressure : Moveable<PricePressure> {
	double low, high;
	double min, max;
	double size;
	bool action;
	int iter;
	
	bool operator()(const PricePressure& a, const PricePressure& b) const {
		if (a.iter < b.iter) return true;
		if (a.iter > b.iter) return false;
		if (a.size < b.size) return true;
		else return false;
	}
};

struct Asm {
	Vector<PricePressure> src;
	int iter = 0;
	
	void Sort() {Upp::Sort(src, PricePressure());}
};

struct Generator {
	static const int data_count = 100;
	
	Vector<PricePressure> active_pressures;
	Asm a;
	double price = 1.0;
	double step = 0.0001;
	int iter = 0;
	
	Generator();
	void AddRandomPressure();
	void Randomize(PricePressure& pp, double price, int iter);
	void GenerateData(Vector<double>& data, bool add_random);
	void GetPricePressure(double price, double& buy_pres, double& sell_pres);
	void ReducePressure(double amount);
	void SimpleReducePressure(double amount);
	
};





#endif
