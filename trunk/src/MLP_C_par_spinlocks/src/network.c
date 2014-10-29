 /**
 * @file Network.h
 * @author Frédéric Simard
 * @date October, 2014
 *
 * @brief This file implements the parallel processing solution for a multilayer perceptron (MLP).  
 * To get it to work, you need to run the following: 
 * *** training
 * - call to InitNetwork(), with all the properties set for your need.
 * - call to SetInput(), to define the state of the first layer.
 * - call to ComputeNetwork(), to execute the forward pass.
 * - call to BackPropError(), to execute one backward pass (you need to feed in the desired output)
 * - repeat
 *
 * *** test
 * - call to SetInput(), to define the state of the first layer.
 * - call to ComputeNetwork(), to execute the forward pass.
 * - call to GetOutput(), to get the state of the output layer.
 */

#define NB_CHECKPOINTS 5

#include <include/network.h>

/**
 * SigmoidFunction(float x)
 * 
 * @brief static inline non-linear function
 * @param x
 * @return float
 */
static inline float SigmoidFunction(float x)
{

	return 1 / (1 + exp(-x));

}

/**
 * InitNetwork(struct SNNetworkProp InitProp)
 * 
 * @brief Network "constructor", allocate memory and initialize variables
 * this function return a network ready to be trained
 * The pool of threads associated with this network are also
 * initiated at the end of this function
 * @param InitProp
 * @return SNNetwork
 */
struct SNNetwork *InitNetwork(struct SNNetworkProp InitProp)
{

	int i = 0;

	int j = 0;

	int TotNumberOfHiddenWeights = 0;

	int TotNumberOfOutputWeights = 0;

	/*allocate memory for the network */
	struct SNNetwork *pNNetwork =
	    (struct SNNetwork *)malloc(sizeof(struct SNNetwork));

	/*copy the properties */
	pNNetwork->OwnProp = InitProp;

	/*these two shouldn't have been initialized by the user */
	pNNetwork->OwnProp.TotNumberOfNeurons = 0;

	pNNetwork->OwnProp.TotNumberOfWeights = 0;

	/*allocate memory for all neurons */
	/*Compute the total number of units */
	for (i = 0; i < InitProp.NbOfLayer; i++) {

		pNNetwork->OwnProp.TotNumberOfNeurons +=
		    InitProp.NbOfNeurons[i];

	}

	/*allocate memory */
	pNNetwork->NeuronsActivity =
	    (float *)malloc(sizeof(float) *
			    pNNetwork->OwnProp.TotNumberOfNeurons);

	/*init the activity of all neurons */
	for (i = 0; i < pNNetwork->OwnProp.TotNumberOfNeurons; i++) {

		pNNetwork->NeuronsActivity[i] = 0.0;

	}

	/*allocate memory for weights */
	/*Compute the total number of weights */
	for (i = 1; i < pNNetwork->OwnProp.NbOfLayer; i++) {

		/*weights connecting neurons */
		pNNetwork->OwnProp.TotNumberOfWeights +=
		    InitProp.NbOfNeurons[i - 1] * InitProp.NbOfNeurons[i];

		/*weights connecting biases */
		pNNetwork->OwnProp.TotNumberOfWeights +=
		    InitProp.NbOfNeurons[i];

	}

	/*allocate memory for weights */
	pNNetwork->Weights =
	    (float *)malloc(sizeof(float) *
			    pNNetwork->OwnProp.TotNumberOfWeights);

	/*All weights and biases */
	TotNumberOfHiddenWeights =
	    InitProp.NbOfNeurons[Input] * InitProp.NbOfNeurons[Hidden] +
	    InitProp.NbOfNeurons[Hidden];

	TotNumberOfOutputWeights =
	    InitProp.NbOfNeurons[Hidden] * InitProp.NbOfNeurons[Output] +
	    InitProp.NbOfNeurons[Output];

