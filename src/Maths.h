extern	long B, LB;		// Working base, Log10(base)
extern	long NbDigits;		// actual number of digits calculated
extern	long MaxDiv;		// about sqrt(2^31/B)

extern	void SetToInteger (long n, long *x, long Integer);
extern	long IsZero (long n, long *x);
extern	void Add (long n, long *x, long *y);
extern	void Mul (long n, long *x, long q);
extern	void Print (long n, long *x, char *buffer);
extern	void arccot (long p, long n, long *x, long *buf1, long *buf2);
extern	void arctanh (long p, long n, long *x, long *buf1, long *buf2);
extern	void Sub (long n, long *x, long *y);
extern	void Div (long n, long *x, long d, long *y);	// Division of the big real x by the integer d. The result is y=x/d.
extern	void DivX (long n, long *x, long d);		// Division of the big real x by the integer d. The result is x=x/d.
extern	void Mul64(long n, long *x, __int64 q);		// 64 bit versions
extern	void DivX64(long n, long *x, __int64 d);

#define MIN_LOG 2
#define MAX_LOG 20

struct ConstantInfo
    {
    int		id;
    char	*Description;
    bool        EnableLog;
    bool        EnableSqrt;
    };

struct SqrtData
    {
    int		numerator;
    int		denominator;

    int		xNumerator;
    int		xDenominator;
    };

struct	LogData
    {
    int		NbArctanh;		// number of terms to pass into m*atanh(1/p)
    int		p[4];		// array of p values
    int		m[4];		// array of multipliers
    char	*label;		// for display
    };

