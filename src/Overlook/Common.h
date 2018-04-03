#ifndef _Overlook_Common_h_
#define _Overlook_Common_h_

#undef ASSERTEXC

#undef DLOG
#define DLOG(x)

#define IMAGECLASS OverlookImg
#define IMAGEFILE <Overlook/Overlook.iml>
#include <Draw/iml_header.h>

#ifdef flagMSC
#include <intrin.h>
#endif


#define TEST_SIZE (4*5*24*60)


namespace Overlook {
using namespace Upp;




class AgentGroup;
class Agent;

typedef const int ConstInt;

#define MAX_ITERS 4
#ifdef flagDEBUG
	#define MIN_REAL_TFID 4
	#define MAX_REAL_TFID 5
#else
	#define MIN_REAL_TFID 1
	#define MAX_REAL_TFID 7
#endif

#ifdef flagDEBUG
#define USEDSYMBOL_COUNT 2
#else
#define USEDSYMBOL_COUNT 12
#endif

#define SLOT_TF 1

enum {
	PHASE_TRAINING,
	PHASE_REAL
};

#define FACTORY_LIST \
FITEM(DataSource) \
FITEM(ValueChange) \
FITEM(MovingAverage) \
FITEM(MovingAverageConvergenceDivergence) \
FITEM(AverageDirectionalMovement) \
FITEM(BollingerBands) \
FITEM(Envelopes) \
FITEM(ParabolicSAR) \
FITEM(StandardDeviation) \
FITEM(AverageTrueRange) \
FITEM(BearsPower) \
FITEM(BullsPower) \
FITEM(CommodityChannelIndex) \
FITEM(DeMarker) \
FITEM(ForceIndex) \
FITEM(Momentum) \
FITEM(OsMA) \
FITEM(RelativeStrengthIndex) \
FITEM(RelativeVigorIndex) \
FITEM(StochasticOscillator) \
FITEM(WilliamsPercentRange) \
FITEM(AccumulationDistribution) \
FITEM(MoneyFlowIndex) \
FITEM(ValueAndVolumeTrend) \
FITEM(OnBalanceVolume) \
FITEM(Volumes) \
FITEM(AcceleratorOscillator) \
FITEM(AwesomeOscillator) \
FITEM(Fractals) \
FITEM(FractalOsc) \
FITEM(MarketFacilitationIndex) \
FITEM(LinearTimeFrames) \
FITEM(LinearWeekTime) \
FITEM(SupportResistance) \
FITEM(SupportResistanceOscillator) \
FITEM(Psychological) \
FITEM(ZigZag) \
FITEM(ZigZagOsc) \
FITEM(TrendChange) \
FITEM(TrendChangeEdge) \
FITEM(PeriodicalChange) \
FITEM(VolatilityAverage) \
FITEM(MinimalLabel) \
FITEM(VolatilitySlots) \
FITEM(VolumeSlots) \
FITEM(ChannelOscillator) \
FITEM(ScissorChannelOscillator) \
FITEM(TrendIndex) \
FITEM(OnlineMinimalLabel) \
FITEM(ReactionContext) \
FITEM(VolatilityContext) \
FITEM(SystemBooleanView) \
FITEM(ChannelContext) /*\
FITEM(Obviousness) \
FITEM(VolatilityContextReversal) \
FITEM(ObviousTargetValue)*/

#define FITEM(x) FACTORY_##x ,

enum {
	FACTORY_LIST
	FACTORY_COUNT
};


#define FITEM(x) if (FACTORY_##x == id) return #x;
inline String GetFactoryName(int id) {
	FACTORY_LIST
	return "Unknown";
}


// Visual setting enumerators
enum {DRAW_LINE, DRAW_SECTION, DRAW_HISTOGRAM, DRAW_ARROW, DRAW_ZIGZAG, DRAW_NONE};
enum {WINDOW_CHART, WINDOW_SEPARATE};
enum {STYLE_SOLID, STYLE_DASH, STYLE_DOT, STYLE_DASHDOT, STYLE_DASHDOTDOT};



class OnlineAverageWindow1 : Moveable<OnlineAverageWindow1> {
	Vector<double> win_a;
	double sum_a = 0.0;
	int period = 0, cursor = 0;
	
public:
	OnlineAverageWindow1() {}
	void Clear() {cursor = 0; sum_a = 0.0; for(int i = 0; i < win_a.GetCount(); i++) win_a[i] = 0.0;}
	void SetPeriod(int i) {period = i; win_a.SetCount(i,0);}
	void Add(double a) {
		double& da = win_a[cursor];
		sum_a -= da;
		da = a;
		sum_a += da;
		cursor = (cursor + 1) % period;
	}
	double GetMean() const {return sum_a / period;}
	void Serialize(Stream& s) {s % win_a % sum_a % period % cursor;}
};

class OnlineAverageWindow2 : Moveable<OnlineAverageWindow2> {
	Vector<double> win_a, win_b;
	double sum_a = 0.0, sum_b = 0.0;
	int period = 0, cursor = 0;
	
public:
	OnlineAverageWindow2() {}
	void SetPeriod(int i) {period = i; win_a.SetCount(i,0); win_b.SetCount(i,0);}
	void Add(double a, double b) {
		double& da = win_a[cursor];
		double& db = win_b[cursor];
		sum_a -= da;
		sum_b -= db;
		da = a;
		db = b;
		sum_a += da;
		sum_b += db;
		cursor = (cursor + 1) % period;
	}
	double GetMeanA() const {return sum_a / period;}
	double GetMeanB() const {return sum_b / period;}
	void Serialize(Stream& s) {s % win_a % win_b % sum_a % sum_b % period % cursor;}
};

struct OnlineAverage2 : Moveable<OnlineAverage2> {
	double mean_a, mean_b;
	int64 count;
	OnlineAverage2() : mean_a(0), mean_b(0), count(0) {}
	void Add(double a, double b) {
		if (count == 0) {
			mean_a = a;
			mean_b = b;
		} else {
			double delta_a = a - mean_a; mean_a += delta_a / count;
			double delta_b = b - mean_b; mean_b += delta_b / count;
		}
		count++;
	}
	void Serialize(Stream& s) {s % mean_a % mean_b % count;}
};

struct OnlineAverage1 : Moveable<OnlineAverage1> {
	double mean;
	int64 count;
	OnlineAverage1() : mean(0), count(0) {}
	void Clear() {mean = 0.0; count = 0;}
	void Add(double a) {
		if (count == 0) {
			mean = a;
		} else {
			double delta = a - mean;
			mean += delta / count;
		}
		count++;
	}
	void Serialize(Stream& s) {s % mean % count;}
};

struct AveragePoint : Moveable<OnlineAverage1> {
	OnlineAverage1 x, y;
	int x_mean_int = 0, y_mean_int = 0;
	