	/*intialize the Hidden weights between -0.1 and 0.1 */
	for (i = 0; i < TotNumberOfHiddenWeights; i++) {

		pNNetwork->Weights[j++] =
		    ((((float)rand()) / (float)RAND_MAX) * (float)0.2) - 0.1;

	}

	/*intialize the output weights between -0.001 and 0.001 */
	for (i = 0; i < TotNumberOfOutputWeights; i++) {

		pNNetwork->Weights[j++] =
		    ((((float)rand()) / (float)RAND_MAX) * (float)0.002) - .001;

	}

	/*allocate the memory for the deltas, used in back propagation */
	/*output layer */
	pNNetwork->OutputDeltas =
	    (float *)malloc(sizeof(float) * InitProp.NbOfNeurons[Output]);

	for (i = 0; i < InitProp.NbOfNeurons[Output]; i++) {

		pNNetwork->OutputDeltas[i] = 0.0;

	}

	/*hidden layer */
	pNNetwork->HiddenDeltas =
	    (float *)malloc(sizeof(float) * InitProp.NbOfNeurons[Hidden]);

	for (i = 0; i < InitProp.NbOfNeurons[Output]; i++) {

		pNNetwork->OutputDeltas[i] = 0.0;

	}

	/*call for the initialization of parallel processing features */
	InitParProc(pNNetwork);

	return pNNetwork;

}

/**
 * KillNetwork(struct SNNetwork *pNNetwork)
 * 
 * @brief Clear the memory used by the network
 * @param pNNetwork
 */
void KillNetwork(struct SNNetwork *pNNetwork)
{

	/*clear the pool of threads */
	TerminateParProc(pNNetwork);

	/*network variables */
	free(pNNetwork->NeuronsActivity);

	free(pNNetwork->Weights);

	free(pNNetwork->HiddenDeltas);

	free(pNNetwork->OutputDeltas);

	free(pNNetwork);

}

/**
 * SetInput(struct SNNetwork *pNNetwork, float *InputActivity)
 * 
 * @brief Function allowing the presentation of an input feature vector to the network
 * it sets the states of the input neurons
 * @param pNNetwork
 * @param InputActivity
 */
void SetInput(struct SNNetwork *pNNetwork, float *InputActivity)
{

	int i = 0;

	/*copy the input vector to the value of the input neurons */
	for (i = 0; i < pNNetwork->OwnProp.NbOfNeurons[Input]; i++) {

		pNNetwork->NeuronsActivity[i] = InputActivity[i];

	}

}

/**
 * InitParProc(struct SNNetwork *pNNetwork)
 * @brief this function initialize the pool of threads and sync mechanisms
 * @param pNNetwork
 * @return 0 if sucess, non-zero if error.
 */
static int InitParProc(struct SNNetwork *pNNetwork)
{

	int ii = 0;

	pthread_attr_t attr;

	/*initialize the check points used for the synchronization */
	pNNetwork->MasterThreadParams.pCheckpoints =
	    init_checkpoints(NB_CHECKPOINTS);

	if (!pNNetwork->MasterThreadParams.pCheckpoints) {

		printf("could not init checkpoints");

		return -1;

	}

	/*init threads params */
	if (InitWorkerParams(pNNetwork) == -1) {

		printf("could not init parameters params");

		return -1;

	}

	printf("init thread");

#ifdef SCHEDULER_POLICY
	
	struct sched_param scheduler_params;
	
	/*change scheduling policy */
	/*and increase process priority */
	/*For this, the program needs to be launched in super user mode */
	sched_getparam(getpid(), &scheduler_params);

	scheduler_params.sched_priority = THREADS_SCHED_PRIORITY;

	if (sched_setscheduler(getpid(), SCHED_RR, &scheduler_params)) {

		int errsv = errno;

		switch (errsv) {

		case EINVAL:

		case ESRCH:

			break;

		case EPERM:

			printf
			    ("Super user mode required for high-speed execution\n");

			break;

		}

	}
#endif				/* 
				 */

	/*Init threads attribute */
	pthread_attr_init(&attr);

	/*set them as detached */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/*init the pool of threads */
	for (ii = 0; ii < NB_WORK_THREADS; ii++) {

		if (pthread_create
		    (&(pNNetwork->MasterThreadParams.WorkerThreads[ii]),
		     &attr,
		     &WorkerThread,
		     (void *)&(pNNetwork->
			       MasterThreadParams.WorkerThreadParams[ii +
								     WORK_THREADS_OFFSET])))
		{

			printf("Could not create thread %d\n", ii);

			return -1;

		}

	}

	pthread_attr_destroy(&attr);

	return 0;

}

