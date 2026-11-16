#pragma once

// Shared enums and constants for the Car Service Center project

// Status of a car (see "Car Status" table in the project description)
enum class CarStatus
{
	IDLE,	// has not arrived yet (VT > current timestep)
	ERLY,	// arrived before its appointment (VT < PT)
	LATE,	// arrived after its appointment (VT > PT)
	REDY,	// ready to be moved to the diagnosis unit
	DIAG,	// being checked by the diagnosis unit
	WAIT,	// waiting for a service to start
	SERV,	// being served in a service bay
	CNCL,	// cancelled
	FNSH	// finished ALL services
};

// The four units of the center. MU/EU/WU are also the service types.
enum class UnitType
{
	DU,		// Diagnosis Unit
	MU,		// Mechanical Unit
	EU,		// Electrical Unit
	WU		// Wash Unit
};

// Service sequence type of a car
enum class SequenceType
{
	SS,		// Strict Sequence: services must be taken in order
	FS		// Flexible Sequence: any remaining service can be taken next
};

// ---------------- Phase 1 random simulator parameters ----------------
// (Phase 1 has no input file, so these values are fixed here.)
// They are enum constants (not variables) so there are no global variables.
enum SimConstants
{
	P1_NUM_CARS           = 1000,	// at least 1000 IDLE cars
	P1_N_DU               = 10,		// diagnosis bays
	P1_N_MU               = 15,		// mechanical bays
	P1_N_EU               = 8,		// electrical bays
	P1_N_WU               = 3,		// wash bays
	P1_WB_CAP             = 3,		// capacity of each wash bay

	P1_IDLE_PICK          = 50,	// 2.1  cars taken from IDLE each timestep
	P1_ERLY_TO_REDY       = 6,		// 2.2  cars moved ERLY -> REDY
	P1_LATE_CANCEL_PROB   = 10,	// 2.3  % chance a random LATE car cancels
	P1_LATE_TO_REDY       = 6,		// 2.4  cars moved LATE -> REDY
	P1_DIAG_TIME          = 3,		// 2.6  diagnosis takes 3 timesteps
	P1_DIAG_CANCEL_PROB   = 20,	// 2.7  % chance a car cancels after diagnosis
	P1_WAIT_PICK          = 6,		// 2.8  cars taken from WAIT lists
	P1_MB_PROB            = 60,	// 2.8  bay assignment probability: MB 60%
	P1_EB_PROB            = 30,	//      EB 30%  (WB gets the remaining 10%)
	P1_SERV_TIME          = 4,		// 2.9  any service takes 4 timesteps
	P1_FINISH_PROB        = 40,	// 2.10 % chance a car finishes after a service

	IDLE_PRINT_LIMIT      = 15,	// print ONLY the first 15 IDLE cars
};