	void Clear() {x.Clear(); y.Clear();}
	void Add(double x, double y) {this->x.Add(x); this->y.Add(y); x_mean_int = this->x.mean; y_mean_int = this->y.mean;}
	void Serialize(Stream& s) {s % x % y % x_mean_int % y_mean_int;}
};

inline double StandardNormalCDF(double x) {
	double sum = x;
	double value = x;
	for (int i = 1; i < 100; i++) {
		value = (value * x * x / (2 * i + 1));
		sum += value;
	}
	return 0.5 + (sum / sqrt(2*M_PI)) * pow(M_E, -1* x*x / 2);
}

inline double NormalCDF(double value, double mean, double deviation) {
	if (deviation == 0) {
		if (value < mean) return 0;
		else return 1;
	}
	double d = (value - mean) / deviation;
	d = StandardNormalCDF(d);
	if (!IsFin(d)) {
		if (value < mean) return 0;
		else return 1;
	}
	return d;
}

class OnlineVariance : Moveable<OnlineVariance> {
	
protected:

	// Vars
	int event_count;
	double mean, M2;
	
public:
	
	// Ctors
	OnlineVariance() : event_count(0), mean(0), M2(0) {}
	OnlineVariance(const OnlineVariance& src) {*this = src;}
	OnlineVariance& operator = (const OnlineVariance& src) {
		event_count			= src.event_count;
		mean				= src.mean;
		M2					= src.M2;
		return *this;
	}
	
	// Main funcs
	void AddResult(double value) {
		event_count++;
		double delta = value - mean;
        mean += delta/event_count;
        M2 += delta*(value - mean);
	}
	void Clear() {event_count = 0; mean = 0; M2 = 0;}
	void Serialize(Stream& s) {s % event_count % mean % M2;}
	
	// Get funcs
	int GetEventCount()		const {return event_count;}
	double GetSum()			const {return mean * event_count;}
	double GetMean()		const {return mean;}
	double GetVariance()	const {if (event_count < 2) return 0; else return M2 / (event_count - 1);}
	double GetDeviation()	const {return sqrt(GetVariance());}
	double GetCDF() const {
		if (!event_count) return 0;
		return NormalCDF(0, GetMean(), GetDeviation());
	}
	double GetCDF(double limit, bool rside) const {
		if (!event_count) return 0;
		if (rside == 1)
			return 1 - NormalCDF(limit, GetMean(), GetDeviation());
		else
			return     NormalCDF(limit, GetMean(), GetDeviation());
	}
	String ToString() const {
		return Format("events=%d mean-av=%f stddev=%f cdf=%f", event_count, GetMean(), GetDeviation(), GetCDF());
	}
	
};

struct DerivZeroTrigger {
	int count;
	int16 I = 0;
	int16 read_pos0, read_pos1, read_pos2, write_pos;
	Vector<OnlineAverage1> av;
	
