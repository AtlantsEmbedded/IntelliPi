/**
 * @file main.h
 * @brief Header for the main file
 * 
 */
#ifndef	TRUE
#define	TRUE	(1==1)
#define	FALSE	(1==2)
#endif

// Defines for the Adafruit Pi LCD interface board

#define	AF_BASE		100
#define	AF_RED		(AF_BASE + 6)
#define	AF_GREEN	(AF_BASE + 7)
#define	AF_BLUE		(AF_BASE + 8)

#define	AF_E		(AF_BASE + 13)
#define	AF_RW		(AF_BASE + 14)
#define	AF_RS		(AF_BASE + 15)

#define	AF_DB4		(AF_BASE + 12)
#define	AF_DB5		(AF_BASE + 11)
#define	AF_DB6		(AF_BASE + 10)
#define	AF_DB7		(AF_BASE +  9)

#define	AF_SELECT	(AF_BASE +  0)
#define	AF_RIGHT	(AF_BASE +  1)
#define	AF_DOWN		(AF_BASE +  2)
#define	AF_UP		(AF_BASE +  3)
#define	AF_LEFT		(AF_BASE +  4)

typedef struct {
	int waitForRelease;
	int colour;
} button_s;