/**
 * InitWorkerParams(struct SNNetwork *pNNetwork)
 * 
 * @brief this function initialize the parameters of the threads
 * Things that are done here:
 * - split the work among the thread
 * - generate memory access shortcut for the threads
 * - make copy of relevant information
 * @param pNNetwork
 * @return 0 if success
 */
int InitWorkerParams(struct SNNetwork *pNNetwork)
{

	int ii = 0;

	/*Divide the hidden neurons evenly, and take into account case for which the */
	/*division doesn't round down to 0 */
	int NbOfHidNeuronsPerThread =
	    pNNetwork->OwnProp.NbOfNeurons[Hidden] / (NB_WORK_THREADS +
						      NB_MASTER_THREADS);

	int ExtraHidNeurons =
	    pNNetwork->OwnProp.NbOfNeurons[Hidden] % (NB_WORK_THREADS +
						      NB_MASTER_THREADS);

	/*Same with output neurons */
	int NbOfOutNeuronsPerThread =
	    pNNetwork->OwnProp.NbOfNeurons[Output] / (NB_WORK_THREADS +
						      NB_MASTER_THREADS);

	int ExtraOutNeurons =
	    pNNetwork->OwnProp.NbOfNeurons[Output] % (NB_WORK_THREADS +
						      NB_MASTER_THREADS);

	/*compute memory offsets for hidden and output neurons */
	/*these will be used to give direct access to the threads */
	int HiddenOffset = pNNetwork->OwnProp.NbOfNeurons[Input];	/*first hidden neuron */

	int HiddenThreadOffset = 0;	/*offset from first, for each thread */

	int OutputOffset = pNNetwork->OwnProp.NbOfNeurons[Input] + pNNetwork->OwnProp.NbOfNeurons[Hidden];	/*first output neuron */

	int OutputThreadOffset = 0;	/*offset from first, for each thread */

	/*compute memory offsets for input to hidden and hidden to output weights */
	int HiddenWeightsThreadOffset = 0;

	int OutputWeightsThreadOffset = pNNetwork->OwnProp.NbOfNeurons[Hidden] *
	    /*nb_hidden times nb_inputs */
	    (pNNetwork->OwnProp.NbOfNeurons[Input] + 1);	/*plus the bias */

#if VERBOSITY
	/*show it for debbuggin */
	printf("HiddenOffset: %d\n", HiddenOffset);

	printf("OutputOffset: %d\n", OutputOffset);

	printf("OutputWeightsThreadOffset: %d\n", OutputWeightsThreadOffset);

#endif

	/*Define the workload for the master (ii==1) and worker (ii>1) threads */
	for (ii = 0; ii < (NB_WORK_THREADS + NB_MASTER_THREADS); ii++) {

		/*initialize variables value */
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].IsAlive =
		    1;

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].TypeOfJobToDo = Idle;

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].pCheckpoints =
		    pNNetwork->MasterThreadParams.pCheckpoints;

		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].OwnProp =
		    pNNetwork->OwnProp;

		/*distribute the number of neurons required */
		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].NbNeuronsToCompute[Input] = 0;

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].NbNeuronsToCompute[Hidden] =
		    NbOfHidNeuronsPerThread;

		/*allocate any extra neurons if needed */
		if (ExtraHidNeurons) {

			pNNetwork->MasterThreadParams.
			    WorkerThreadParams[ii].NbNeuronsToCompute[Hidden]++;

			ExtraHidNeurons--;

		}

		/*distribute the number of neurons required */
		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].NbNeuronsToCompute[Output] =
		    NbOfOutNeuronsPerThread;

		/*allocate any extra neurons if needed */
		if (ExtraHidNeurons) {

			pNNetwork->MasterThreadParams.
			    WorkerThreadParams[ii].NbNeuronsToCompute[Output]++;

			ExtraOutNeurons--;

		}