	DerivZeroTrigger() {Clear();}
	int GetPeriod() const {return I;}
	void SetPeriod(int i) {
		ASSERT(i > 0);
		I = i;
		Clear();
	}
	void Clear() {
		count = 0;
		read_pos2 = 3;
		read_pos1 = 2;
		read_pos0 = 1;
		write_pos = 0;
		av.SetCount(I+2);
		for(int i = 0; i < I+2; i++)
			av[i].Clear();
	}
	void Add(double d) {
		ASSERT(I > 0);
		write_pos = read_pos0;
		read_pos0 = read_pos1;
		read_pos1 = read_pos2;
		read_pos2 = (read_pos2 + 1) % (I+2);
		av[write_pos].Clear();
		for(int i = 0; i < I; i++)
			av[(read_pos2 + i) % (I+2)].Add(d);
		count++;
	}
	bool IsTriggered() const {
		if (count < I+2) return false;
		const OnlineAverage1& av0 = av[read_pos0];
		const OnlineAverage1& av1 = av[read_pos1];
		const OnlineAverage1& av2 = av[read_pos2];
		ASSERT(av0.count == av1.count);
		ASSERT(av0.count == av2.count);
		double diff0 = av0.mean - av1.mean;
		double diff1 = av1.mean - av2.mean;
		return diff0 * diff1 <= 0.0;
	}
	double GetChange() const {
		if (count < I+2) return 0.0;
		const OnlineAverage1& av1 = av[read_pos1];
		const OnlineAverage1& av2 = av[read_pos2];
		ASSERT(av1.count == av2.count);
		return av2.mean / av1.mean - 1.0;
	}
};

struct ValueBase {
	int count=0, visible=0, data_type=-1, min=-1, max=-1, factory=-1;
	const char* s0 = "";
	void* data = NULL;
	void* data2 = NULL;
	ValueBase() {}
	virtual ~ValueBase() {}
	enum {IN_, INOPT_, OUT_, BOOL_, INT_, PERS_};
	void operator = (const ValueBase& vb) {
		count		= vb.count;
		visible		= vb.visible;
		data_type	= vb.data_type;
		min			= vb.min;
		max			= vb.max;
		factory		= vb.factory;
		s0			= vb.s0;
		data		= vb.data;
		data2		= vb.data2;
	}
};


struct FactoryDeclaration {

	// Persistent
	int args[8];
	int factory = -1;
	int arg_count = 0;

	// Temp
	int input_id[8];
	int input_count = 0;
	int buffer_count = 0;


	FactoryDeclaration() {}
	FactoryDeclaration(const FactoryDeclaration& src) {*this = src;}
	FactoryDeclaration& Set(int i) {factory = i; return *this;}
	FactoryDeclaration& AddArg(int i) {ASSERT(arg_count >= 0 && arg_count < 8); args[arg_count++] = i; return *this;}
	FactoryDeclaration& operator=(const FactoryDeclaration& src) {
		factory = src.factory;
		arg_count = src.arg_count;
		for(int i = 0; i < 8; i++) args[i] = src.args[i];
		for(int i = 0; i < 8; i++) input_id[i] = src.input_id[i];
		input_count = src.input_count;
		buffer_count = src.buffer_count;
		return *this;
	}
	void Reset() {arg_count = 0; factory = -1;}
	bool operator==(const FactoryDeclaration& src) {
		if (factory != src.factory) return false;
		if (arg_count != src.arg_count) return false;
		for(int i = 0; i < src.arg_count; i++)
			if (args[i] != src.args[i])
				return false;
		return true;
	}

	unsigned GetHashValue() {
		CombineHash ch;
		ch << factory << 1;
		for(int i = 0; i < arg_count; i++)
			ch << args[i] << 1;
		return ch;
	}

	String ToString() const {
		String s;
		s << "factory=" << factory << " arg_count=" << arg_count << " ";
		for(int i = 0; i < arg_count; i++) s << "a" << i << "=" << args[i] << " ";
		s << "input_count=" << input_count << " ";
		for(int i = 0; i < input_count; i++) s << "in" << i << "=" << input_id[i] << " ";
		return s;
	}
	void Serialize(Stream& s) {if (s.IsLoading()) s.Get(this, sizeof(FactoryDeclaration)); else s.Put(this, sizeof(FactoryDeclaration));}
};


typedef const FactoryDeclaration ConstFactoryDeclaration;

struct ValueRegister {
	struct ArgDecl {
		int def = 0, min = 0, max = 0;
	};
	FactoryDeclaration inputs[8];
	ArgDecl args[8];
	int input_count = 0;
	int arg_count = 0;
	int arg_cursor = 0;
	int output_count = -1, output_visible = -1, output_boolean_count = -1;
	
