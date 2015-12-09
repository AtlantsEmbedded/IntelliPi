
#ifndef FEATURE_OUTPUT_H
#define FEATURE_OUTPUT_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define INIT_FEATURE_OUTPUT_FC(options) \
		_INIT_FEATURE_OUTPUT_FC(options)
		
#define WRITE_FEATURE_FC(feature_buf, output_interface) \
		_WRITE_FEATURE_FC(feature_buf, output_interface)
		
#define TERMINATE_FEATURE_OUTPUT_FC(output_interface) \
		_TERMINATE_FEATURE_OUTPUT_FC(output_interface)
		
typedef void* (*init_functionPtr_t) (void *);
typedef int (*wrt_functionPtr_t) (void *, void *);
typedef int (*clean_functionPtr_t) (void *);
init_functionPtr_t _INIT_FEATURE_OUTPUT_FC;
wrt_functionPtr_t _WRITE_FEATURE_FC;
clean_functionPtr_t _TERMINATE_FEATURE_OUTPUT_FC;



/*Structure containing the reference to all output interfaces*/
/*primarily use if you need to output to SHM and CSV at the same time*/
typedef struct output_interface_array_s {
	int nb_output;
	void** output_interface;
}output_interface_array_t;


void* init_feature_output(char output_type, void* options);


#endif