#if VERBOSITY
		/*report the amount of work for each worker */
		printf("Thread: %d, NbHidden: %d\n", ii,
		       pNNetwork->MasterThreadParams.
		       WorkerThreadParams[ii].NbNeuronsToCompute[Hidden]);

		printf("Thread: %d, NbOutput: %d\n", ii,
		       pNNetwork->MasterThreadParams.
		       WorkerThreadParams[ii].NbNeuronsToCompute[Output]);

#endif

		/*Copy indented activity vectors for direct acccess */
		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].AllInputActivity =
		    pNNetwork->NeuronsActivity;

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].AllHiddenActivity =
		    &(pNNetwork->NeuronsActivity[HiddenOffset]);

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].AllOutputActivity =
		    &(pNNetwork->NeuronsActivity[OutputOffset]);

		/*define the neurons vectors for each thread, also for direct access */
		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].ToComputeHiddenActivity =
		    &(pNNetwork->NeuronsActivity
		      [HiddenOffset + HiddenThreadOffset]);

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].ToComputeOutputActivity =
		    &(pNNetwork->NeuronsActivity
		      [OutputOffset + OutputThreadOffset]);

		/*update the neurons vectors offset for the next thread */
		HiddenThreadOffset +=
		    pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].NbNeuronsToCompute[Hidden];

		OutputThreadOffset +=
		    pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].NbNeuronsToCompute[Output];

		/*define the a weight vectors starting at the given location for each thread */
		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].HiddenWeights =
		    &(pNNetwork->Weights[HiddenWeightsThreadOffset]);

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].OutputWeights =
		    &(pNNetwork->Weights[OutputWeightsThreadOffset]);

		/*update the offset of the weights for the next thread */
		HiddenWeightsThreadOffset +=
		    pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].NbNeuronsToCompute[Hidden] *
		    (pNNetwork->OwnProp.NbOfNeurons[Input] + 1);

		OutputWeightsThreadOffset +=
		    pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].NbNeuronsToCompute[Output] *
		    (pNNetwork->OwnProp.NbOfNeurons[Hidden] + 1);

	}

	return 0;

}

/**
 * TerminateParProc(struct SNNetwork *pNNetwork)
 * 
 * @brief Terminates par proc
 * @param pNNetwork
 */
void TerminateParProc(struct SNNetwork *pNNetwork)
{

	int ii = 0;

	/*Set threads to not alive and post an idle job */
	for (ii = 0; ii < (NB_WORK_THREADS + NB_MASTER_THREADS); ii++) {

		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].IsAlive =
		    0;

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].TypeOfJobToDo = Idle;

	}

	open_checkpoint(pNNetwork->MasterThreadParams.pCheckpoints,
			check_start_job, NB_WORK_THREADS, 0);

}

/**
 * WorkerThread(void *params)
 * @brief worker thread
 * @param params
 */