	bool is_loading = false;
	
	
	ValueRegister() {}
	ValueRegister(ConstFactoryDeclaration& decl);
	
	void Reset() {
		arg_count = 0;
		arg_cursor = 0;
		for(int i = 0; i < 8; i++) inputs[i].Reset();
	}
	ValueRegister& In(int factory);
	ValueRegister& In(int factory, int arg0);
	ValueRegister& In(int factory, int arg0, int arg1);
	ValueRegister& In(int factory, int arg0, int arg1, int arg2);
	ValueRegister& In(int factory, int arg0, int arg1, int arg2, int arg3);
	ValueRegister& Out(int bufcount, int bufvisible, int boolcount);
	ValueRegister& Arg(int& def_value, int min_value=0, int max_value=10000);
	
	template <class T> ValueRegister& Mem(T& obj){return *this;}
};

/*
// Utility function for changing class arguments
struct ArgChanger : public ValueRegister {
	ArgChanger() : cursor(0), storing(0) {}
	
	virtual void IO(const ValueBase& base) {
		if (!storing) {
			keys.SetCount(cursor+1);
			keys[cursor] = base.s0;
			args.SetCount(cursor+1);
			if (base.data_type == ValueBase::BOOL_)
				args[cursor++] = *(bool*)base.data;
			else if (base.data_type == ValueBase::INT_)
				args[cursor++] = *(int*)base.data;
		} else {
			if (base.data_type == ValueBase::BOOL_)
				*(bool*)base.data = args[cursor++];
			else if (base.data_type == ValueBase::INT_)
				*(int*)base.data = args[cursor++];
		}
	}
	void SetLoading() {storing = false; cursor = 0;}
	void SetStoring() {storing = true;  cursor = 0;}
	
	Vector<Value> args;
	Vector<String> keys;
	int cursor;
	bool storing;
};
*/


struct DataExc : public Exc {
	DataExc() {
		#ifdef flagDEBUG
		Panic("debug DataExc");
		#endif
	}
	
	DataExc(String msg) : Exc(msg) {
		#ifdef flagDEBUG
		Panic(msg);
		#endif
	}
};
#define ASSERTEXC(x) if (!(x)) throw ::Overlook::DataExc(#x);
#define ASSERTEXC_(x, msg) if (!(x)) throw ::Overlook::DataExc(msg);


struct UserExc : public Exc {
	UserExc() {}
	UserExc(String msg) : Exc(msg) {}
};
#define ASSERTUSER(x) if (!(x)) throw ::Overlook::UserExc(#x);
#define ASSERTUSER_(x, msg) if (!(x)) throw ::Overlook::UserExc(msg);



// Helper macros for indicator short names
#define SHORTNAME0(x) x
#define SHORTNAME1(x, a1) x "(" + DblStr(a1) + ")"
#define SHORTNAME2(x, a1, a2) x "(" + DblStr(a1) + "," + DblStr(a2) + ")"
#define SHORTNAME3(x, a1, a2, a3) x "(" + DblStr(a1) + "," + DblStr(a2) + "," + DblStr(a3) + ")"
#define SHORTNAME4(x, a1, a2, a3, a4) x "(" + DblStr(a1) + "," + DblStr(a2) + "," + DblStr(a3) + "," + DblStr(a4) + ")"
#define SHORTNAME5(x, a1, a2, a3, a4, a5) x "(" + DblStr(a1) + "," + DblStr(a2) + "," + DblStr(a3) + "," + DblStr(a4) + "," + DblStr(a5) + ")"
#define SHORTNAME6(x, a1, a2, a3, a4, a5, a6) x "(" + DblStr(a1) + "," + DblStr(a2) + "," + DblStr(a3) + "," + DblStr(a4) + "," + DblStr(a5) + "," + DblStr(a6) +  ")"
#define SHORTNAME7(x, a1, a2, a3, a4, a5, a6, a7) x "(" + DblStr(a1) + "," + DblStr(a2) + "," + DblStr(a3) + "," + DblStr(a4) + "," + DblStr(a5) + "," + DblStr(a6) + "," + DblStr(a7) + ")"
#define SHORTNAME8(x, a1, a2, a3, a4, a5, a6, a7, a8) x "(" + DblStr(a1) + "," + DblStr(a2) + "," + DblStr(a3) + "," + DblStr(a4) + "," + DblStr(a5) + "," + DblStr(a6) + "," + DblStr(a7) + "," + DblStr(a8) + ")"

typedef Vector<byte> CoreData;

class Core;

struct BatchPartStatus : Moveable<BatchPartStatus> {
	BatchPartStatus() {slot = NULL; begin = Time(1970,1,1); end = begin; sym_id = -1; tf_id = -1; actual = 0; total = 1; complete = false; batch_slot = 0;}
	Core* slot;
	Time begin, end;
	int sym_id, tf_id, actual, total;
	byte batch_slot;
	bool complete;
	
