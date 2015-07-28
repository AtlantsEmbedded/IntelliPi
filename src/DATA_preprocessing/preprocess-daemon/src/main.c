/**
 * @file main.c
 * @author Frederic Simard (frederic.simard.1@outlook.com) | Atlants Embedded
 * @brief 
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "app_signal.h"
#include "data_input.h"
#include "xml.h"

/**
 * main()
 * @brief Application main running loop
 */
int main(int argc __attribute__ ((unused)), char **argv __attribute__ ((unused)))
{
	
	int i;
	
	/*Set up ctrl c signal handler*/
	(void)signal(SIGINT, ctrl_c_handler);
	
	/*Initialize data input*/
	init_data_input(SHM_INPUT);
	
	
	/*Begin data processing*/
	/*read the first 10 pages then quit*/
	for(;;)
	{
		READ_DATA(NULL);
	}

	TERMINATE_DATA_INPUT_FC();


	return 0;
}