static void *WorkerThread(void *params)
{

	int i = 0;

	int j = 0;

	float InputSum = 0;

	/*make local copy to speedup a bit and simplify the code */
	struct SWorkerThreadInitParams *ThreadParams =
	    (struct SWorkerThreadInitParams *)params;

	struct SNNetworkProp OwnProp = ThreadParams->OwnProp;

	float *AllInputActivity = ThreadParams->AllInputActivity;

	float *AllHiddenActivity = ThreadParams->AllHiddenActivity;

	float *AllOutputActivity = ThreadParams->AllOutputActivity;

	float *ToComputeHiddenActivity = ThreadParams->ToComputeHiddenActivity;

	float *ToComputeOutputActivity = ThreadParams->ToComputeOutputActivity;

	float *HiddenWeights = ThreadParams->HiddenWeights;

	float *OutputWeights = ThreadParams->OutputWeights;

	int WeightPointer = 0;

	int NeuronPointer = 0;

	char tmp_done_waiting = 0;

	pthread_detach(pthread_self());

	/*While alive */
	while (ThreadParams->IsAlive) {

//~ #if 1 
		wait_on_checkpoint(ThreadParams->pCheckpoints,
				   check_start_job, 1);

		/*Check for job to do */
		switch (ThreadParams->TypeOfJobToDo) {

			/*The first type of job is the forward pass */
		case ForwardProp:

			/*Compute hidden layer */
			WeightPointer = 0;

			NeuronPointer = 0;

			/*For all hidden unit allocated to this thread */
			for (i = 0;
			     i < ThreadParams->NbNeuronsToCompute[Hidden]; i++)
			{

				/*sum inputs from input layer */
				InputSum = 0;

				for (j = 0; j < OwnProp.NbOfNeurons[Input]; j++) {

					InputSum +=
					    HiddenWeights[WeightPointer++] *
					    AllInputActivity[j];

				}

				/*plus the connection to the bias */
				InputSum +=
				    HiddenWeights[WeightPointer++] *
				    OwnProp.HiddenLayerBias;

				/*sigmoid non-nonlinearity */
				ToComputeHiddenActivity[NeuronPointer++] =
				    SigmoidFunction(InputSum);

			}

			/*Resync */
			open_checkpoint(ThreadParams->pCheckpoints,
					check_done_one, 1, 1);

			wait_on_checkpoint(ThreadParams->pCheckpoints,
					   check_go_two, 1);

			/*reset offset counter */
			WeightPointer = 0;

			NeuronPointer = 0;

			/*Compute output layer */
			/*For all output layer */
			for (i = 0;
			     i < ThreadParams->NbNeuronsToCompute[Output]; i++)
			{

				/*sum all inputs */
				InputSum = 0;

				for (j = 0; j < OwnProp.NbOfNeurons[Hidden];
				     j++) {

					InputSum +=
					    OutputWeights[WeightPointer++] *
					    AllHiddenActivity[j];

				}

				InputSum +=
				    OutputWeights[WeightPointer++] *
				    OwnProp.OutputLayerBias;

				/*sigmoid non-nonlinearity */
				ToComputeOutputActivity[NeuronPointer++] =
				    SigmoidFunction(InputSum);

			}

			open_checkpoint(ThreadParams->pCheckpoints,
					check_done_two, 1, 1);

			//wait_on_checkpoint(ThreadParams->pCheckpoints, check_finish_job, 1);

			break;

//~ #if 0
			//~ /*The second type of job is the backpropagation of error */
			//~ case BackProp:
//~ 
			//~ /*Compute deltas output layer */
			//~ /*Barrier */
			//~ pthread_barrier_wait(&
			//~ (ThreadParams->pSyncStuff->
			//~ AllInSync_barrier));
//~ 
			//~ /*Compute deltas hidden layer */
			//~ /*Barrier */
			//~ pthread_barrier_wait(&
			//~ (ThreadParams->pSyncStuff->
			//~ AllInSync_barrier));
//~ 
			//~ /*adjust weights output Layer */
			//~ /*Barrier */
			//~ pthread_barrier_wait(&
			//~ (ThreadParams->pSyncStuff->
			//~ AllInSync_barrier));
//~ 
			//~ break;

//~ #endif                              /* 
	
		case Idle:

			break;

		}

//~ #endif				/* 
				 //~ */
	}

}