	void Serialize(Stream& s) {
		s % begin % end % sym_id % tf_id % actual % total % batch_slot % complete;
	}
};

template <class T>
String HexVector(const Vector<T>& vec) {
	String s;
	int byts = sizeof(T);
	int chrs = sizeof(T) * 2;
	typedef const T ConstT;
	ConstT* o = vec.Begin();
	for(int i = 0; i < vec.GetCount(); i++) {
		T mask = 0xFF << ((byts-1) * 8);
		for(int j = 0; j < byts; j++) {
			byte b = (mask & *o) >> ((byts-1-j) * 8);
			int d0 = b >> 4;
			int d1 = b & 0xF;
			s.Cat( d0 < 10 ? '0' + d0 : 'A' + d0);
			s.Cat( d1 < 10 ? '0' + d1 : 'A' + d1);
			mask >>= 8;
		}
		o++;
	}
	return s;
}

inline Color Silver() {return Color(192, 192, 192);}
inline Color Wheat() {return Color(245, 222, 179);}
inline Color LightSeaGreen() {return Color(32, 178, 170);}
inline Color YellowGreen() {return Color(154, 205, 50);}
inline Color Lime() {return Color(0, 255, 0);}
inline Color DodgerBlue() {return Color(30, 144, 255);}
inline Color SaddleBrown() {return Color(139, 69, 19);}
inline Color Pink() {return Color(255, 192, 203);}
inline Color RainbowColor(double progress) {
    int div = (int)(progress * 6);
    int ascending = (int) (progress * 255);
    int descending = 255 - ascending;

    switch (div)
    {
        case 0:
            return Color(255, ascending, 0);
        case 1:
            return Color(descending, 255, 0);
        case 2:
            return Color(0, 255, ascending);
        case 3:
            return Color(0, descending, 255);
        case 4:
            return Color(ascending, 0, 255);
        default: // case 5:
            return Color(255, 0, descending);
    }
}

inline int IncreaseMonthTS(int ts) {
	Time t(1970,1,1);
	int64 epoch = t.Get();
	t += ts;
	int year = t.year;
	int month = t.month;
	month++;
	if (month == 13) {year++; month=1;}
	return (int)(Time(year,month,1).Get() - epoch);
}

class CoreIO;

typedef const double ConstDouble;



typedef const uint64 ConstU64;

class VectorBool : Moveable<VectorBool> {
	
protected:
	friend class ImageCompiler;
	
	Vector<uint64> data;
	int count = 0;
	int64 data_begin = 0;
	
public:
	VectorBool() {}
	VectorBool(const VectorBool& src) {*this = src;}
	void operator=(const VectorBool& src);
	
	int GetCount() const;
	int GetEnd() const {return data_begin + GetCount();}
	int PopCount() const;
	VectorBool& SetCount(int i);
	VectorBool& Reserve(int i);
	VectorBool& Zero();
	VectorBool& One();
	VectorBool& SetInverse(const VectorBool& b);
	VectorBool& InverseAnd(const VectorBool& b);
	VectorBool& And(const VectorBool& b);
	VectorBool& Or(const VectorBool& b);
	double GetOverlapFactor(const VectorBool& b) const;
	
	bool Get(int64 i) const;
	void Set(int64 i, bool b);
	void LimitLeft(int i);
	void LimitRight(int i);
	
	ConstU64* Begin() const;
	ConstU64* End() const;
	uint64*   Begin();
	uint64*   End();
	
	void Serialize(Stream& s) {s % data % count;}
};

typedef const VectorBool	ConstVectorBool;
//typedef const Buffer		ConstBuffer;


struct BufferImage : Moveable<BufferImage> {
	Vector<double> value;
	Color clr = Black();
	double min = 0, max = 0;
	int style = 0, line_style = 0, line_width = 1, chr = '*', shift = 0, begin = 0;
	int data_begin = 0;
	char label[10];
	
	
	double GetMaximum() const {return max;}
	double GetMinimum() const {return min;}
	int GetCount() const {return value.GetCount();}
	bool IsEmpty() const {return value.IsEmpty();}
	void Set(int i, double d) {value[i - data_begin] = d;}
	double Get(int i) const {return value[i - data_begin];}
	double operator[](int i) const {return value[i - data_begin];}
	int GetEnd() const {return data_begin + value.GetCount();}
};

typedef const BufferImage ConstBufferImage;

struct GraphImage : Moveable<GraphImage> {
	struct LevelSettings {
		double value = 0;
		int style = 0;
		int line_width = 1;
	};
	static const int MAX_SETTINGS = 4;
	
