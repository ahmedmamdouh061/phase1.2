#pragma once

enum class CarStatus
{
	IDLE,
	ERLY,
	LATE,
	REDY,
	DIAG,
	WAIT,
	SERV,
	CNCL,
	FNSH
};

enum class UnitType
{
	DU,
	MU,
	EU,
	WU
};

enum class SequenceType
{
	SS,
	FS
};

enum SimConstants
{
	P1_NUM_CARS           = 1000,
	P1_N_DU               = 10,
	P1_N_MU               = 15,
	P1_N_EU               = 8,
	P1_N_WU               = 3,
	P1_WB_CAP             = 3,

	P1_IDLE_PICK          = 50,
	P1_ERLY_TO_REDY       = 6,
	P1_LATE_CANCEL_PROB   = 10,
	P1_LATE_TO_REDY       = 6,
	P1_DIAG_TIME          = 3,
	P1_DIAG_CANCEL_PROB   = 20,
	P1_WAIT_PICK          = 6,
	P1_MB_PROB            = 60,
	P1_EB_PROB            = 30,
	P1_SERV_TIME          = 4,
	P1_FINISH_PROB        = 40,

	IDLE_PRINT_LIMIT      = 15,
};