/**
 * ComputeNetwork(struct SNNetwork *pNNetwork)
 * 
 * @brief Feedforward propogation of activation. The activity of the input layer
 * should already have been set using SetInput()
 * 
 * This function will compute the activity of all cells down *output layer
 * @param pNNetwork
 */
void ComputeNetwork(struct SNNetwork *pNNetwork)
{

	int i, j = 0;

	int WeightPointer = 0;

	int NeuronPointer = 0;

	int HiddenLayerOffset = 0;

	int OutputLayerOffset = 0;

	float InputSum;

	/*Compute activation for hidden layer */
	/*Offset over the Input Layer */
	NeuronPointer = pNNetwork->OwnProp.NbOfNeurons[Input];

	float *NeuronsActivity = pNNetwork->NeuronsActivity;

	float *Weights = pNNetwork->Weights;

	struct SNNetworkProp OwnProp = pNNetwork->OwnProp;

#if 1
/*That's the parallel processing solution*/

	int ii = 0;

	struct SWorkerThreadInitParams ThreadParams =
	    pNNetwork->MasterThreadParams.WorkerThreadParams[0];

	/*Post a forward pass job */
	for (ii = 0; ii < (NB_WORK_THREADS + NB_MASTER_THREADS); ii++) {

		pNNetwork->MasterThreadParams.
		    WorkerThreadParams[ii].TypeOfJobToDo = ForwardProp;

	}

	/*start the job */
	open_checkpoint(pNNetwork->MasterThreadParams.pCheckpoints,
			check_start_job, NB_WORK_THREADS, 0);

	/*For all hidden unit allocated to this thread */
	for (i = 0; i < ThreadParams.NbNeuronsToCompute[Hidden]; i++) {

		/*sum inputs from input layer */
		InputSum = 0;

		for (j = 0; j < pNNetwork->OwnProp.NbOfNeurons[Input]; j++) {

			InputSum +=
			    ThreadParams.HiddenWeights[WeightPointer++] *
			    ThreadParams.AllInputActivity[j];

		}

		/*plus the connection to the bias */
		InputSum +=
		    ThreadParams.HiddenWeights[WeightPointer++] *
		    pNNetwork->OwnProp.HiddenLayerBias;

		/*sigmoid non-nonlinearity */
		ThreadParams.ToComputeHiddenActivity[NeuronPointer++] =
		    SigmoidFunction(InputSum);

	}

	/*wait till first layer is finished */
	wait_on_checkpoint(pNNetwork->MasterThreadParams.pCheckpoints,
			   check_done_one, NB_WORK_THREADS);

	open_checkpoint(pNNetwork->MasterThreadParams.pCheckpoints,
			check_go_two, NB_WORK_THREADS, 0);

	/*reset offset counter */
	WeightPointer = 0;

	NeuronPointer = 0;

	/*Compute output layer */
	/*For all output layer */
	for (i = 0; i < ThreadParams.NbNeuronsToCompute[Output]; i++) {

		/*sum all inputs */
		InputSum = 0;

		for (j = 0; j < pNNetwork->OwnProp.NbOfNeurons[Hidden]; j++) {

			InputSum +=
			    ThreadParams.OutputWeights[WeightPointer++] *
			    ThreadParams.AllHiddenActivity[j];

		}

		InputSum +=
		    ThreadParams.OutputWeights[WeightPointer++] *
		    pNNetwork->OwnProp.OutputLayerBias;

		/*sigmoid non-nonlinearity */
		ThreadParams.ToComputeOutputActivity[NeuronPointer++] =
		    SigmoidFunction(InputSum);

	}

	wait_on_checkpoint(pNNetwork->MasterThreadParams.pCheckpoints,
			   check_done_two, NB_WORK_THREADS);

	//open_checkpoint(pNNetwork->MasterThreadParams.pCheckpoints, check_finish_job, NB_WORK_THREADS);

#else
//~ /*That's the sequential solution*/
	//~ /*For all hidden units */
	//~ for (i = 0; i < OwnProp.NbOfNeurons[Hidden]; i++) {
//~ 
	//~ /*sum inputs from input layer */
	//~ InputSum = 0;
//~ 
	//~ for (j = 0; j < OwnProp.NbOfNeurons[Input]; j++) {
//~ 
	//~ InputSum +=
	//~ Weights[WeightPointer++] * NeuronsActivity[j];
//~ 
	//~ }
//~ 
	//~ /*plus the connection to the bias */
	//~ InputSum += Weights[WeightPointer++] * OwnProp.HiddenLayerBias;
//~ 
	//~ /*sigmoid non-nonlinearity */
	//~ NeuronsActivity[NeuronPointer++] = SigmoidFunction(InputSum);
//~ 
	//~ }
//~ 
	//~ /*Compute Activation for output layer */
	//~ /*precompute the limits of sources (in hidden layer) */
	//~ HiddenLayerOffset = OwnProp.NbOfNeurons[Input];
//~ 
	//~ OutputLayerOffset =
	//~ OwnProp.NbOfNeurons[Input] + OwnProp.NbOfNeurons[Hidden];
//~ 
	//~ /*readjust the pointers */
	//~ NeuronPointer = OutputLayerOffset;
//~ 
	//~ WeightPointer =
	//~ (OwnProp.NbOfNeurons[Input] + 1) * OwnProp.NbOfNeurons[Hidden];
//~ 
	//~ /*For all output layer */
	//~ for (i = 0; i < OwnProp.NbOfNeurons[Output]; i++) {
//~ 
	//~ /*sum all inputs */
	//~ InputSum = 0;
//~ 
	//~ for (j = HiddenLayerOffset; j < OutputLayerOffset; j++) {
//~ 
	//~ InputSum +=
	//~ Weights[WeightPointer++] * NeuronsActivity[j];
//~ 
	//~ }
//~ 
	//~ InputSum += Weights[WeightPointer++] * OwnProp.OutputLayerBias;
//~ 
	//~ /*sigmoid non-nonlinearity */
	//~ NeuronsActivity[NeuronPointer++] = SigmoidFunction(InputSum);
//~ 
	//~ }

#endif				/* 
				 */

}

