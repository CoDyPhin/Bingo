#pragma once
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include "./shared/shared.h"


// DL macros

#ifdef _WIN32 // including 64bit 
#ifdef BingoProject_EXPORTS
#define BingoProject_API __declspec(dllexport)
#endif
#elif defined(__GNUC__)
#ifdef BingoProject_EXPORTS
#define BingoProject_API __attribute__((visibility("default")))
#else
#define BingoProject_API 
#endif
#endif

#define MacroConst inline constexpr unsigned

// Card consts

MacroConst MAX_BALL_NUM = 60;
MacroConst MIN_BALL_NUM = 1;
MacroConst N_ROWS = 3;
MacroConst N_COLS = 5;

// Game consts

MacroConst N_MAX_EXTRABALLS = 10;
MacroConst PRICE_PER_CARD = 1;
MacroConst PRICE_INCREASE_PER_EXTRABALL = 1;

// Shared game macros

#define MAX_CARDS 1
#define N_BALLS 30

// Patterns (X are marked numbers, NUM are non marked numbers)

MacroConst N_PATTERNS = 2;

// Pattern 1
// [[X,			X,		X,		X,		X],
//  [NUM,		X,		X,		X,		NUM],
//  [NUM,		NUM,	X,		NUM,	NUM]]
MacroConst PAYOUT_1 = 10;


// Pattern 2
// [[X,		X,		X,		X,		X],
//  [NUM,	NUM,	X,		NUM,	NUM],
//  [X,		X,		X,		X,		X]]
MacroConst PAYOUT_2 = 50;