	Vector<BufferImage> buffers;
	Vector<VectorBool> booleans;
	LevelSettings levels[MAX_SETTINGS];
	ValueRegister reg;
	Color levels_clr;
	double minimum = 0, maximum = 0;
	int levels_style = 0;
	int level_count = 0;
	int window_type = 0;
	int input_id[8];
	int input_count = 0;
	int future_bars = 0;
	int factory = -1;
	bool is_fixed_max = false, is_fixed_min = false;
	
	
	void SetBoolean(int shift, int row, bool value) {booleans[row].Set(shift, value);}
	bool GetBoolean(int shift, int row) {return booleans[row].Get(shift);}
	
	
	void RefreshLimits();
	double GetMaximum() const {return maximum;}
	double GetMinimum() const {return minimum;}
	bool HasMaximum() const {return is_fixed_max;}
	bool HasMinimum() const {return is_fixed_min;}
	int GetCount() const {return buffers.GetCount();}
	int GetVisibleCount() const {return reg.output_visible;}
	BufferImage& GetBuffer(int i) {return buffers[i];}
	bool IsCoreSeparateWindow() {return window_type == WINDOW_SEPARATE;}
	int GetFutureBars() const {return future_bars;}
	int GetFactory() const {return factory;}
	double GetBufferValue(int buf, int shift) {return buffers[buf].Get(shift);}
	Color GetBufferColor(int i) {return buffers[i].clr;}
	int GetBufferStyle(int i) {return buffers[i].style;}
	int GetBufferArrow(int i) {return buffers[i].chr;}
	int GetBufferLineWidth(int i) {return buffers[i].line_width;}
	int GetBufferType(int i) {return buffers[i].line_style;}
	int GetBars() const {return buffers.Top().GetEnd();}
	int GetCoreLevelCount() const {return level_count;}
	int GetCoreLevelType(int i) const {return levels[i].style;}
	int GetCoreLevelLineWidth(int i) const {return levels[i].line_width;}
	double GetCoreLevelValue(int i) const {return levels[i].value;}
	
	void SetCoreChartWindow() {window_type = WINDOW_CHART;}
	void SetCoreSeparateWindow() {window_type = WINDOW_SEPARATE;}
	void SetCoreLevelCount(int count) {ASSERT(count >= 0 && count < MAX_SETTINGS); level_count = count;}
	void SetCoreLevel(int i, double value) {levels[i].value = value;}
	void SetCoreLevelType(int i, int style) {levels[i].style = style;}
	void SetCoreLevelLineWidth(int i, int line_width) {levels[i].line_width = line_width;}
	void SetCoreLevelsColor(Color clr) {levels_clr = clr;}
	void SetCoreLevelsStyle(int style) {levels_style = style;}
	void SetCoreMinimum(double value) {minimum = value; is_fixed_min = true;}
	void SetCoreMaximum(double value) {maximum = value; is_fixed_max = true;}
	void SetBufferColor(int i, Color c) {buffers[i].clr = c;}
	void SetBufferLineWidth(int i, int line_width) {buffers[i].line_width = line_width;}
	void SetBufferType(int i, int style) {buffers[i].line_style = style;}
	void SetBufferStyle(int i, int style) {buffers[i].style = style;}
	void SetBufferShift(int i, int shift) {buffers[i].shift = shift;}
	void SetBufferBegin(int i, int begin) {buffers[i].begin = begin;}
	void SetBufferArrow(int i, int chr)   {buffers[i].chr = chr;}
	
};

struct ChartImage {
	Vector<GraphImage> graphs;
	double point = 0.0;
	int symbol = -1, tf = -1, period = -1;
	int begin = 0, end = 0;
	int cursor = -1;
	
	int GetSymbol() const {return symbol;}
	int GetTf() const {return tf;}
	int GetPeriod() const {return period;}
	GraphImage& GetGraph(int i) {return graphs[i];}
	int GetCount() {return graphs.GetCount();}
	
	
	int GetBegin() const {return begin;}
	int GetEnd() const {return end;}
	double GetPoint() {return point;}
	
	VectorBool& GetInputBoolean(int in, int b) {return graphs[graphs[cursor].input_id[in]].booleans[b];}
	ConstBufferImage& GetInputBuffer(int in, int buf) {return graphs[graphs[cursor].input_id[in]].GetBuffer(buf);}
	
};













struct Downloader {
	HttpRequest http;
	int64       loaded, prev_loaded;
	String      url;
	FileOut     out;
	String      path;
	
	typedef Downloader CLASSNAME;
	
	
	Downloader()
	{
		prev_loaded = 0;
		http.UserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		http.MaxContentSize(INT_MAX);
		http.WhenContent = THISBACK(Content);
		http.WhenWait = http.WhenDo = THISBACK(ShowProgress);
		http.WhenStart = THISBACK(Start);
	}
	