/**
 * BackPropError(struct SNNetwork* pNNetwork, float* DesiredOutput)
 * 
 * @brief Back propagation of error algorithm:
 * We start by computing the deltas of the output and hidden layers. 
 * After we compute the new weights
 * @param pNNetwork
 * @param DesiredOutput
 */
void BackPropError(struct SNNetwork *pNNetwork, float *DesiredOutput)
{

	int i, j = 0;

	float SumOfWeightedOutputs = 0.0;

	/*Computes the offsets */
	int HiddenActOffset = pNNetwork->OwnProp.NbOfNeurons[0];

	int OutputLayerOffset =
	    pNNetwork->OwnProp.NbOfNeurons[Input] +
	    pNNetwork->OwnProp.NbOfNeurons[Hidden];

	/*Get Local pointers to reduce indirect memory accesses */
	float *InputLayer = &(pNNetwork->NeuronsActivity[0]);

	float *HiddenLayer = &(pNNetwork->NeuronsActivity[HiddenActOffset]);

	float *OutputLayer = &(pNNetwork->NeuronsActivity[OutputLayerOffset]);

	float *Weights = &(pNNetwork->Weights[0]);

	float *OutputDeltas = &(pNNetwork->OutputDeltas[0]);

	float *HiddenDeltas = &(pNNetwork->HiddenDeltas[0]);

	struct SNNetworkProp OwnProp = pNNetwork->OwnProp;

	/*Compute the deltas */
	/*Compute the deltas for the output layer */
	/*Delta for output = F'(Yo)*(To-Yo) */
	/*Do = Gain*(To-Yo) */
	/*To = Target(or desired) output */
	/*Yo = actual output */
	for (i = 0; i < OwnProp.NbOfNeurons[Output]; i++) {

		OutputDeltas[i] = (DesiredOutput[i] - OutputLayer[i]);

	}

	/*Compute the deltas for the hidden layer */
	/*precompute the offsets */
	int HidToOutWeightOffset =
	    OwnProp.NbOfNeurons[Hidden] * OwnProp.NbOfNeurons[Input] +
	    OwnProp.NbOfNeurons[Hidden];

	float *HidToOutLayerWeights = &(Weights[HidToOutWeightOffset]);

	int JumpOffset = OwnProp.NbOfNeurons[Hidden] + 1;

	/*Delta = F'(Yh)*SumForAllOutput(Do*Who) */
	/*Delta = F'(Yh)*E(Do*Who) */
	/*Delta = Yh*(1-Yh)*E(Do*Who) */
	for (i = 0; i < OwnProp.NbOfNeurons[Hidden]; i++) {

		SumOfWeightedOutputs = 0.0;

		for (j = 0; j < OwnProp.NbOfNeurons[Output]; j++) {

			SumOfWeightedOutputs +=
			    OutputDeltas[j] * HidToOutLayerWeights[i +
								   j *
								   JumpOffset];

		}

		HiddenDeltas[i] =
		    HiddenLayer[i] * (1 -
				      HiddenLayer[i]) * SumOfWeightedOutputs;

	}

	/*Compute the new weights */
	/*We proceed backward, (usually it goes from output to hidden) */
	/*but we precomputed the deltas, so we can go from input to output */
	/*this is slightly more consistent with the cache memory and easier on computation */
	int WeightOffset = 0;

	/*weights between the input and hidden layer */
	/*Wih(t+1) = Wih(t)+N*Dh*Yi */
	for (i = 0; i < OwnProp.NbOfNeurons[Hidden]; i++) {

		for (j = 0; j < OwnProp.NbOfNeurons[Input]; j++) {

			Weights[WeightOffset++] +=
			    OwnProp.LearningRate * HiddenDeltas[i] *
			    InputLayer[j];

		}

		Weights[WeightOffset++] +=
		    OwnProp.LearningRate * HiddenDeltas[i] *
		    OwnProp.HiddenLayerBias;

	}

	/*weights between the hidden and output layer */
	/*Who(t+1) = Who(t)+N*Do*Yh */
	for (i = 0; i < OwnProp.NbOfNeurons[Output]; i++) {

		for (j = 0; j < OwnProp.NbOfNeurons[Hidden]; j++) {

			Weights[WeightOffset++] +=
			    OwnProp.LearningRate * OutputDeltas[i] *
			    HiddenLayer[j];

		}

		Weights[WeightOffset++] +=
		    OwnProp.LearningRate * OutputDeltas[i] *
		    OwnProp.OutputLayerBias;

	}

}

/**
 * GetOutput(struct SNNetwork* pNNetwork, float* OutputActivity)
 * 
 * @brief Copy the output layer into the memory area provided by the user
 * @param pNNetwork
 * @param OutputActivity
 */
void GetOutput(struct SNNetwork *pNNetwork, float *OutputActivity)
{

	int i = 0;

	int OutputOffset =
	    pNNetwork->OwnProp.TotNumberOfNeurons -
	    pNNetwork->OwnProp.NbOfNeurons[Output];

	float *OutputLayer = &(pNNetwork->NeuronsActivity[OutputOffset]);

	for (i = 0; i < pNNetwork->OwnProp.NbOfNeurons[Output]; i++) {

		OutputActivity[i] = OutputLayer[i];

	}

}
