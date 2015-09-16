#ifndef SAMPLE_PROPS_H
#define SAMPLE_PROPS_H

/**
 * @file sample_props.c
 * @author Frédéric Simard
 * @date December, 2014
 * @brief This file defines the properties of the recorded samples.
 *		  It is used to defines the settings of the wrapper and the data processing stages.
 *       
 * 		  As of December, 2014, it only contains preliminary definition. It should be updated as
 *		  the program is developed.
 */



#define NATIV_SAMPLING_RATE 220 /*220 Hz*/
#define NATIV_RECORD_LENGTH_SEC 5 /*5 secondes*/

#define NATIV_NB_SAMPLES NATIV_SAMPLING_RATE*NATIV_RECORD_LENGTH_SEC /*220*5 = 1100 samples/features*/


#define DOWNSAMP_SAMPLING_RATE NATIV_SAMPLING_RATE/2 /*220 Hz*/
#define DOWNSAMP_NB_SAMPLES NATIV_NB_SAMPLES/2;











#endif 