	void Start()
	{
		if(out.IsOpen()) {
			out.Close();
			DeleteFile(path);
		}
		loaded = 0;
	}
	
	void Perform()
	{
		http.New();
		http.Url(url).Execute();
		if(out.IsOpen())
			out.Close();
		if(!http.IsSuccess()) {
			DeleteFile(path);
			LOG("Download has failed.&\1" +
			            (http.IsError() ? http.GetErrorDesc()
			                            : AsString(http.GetStatusCode()) + ' ' + http.GetReasonPhrase()));
		}
	}
	
	void Content(const void *ptr, int size)
	{
		loaded += size;
		if(!out.IsOpen()) {
			RealizePath(path);
			out.Open(path);
		}
		out.Put(ptr, size);
	}
	
	void ShowProgress() {
		if (loaded > prev_loaded + 1000000) {
			LOG("Downloading " << (int)loaded << "/" << (int)http.GetContentLength());
			prev_loaded = loaded;
		}
	}

};

inline int GetUsedCpuCores() {
	static int cores;
	if (!cores) cores = CPU_Cores();
	
	return Upp::max(1, cores - 2); // Leave a little for the system
}

inline unsigned int root(unsigned int x) {
	unsigned int a, b;
	b     = x;
	a = x = 0x3f;
	x     = b / x;
	a = x = (x + a) >> 1;
	x     = b / x;
	a = x = (x + a) >> 1;
	x     = b / x;
	x     = (x + a) >> 1;
	return x;
}

inline int StrPut(TcpSocket& out, void* data, int count) {
	void* mem = MemoryAlloc(count);
	byte* buf = (byte*)data;
	byte* tmp = (byte*)mem;
	buf += count-1;
	for(int i = 0; i < count; i++) {
		*tmp = *buf;
		buf--; tmp++;
	}
	int res = out.Put(mem, count);
	MemoryFree(mem);
	return res;
}
inline int StrGet(TcpSocket& in, void* data, int count) {
	void* mem = MemoryAlloc(count);
	byte* buf = (byte*)data;
	byte* tmp = (byte*)mem;
	int res = in.Get(mem, count);
	buf += res-1;
	for(int i = 0; i < res; i++) {
		*buf = *tmp;
		buf--; tmp++;
	}
	MemoryFree(mem);
	return res;
}
inline int StrPut(Stream& out, void* data, int count) {
	void* mem = MemoryAlloc(count);
	byte* buf = (byte*)data;
	byte* tmp = (byte*)mem;
	buf += count-1;
	for(int i = 0; i < count; i++) {
		*tmp = *buf;
		buf--; tmp++;
	}
	out.Put(mem, count);
	MemoryFree(mem);
	return 0;
}
inline int StrGet(Stream& in, void* data, int count) {
	void* mem = MemoryAlloc(count);
	byte* buf = (byte*)data;
	byte* tmp = (byte*)mem;
	in.Get(mem, count);
	buf += count-1;
	for(int i = 0; i < count; i++) {
		*buf = *tmp;
		buf--; tmp++;
	}
	MemoryFree(mem);
	return 0;
}

inline Time TimeFromTimestamp(int64 seconds) {
	return Time(1970, 1, 1) + seconds;
}

inline int PopCount64(uint64 i) {
	#ifdef flagMSC
	#if CPU_64
	return __popcnt64(i);
	#elif CPU_32
	return __popcnt(i) + __popcnt(i >> 32);
	#endif
	#else
	return __builtin_popcountll(i);
	#endif
}


// Reduce complexity: e.g. for zigzag

struct ExtremumCache : Moveable<ExtremumCache> {
	Vector<double> max, min;
	double max_value = -DBL_MAX, min_value = DBL_MAX;
	int pos = -1, size = 0, max_left = 0, min_left = 0;
	
	ExtremumCache(int size=0) {
		SetSize(size);
	}
	
	void SetSize(int size) {
		this->size = size;
		max.SetCount(size, -DBL_MAX);
		min.SetCount(size, DBL_MAX);
	}
	
	void Serialize(Stream& s) {
		s % max % min % max_value % min_value % pos % size % max_left % min_left;
	}
	
	void Add(double low, double high) {
		pos++;
		int write_pos = pos % size;
		
		max_left--;
		max[write_pos] = high;
		if (max_left <= 0) {
			max_value = -DBL_MAX;
			for(int i = 0; i < size; i++) {
				double d = max[i];
				if (d > max_value) {
					if (i <= write_pos)	max_left = size - (write_pos - i);
					else				max_left = i - write_pos;
					max_value = d;
				}
			}
		}
		else if (high >= max_value) {
			max_left = size;
			max_value = high;
		}
		
		min_left--;
		min[write_pos] = low;
		if (min_left <= 0) {
			min_value = DBL_MAX;
			for(int i = 0; i < size; i++) {
				double d = min[i];
				if (d < min_value) {
					if (i <= write_pos)	min_left = size - (write_pos - i);
					else				min_left = i - write_pos;
					min_value = d;
				}
			}
		}
		else if (low <= min_value) {
			min_left = size;
			min_value = low;
		}
	}
	
