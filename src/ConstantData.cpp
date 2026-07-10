#include "maths.h"
#include "resource.h"

extern const struct ConstantInfo ConstantData[] =
    {
    { 
    IDC_PI,
    "The ratio of a circle's circumference to its diameter.\r\n"
    "\r\n"
    "Probably the most famous mathematical constant.\r\n"
    "\r\n"
    "Approximately 3.1415926535...",
    false, false
    },
    {
    IDC_PHI,
    "The limit of successive Fibonacci ratios.\r\n"
    "\r\n"
    "Appears throughout geometry, art and nature.\r\n"
    "\r\n"
    "Approximately 1.6180339887...",
    false, false
    },
    {
    IDC_E,
    "The base of the natural logarithm.\r\n"
    "\r\n"
    "Computed using the classical Taylor series.\r\n"
    "\r\n"
    "e = 1 + 1 + 1/2! + 1/3! + ...\r\n"
    "\r\n"
    "Approximately 2.718281828459045...",
    false, false
    },
    {
    IDC_CATALAN,
    "Appears in combinatorics, lattice sums,\r\n"
    "\r\n"
    "Fourier series and elliptic integrals.\r\n"
    "\r\n"
    "Approximately 0.915965594...",
    false, false
    },
    {
    IDC_APERY,
    "The value of the Riemann zeta function at 3.\r\n"
    "\r\n"
    "Proven irrational by Roger Apéry in 1978.\r\n"
    "\r\n"
    "Approximately 1.202056903...",
    false, false
    },
    {
    IDC_EFAST,
    "The base of the natural logarithm.\r\n"
    "\r\n"
    "Computed using a Fast Fourier\r\n"
    "Transform (FFT) multiplication\r\n"
    "algorithm for very high precision.\r\n"
    "\r\n"
    "Significantly faster for large numbers of digits.\r\n"
    "\r\n"
    "Approximately 2.718281828459045...",
    false, false
    },
    {
    IDC_LOG,
    "Computes ln(n).\r\n"
    "\r\n"
    "Valid for integers from 2 to 20.\r\n"
    "\r\n"
    "Uses Pascal Sebah's rapidly convergent atanh series.",
    true, false
    },
    {
    IDC_SQRT,
    "Computes sqrt(n).\r\n"
    "\r\n"
    "Valid for integers from 2 to 20.\r\n"
    "\r\n"
    "Uses Pascal Sebah's rapidly convergent square-root series.",
    false, true
    },
    { 0, nullptr, false, false }
    };    


extern const SqrtData SqrtTable[] =
    {
    {0,0,0,0},			    // 0
    {0,0,0,0},			    // 1

    {239, 169, 1, 57122},	    // sqrt(2)
    {265, 153, 2, 70227},	    // sqrt(3)
    {0, 0, 0, 0},		    // sqrt(4) exact

    {682,305,1,465125},         // sqrt(5)
    {218,89,2,47526},           // sqrt(6)
    {1307,494,3,1708252},       // sqrt(7)
    {478,169,1,57122},          // sqrt(8)

    {0,0,0,0},                  // sqrt(9) exact

    {117,37,1,13690},           // sqrt(10)
    {1257,379,2,1580051},       // sqrt(11)
    {627,181,3,393132},         // sqrt(12)
    {18,5,1,325},               // sqrt(13)
    {333,89,5,110894},          // sqrt(14)
    {1677,433,6,2812335},       // sqrt(15)

    {0,0,0,0},                  // sqrt(16) exact

    {268,65,1,71825},           // sqrt(17)
    {140,33,2,19602},           // sqrt(18)
    {13,3,2,171},               // sqrt(19)
    {1364,305,4,1860500},       // sqrt(20)
    {0,0,0,0}
    };

extern	const 	LogData   LogTable[] = 
    {
	{
	// Log(2) = 14*atanh(1/31)+10*atanh(1/49)+6*atanh(1/161)
	3, 31, 49, 161, 0, 14, 10, 6, 0, "Constant Log 2 = "
	},
	{
	// Log(3) = 22*atanh(1/31)+16*atanh(1/49)+10*atanh(1/161)
	3, 31, 49, 161, 0, 22, 16, 10, 0, "Constant Log 3 = "
	},
	{
	// Log(4) = Log(2) * 2
	3, 31, 49, 161, 0, 28, 20, 12, 0, "Constant Log 4 = "
	},
	{
	// Log(5) = 22*atanh(1/31)+24*atanh(1/49)+14*atanh(1/161)
	3, 31, 49, 161, 0, 32, 24, 14, 0, "Constant Log 5 = "
	},
	{
	// Log(6) = Log(2) + Log(3)
	3, 31, 49, 161, 0, 36, 26, 16, 0, "Constant Log 6 = "
	},
	{
	// Log(7) = Log(6) + 2*arctanh(1/13) because Log(n) = Log(n - 1) + 2*arctanh(1/(2n - 1))
	4, 31, 49, 161, 13, 36, 26, 16, 2, "Constant Log 7 = "
	},
	{
	// Log(8) = Log(2) * 3
	3, 31, 49, 161, 0, 42, 30, 18, 0, "Constant Log 8 = "
	},
	{
	// Log(9) = Log(3) * 2
	3, 31, 49, 161, 0, 44, 32, 20, 0, "Constant Log 9 = "
	},
	{
	// Log(10) = Log(2) + Log(5)
	3, 31, 49, 161, 0, 46, 34, 20, 0, "Constant Log 10 = "
	},
	{
	// Log(11) = Log(10) + 2*atanh(1/21)
	4, 31, 49, 161, 21, 46, 34, 20, 2, "Constant Log 11 = "
	},
	{
	// Log(12) = Log(3) + 2*Log(2)
	3, 31, 49, 161, 0, 50, 36, 22, 0, "Constant Log 12 = "
	},
	{
	// Log(13) = Log(12) + 2*atanh(1/25)
	4, 31, 49, 161, 25, 50, 36, 22, 2, "Constant Log 13 = "
	},
	{
	// Log(14) = Log(2) + Log(7)
	4, 31, 49, 161, 13, 50, 36, 22, 2, "Constant Log 14 = "
	},
	{
	// Log(15) = Log(3) + Log(5)
	3, 31, 49, 161, 0, 54, 40, 24, 0, "Constant Log 15 = "
	},
	{
	// Log(16) = Log(2) * 4
	3, 31, 49, 161, 0, 56, 40, 24, 0, "Constant Log 16 = "
	},
	{
	// Log(17) = Log(16) + 2*atanh(1/33)
	4, 31, 49, 161, 33, 56, 40, 24, 2, "Constant Log 17 = "
	},
	{
	// Log(18) = Log(2) + 2*Log(3)
	3, 31, 49, 161, 0, 58, 42, 26, 0, "Constant Log 18 = "
	},
	{
	// Log(19) = Log(18) + 2*atanh(1/37)
	4, 31, 49, 161, 37, 58, 42, 26, 2, "Constant Log 19 = "
	},
	{
	// Log(20) = 2*Log(2) + Log(5)
	3, 31, 49, 161, 0, 60, 44, 26, 0, "Constant Log 20 = "
	},
	{
	0,0,0,0,0,0,0,0,0, nullptr
	}
    };
