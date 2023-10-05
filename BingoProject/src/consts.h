#pragma once
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include "./shared/shared.h"


// DL consts

#ifdef _WIN32 // including 64bit 
#ifdef BingoProject_EXPORTS
#define BingoProject_API __declspec(dllexport)
#else
#define BingoProject_API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#ifdef BingoProject_EXPORTS
#define BingoProject_API __attribute__((visibility("default")))
#else
#define BingoProject_API 
#endif
#else
#define BingoProject_API
#endif

// Card consts

#define MAX_BALL_NUM 60
#define MIN_BALL_NUM 1
#define N_ROWS 3
#define N_COLS 5

// Game consts

#define MAX_CARDS 1
#define N_BALLS 30
#define N_MAX_EXTRABALLS 0
#define PRICE_PER_CARD 1
#define PRICE_INCREASE_PER_EXTRABALL 1

// Patterns (X are marked numbers, NUM are non marked numbers)

// Pattern 1
// [[X,			X,		X,		X,		X],
//  [NUM,		X,		X,		X,		NUM],
//  [NUM,		NUM,	X,		NUM,	NUM]]
#define PAYOUT_1 10


// Pattern 2
// [[X,		X,		X,		X,		X],
//  [NUM,	NUM,	X,		NUM,	NUM],
//  [X,		X,		X,		X,		X]]
#define PAYOUT_2 50