	int GetHighest() {
		return pos - (size - max_left);
	}
	
	int GetLowest() {
		return pos - (size - min_left);
	}
};

void TestExtremumCache();


struct ArrayCtrlPrinter {
	ArrayCtrl* list = NULL;
	int i = 0;

	ArrayCtrlPrinter(ArrayCtrl& list) : list(&list) {}

	template <class K, class T>
	void Add(K key, T value) {
		list->Set(i, 0, AsString(key));
		list->Set(i, 1, AsString(value));
		i++;
	}

	void Title(String key) {
		list->Set(i, 0, "");  list->Set(i, 1, ""); i++;
		list->Set(i, 0, key); list->Set(i, 1, ""); i++;
	}
};


// Many Ctrl classes don't have hook for right click context menu, which is added here.
// The situation is confusing...
// TODO: find a supported way
template <class T>
class CtrlCallbacks : public T {
	bool visible = false;
public:
	
    virtual void LeftDown(Point pt, dword v) {T::LeftDown(pt, v); WhenLeftDown(pt, v);}
    virtual void RightDown(Point pt, dword v) {T::RightDown(pt, v); WhenRightDown(pt, v);}
	virtual void Layout() {bool v = Ctrl::IsVisible(); if (v == true && visible == false) WhenVisible(); visible = v; T::Layout();}
	
    Callback2<Point, dword> WhenRightDown, WhenLeftDown;
    Callback WhenVisible;
};


typedef void (*ArgsFn)(int input, FactoryDeclaration& decl, const Vector<int>& args);


void DrawVectorPoints(Draw& d, Size sz, const Vector<double>& pts);
void DrawVectorPolyline(Draw& d, Size sz, const Vector<double>& pts, Vector<Point>& cache);



inline int HashSymTf(int sym, int tf) {return sym * 1000 + tf;}

#define LOCK(x) for(int i_l_ = 0; ([&]()->bool {if (!i_l_) x.Enter(); else x.Leave(); return false;})() || i_l_ < 1; i_l_++)
#define READLOCK(x) for(int i_l_ = 0; ([&]()->bool {if (!i_l_) x.EnterRead(); else x.LeaveRead(); return false;})() || i_l_ < 1; i_l_++)
#define WRITELOCK(x) for(int i_l_ = 0; ([&]()->bool {if (!i_l_) x.EnterWrite(); else x.LeaveWrite(); return false;})() || i_l_ < 1; i_l_++)
#define TRYLOCK(x) for(int i_l_ = 0; ([&]()->bool {if (!i_l_) {if (!x.TryEnter()) i_l_++;} else x.Leave(); return false;})() || i_l_ < 1; i_l_++)
void TestLockMacro();


#define INSPECT(x, msg) if (!(x)) {GetSystem().InspectionFailed(__FILE__, __LINE__, GetSymbol(), GetTf(), msg);}




/*
struct ConstBufferSource {
	Vector<ConstBuffer*> bufs;
	int serial_depth = 0;
	bool serial_mode = false;
	
public:
	ConstBufferSource();
	
	void SetDepth(int i) {bufs.SetCount(i, NULL);}
	void SetSource(int i, ConstBuffer& buf) {ASSERT(&buf); bufs[i] = &buf;}
	void SetSerialDepth(int i) {serial_mode = true; serial_depth = i; bufs.SetCount(1, NULL);}
	int GetCount() const;
	int GetDepth() const;
	
	double Get(int pos, int depth) const;
	
};

class ConstBufferSourceIter {
	typedef const ConstBufferSource ConstConstBufferSource;
	ConstConstBufferSource* src;
	ConstInt* cursor_ptr = NULL;
public:
	ConstBufferSourceIter(const ConstBufferSource& src, ConstInt* cursor_ptr);
	
	double operator[](int i) const;
	
	const ConstBufferSource& GetSource() const {return *src;}
	int GetCursor() const {return *cursor_ptr;}
	
};


struct AssistItem : Moveable<AssistItem> {
	String msg;
	
	
	void Set(String msg) {this->msg = msg;}
};

struct AssistBase {
	Vector<AssistItem> items;
	
	
	void Add(String msg) {items.Add().Set(msg);}
};
*/
int log2_64 (uint64 value);




inline void ReleaseLog(String s) {FileAppend fout(ConfigFile("release.log")); fout << s; fout.PutEol(); LOG(s);}

}

#endif
