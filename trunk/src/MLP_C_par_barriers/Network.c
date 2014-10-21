//
//
// Network.h
//
// This file implements the parallel processing solution for a multilayer perceptron (MLP).  
// To get it to work, you need to run the following: 
// *** training
// - call to InitNetwork(), with all the properties set for your need.
// - call to SetInput(), to define the state of the first layer.
// - call to ComputeNetwork(), to execute the forward pass.
// - call to BackPropError(), to execute one backward pass (you need to feed in the desired output)
// - repeat
//
// *** test
// - call to SetInput(), to define the state of the first layer.
// - call to ComputeNetwork(), to execute the forward pass.
// - call to GetOutput(), to get the state of the output layer.
//
//
// Frédéric Simard, 2014
//


#include "Network.h"



/*Defines the configuration of a worker threads*/
enum TypeOfJobs
{
	ForwardProp, /*forward pass*/
	BackProp, /*backward pass*/
	Idle /*do nothing*/
};


/*This struct is known to all thread and is used to access*/
/*synchronization mechanisms*/
struct SSyncStuff
{
	sem_t WorkerGo_sem; /*the semaphore giving the go signal*/
	pthread_barrier_t AllInSync_barrier; /*synchronization barrier*/
};

/*Initialization parameters for worker threads*/
struct SWorkerThreadInitParams
{

	struct SNNetworkProp OwnProp; /*a copy of the network properties*/
	
	char IsAlive; /*state variable for */
	int TypeOfJobToDo; /*current thread config*/

	int NbNeuronsToCompute[3];
	
	float* AllInputActivity; /*direct access to input neuron activations (for reading)*/
	float* AllHiddenActivity; /*direct access to hidden neuron activations (for reading)*/
	float* AllOutputActivity; /*direct access to output neuron activations (for reading)*/
	
	float* ToComputeHiddenActivity; /*direct access to hidden neuron activations (for writing)*/
	float* ToComputeOutputActivity; /*direct access to output neuron activations (for writing)*/

	float* HiddenWeights; /*direct access to input-hidden weights*/
	float* OutputWeights; /*direct access to hidden-output weights*/

	float* HiddenDeltas; /*direct access to hidden layer deltas*/
	float* OutputDeltas; /*direct access to output layer deltas*/
	
	struct SSyncStuff* pSyncStuff; /*pointer to sync mechanism*/
};


/*Parameters for main (or master) thread*/
struct SMasterThreadParams
{
	pthread_t WorkerThreads[NB_WORK_THREADS]; /*thread handles*/
	struct SWorkerThreadInitParams WorkerThreadParams[NB_WORK_THREADS]; /*thread params handles*/
	
	struct SSyncStuff SyncStuff; /*sync variables instance*/
};

/*the neural network*/
struct SNNetwork
{
	struct SNNetworkProp OwnProp; /*properties of the network*/
	float* Weights; 			  /*all weights in a vector*/
	float* NeuronsActivity;		  /*neurons activation*/

	float* OutputDeltas; 		  /*deltas used during the backprop*/
	float* HiddenDeltas;		  /*deltas used during the backprop*/
	
	struct SMasterThreadParams MasterThreadParams; /*a copy of the params for the main thread*/
};

/*ticket "fast" barrier*/
ticket_barrier_t MyTicketBarrier;

/*declaration of the sigmoid function, used as non-linearity*/
float SigmoidFunction(float x);

/*function associated with worker threads*/
void* WorkerThread(void * params);

/*initialization of parallel processing features*/
int InitParProc(struct SNNetwork* pNNetwork);
/*termination of parallel processing features*/
void TerminateParProc(struct SNNetwork* pNNetwork);


/*network "constructor", allocate memory and initialize variables*/
/*this function return a network ready to be trained*/
/*The pool of threads associated with this network are also*/
/*initiated at the end of this function*/
struct SNNetwork* InitNetwork(struct SNNetworkProp InitProp)
{
	int i=0;
	int j=0;
	int TotNumberOfHiddenWeights = 0;
	int TotNumberOfOutputWeights = 0;
	/*allocate memory for the network*/
	struct SNNetwork* pNNetwork = (struct SNNetwork*)malloc(sizeof(struct SNNetwork));
	/*copy the properties*/
	pNNetwork->OwnProp = InitProp;

	/*these two shouldn't have been initialized by the user*/
	pNNetwork->OwnProp.TotNumberOfNeurons = 0;
	pNNetwork->OwnProp.TotNumberOfWeights = 0;

	/*allocate memory for all neurons*/
	/*Compute the total number of units*/
	for(i=0;i<InitProp.NbOfLayer;i++)
	{
		pNNetwork->OwnProp.TotNumberOfNeurons += InitProp.NbOfNeurons[i];
	}

	/*allocate memory*/
	pNNetwork->NeuronsActivity = (float*)malloc(sizeof(float)*pNNetwork->OwnProp.TotNumberOfNeurons);

	/*init the activity of all neurons*/
	for(i=0;i<pNNetwork->OwnProp.TotNumberOfNeurons;i++)
	{
		pNNetwork->NeuronsActivity[i] = 0.0;
	}
	
	/*allocate memory for weights*/
	/*Compute the total number of weights*/
	for(i=1;i<pNNetwork->OwnProp.NbOfLayer;i++)
	{
	    /*weights connecting neurons*/
		pNNetwork->OwnProp.TotNumberOfWeights += InitProp.NbOfNeurons[i-1]*InitProp.NbOfNeurons[i];
		/*weights connecting biases*/
		pNNetwork->OwnProp.TotNumberOfWeights += InitProp.NbOfNeurons[i];
	}

	/*allocate memory for weights*/
	pNNetwork->Weights = (float*)malloc(sizeof(float)*pNNetwork->OwnProp.TotNumberOfWeights);

	/*All weights and biases*/
	TotNumberOfHiddenWeights = InitProp.NbOfNeurons[Input]*InitProp.NbOfNeurons[Hidden]+InitProp.NbOfNeurons[Hidden];
	TotNumberOfOutputWeights = InitProp.NbOfNeurons[Hidden]*InitProp.NbOfNeurons[Output]+InitProp.NbOfNeurons[Output];

	/*intialize the Hidden weights between -0.1 and 0.1*/
	for(i=0;i<TotNumberOfHiddenWeights;i++)
	{
		pNNetwork->Weights[j++] = ((((float)rand())/(float)RAND_MAX)*(float)0.2)-0.1;
	}

	/*intialize the output weights between -0.001 and 0.001*/
	for(i=0;i<TotNumberOfOutputWeights;i++)
	{
		pNNetwork->Weights[j++] = ((((float)rand())/(float)RAND_MAX)*(float)0.002)-.001;
	}

	/*allocate the memory for the deltas, used in back propagation*/
	/*output layer*/
	pNNetwork->OutputDeltas = (float*)malloc(sizeof(float)*InitProp.NbOfNeurons[Output]);

	for(i=0;i<InitProp.NbOfNeurons[Output];i++)
	{
		pNNetwork->OutputDeltas[i] = 0.0;
	}
	
	/*hidden layer*/
	pNNetwork->HiddenDeltas = (float*)malloc(sizeof(float)*InitProp.NbOfNeurons[Hidden]);

	for(i=0;i<InitProp.NbOfNeurons[Output];i++)
	{
		pNNetwork->OutputDeltas[i] = 0.0;
	}
	
	/*call for the initialization of parallel processing features*/
	InitParProc(pNNetwork);
	
	return pNNetwork;
}

/*Clear the memory used by the network*/
void KillNetwork(struct SNNetwork* pNNetwork)
{
	/*clear the pool of threads*/
	TerminateParProc(pNNetwork);

	/*network variables*/
	free(pNNetwork->NeuronsActivity);
	free(pNNetwork->Weights);
	free(pNNetwork->HiddenDeltas);
	free(pNNetwork->OutputDeltas);
	free(pNNetwork);
}

/*Function allowing the presentation of an input feature vector to the network*/
/*it sets the states of the input neurons*/
void SetInput(struct SNNetwork* pNNetwork, float* InputActivity)
{
	int i=0;
	/*copy the input vector to the value of the input neurons*/
	for(i=0;i<pNNetwork->OwnProp.NbOfNeurons[Input];i++)
	{
		pNNetwork->NeuronsActivity[i] = InputActivity[i];
	}
}


/*this function initialize the pool of threads and sync mechanisms*/
int InitParProc(struct SNNetwork* pNNetwork)
{
	int ii=0;

	//test only
	//ticket_barrier_init(&MyTicketBarrier, 5);
	//pthread_mutex_init(&mutex_arr, NULL);
	//pthread_cond_init(&cond,NULL);
	
	/*init semaphore used to launch a job*/
	if(sem_init(&(pNNetwork->MasterThreadParams.SyncStuff.WorkerGo_sem), PTHREAD_PROCESS_PRIVATE,0))
	{
		printf("could not init semaphore");
        return -1;
	}
	
	/*init the barrier used to sync the threads*/
	if(pthread_barrier_init(&(pNNetwork->MasterThreadParams.SyncStuff.AllInSync_barrier), NULL, NB_WORK_THREADS+1))
	{
		printf("could not create barrier");
        return -1;
	}
	
	/*init threads params*/
	if(InitWorkerParams(pNNetwork)==-1)
	{
		printf("could not init parameters params");
        return -1;
	}
	
	/*init the threads themselves*/
	for(ii = 0; ii < NB_WORK_THREADS; ii++)
    {
    	
        if(pthread_create(&(pNNetwork->MasterThreadParams.WorkerThreads[ii]), 
        				  NULL, 
        				  &WorkerThread, 
        				  (void*)&(pNNetwork->MasterThreadParams.WorkerThreadParams[ii])))
        {
            printf("Could not create thread %d\n", ii);
            return -1;
        }
    }
    
	return 0;
}

/*this function initialize the parameters of the threads*/
/*Things that are done here:*/
/*- split the work among the thread*/
/*- generate memory access shortcut for the threads*/
/*- make copy of relevant information*/
int InitWorkerParams(struct SNNetwork* pNNetwork)
{
	int ii=0;
	
	/*Divide the hidden neurons evenly, and take into account case for which the*/
	/*division doesn't round down to 0*/
	int NbOfHidNeuronsPerThread = pNNetwork->OwnProp.NbOfNeurons[Hidden]/NB_WORK_THREADS;
	int ExtraHidNeurons = pNNetwork->OwnProp.NbOfNeurons[Hidden]%NB_WORK_THREADS;
	
	/*Same with output neurons*/
	int NbOfOutNeuronsPerThread = pNNetwork->OwnProp.NbOfNeurons[Output]/NB_WORK_THREADS;
	int ExtraOutNeurons = pNNetwork->OwnProp.NbOfNeurons[Output]%NB_WORK_THREADS;
	
	/*compute memory offsets for hidden and output neurons*/
	/*these will be used to give direct access to the threads*/
	int HiddenOffset = pNNetwork->OwnProp.NbOfNeurons[Input]; /*first hidden neuron*/
	int HiddenThreadOffset = 0; /*offset from first, for each thread*/
	int OutputOffset = pNNetwork->OwnProp.NbOfNeurons[Input]+pNNetwork->OwnProp.NbOfNeurons[Hidden]; /*first output neuron*/
	int OutputThreadOffset = 0; /*offset from first, for each thread*/
	
	/*compute memory offsets for input to hidden and hidden to output weights*/
	int HiddenWeightsThreadOffset = 0;
	int OutputWeightsThreadOffset = pNNetwork->OwnProp.NbOfNeurons[Hidden]* /*nb_hidden times nb_inputs*/
									(pNNetwork->OwnProp.NbOfNeurons[Input]+1); /*plus the bias*/
	
#if VERBOSITY>0
	/*show it for debbuggin*/
	printf("HiddenOffset: %d\n",HiddenOffset);
	printf("OutputOffset: %d\n",OutputOffset);
	printf("OutputWeightsThreadOffset: %d\n",OutputWeightsThreadOffset);
#endif

	/*for each thread*/
	for(ii = 0; ii < NB_WORK_THREADS; ii++)
	{
		/*initialize variables value*/
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].IsAlive = 1;
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].TypeOfJobToDo = Idle;
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].pSyncStuff = &pNNetwork->MasterThreadParams.SyncStuff;
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].OwnProp = pNNetwork->OwnProp;
		
		/*distribute the number of neurons required*/
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Input] = 0;
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Hidden] = NbOfHidNeuronsPerThread;
		
		/*allocate any extra neurons if needed*/
		if(ExtraHidNeurons)
		{
			pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Hidden]++;
			ExtraHidNeurons--;
		}
		
		/*distribute the number of neurons required*/
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Output] = NbOfOutNeuronsPerThread; 
		
		/*allocate any extra neurons if needed*/
		if(ExtraHidNeurons)
		{
			pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Output]++;
			ExtraOutNeurons--;
		}
		
#if VERBOSITY>0
		/*report the amount of work for each worker*/
    	printf("Thread: %d, NbHidden: %d\n",ii,pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Hidden]);
    	printf("Thread: %d, NbOutput: %d\n",ii,pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Output]);
#endif

		/*Copy indented activity vectors for direct acccess*/
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].AllInputActivity = pNNetwork->NeuronsActivity;
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].AllHiddenActivity = &(pNNetwork->NeuronsActivity[HiddenOffset]);
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].AllOutputActivity = &(pNNetwork->NeuronsActivity[OutputOffset]);
		
		/*define the neurons vectors for each thread, also for direct access*/
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].ToComputeHiddenActivity = &(pNNetwork->NeuronsActivity[HiddenOffset+HiddenThreadOffset]);
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].ToComputeOutputActivity = &(pNNetwork->NeuronsActivity[OutputOffset+OutputThreadOffset]);
		
		/*update the neurons vectors offset for the next thread*/
		HiddenThreadOffset += pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Hidden];
		OutputThreadOffset += pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Output];
		
		/*define the a weight vectors starting at the given location for each thread*/
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].HiddenWeights = &(pNNetwork->Weights[HiddenWeightsThreadOffset]);
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].OutputWeights = &(pNNetwork->Weights[OutputWeightsThreadOffset]);
		
		/*update the offset of the weights for the next thread*/
		HiddenWeightsThreadOffset += pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Hidden]*
									 (pNNetwork->OwnProp.NbOfNeurons[Input]+1);
		OutputWeightsThreadOffset += pNNetwork->MasterThreadParams.WorkerThreadParams[ii].NbNeuronsToCompute[Output]*
									 (pNNetwork->OwnProp.NbOfNeurons[Hidden]+1);
									 
							
    	
	}
}


void TerminateParProc(struct SNNetwork* pNNetwork)
{
	int ii=0;

	/*Set threads to not alive and post an idle job*/
	for(ii = 0; ii < NB_WORK_THREADS; ii++)
	{
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].IsAlive = 0;
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].TypeOfJobToDo = Idle;
		sem_post(&(pNNetwork->MasterThreadParams.SyncStuff.WorkerGo_sem));
	}
	
	/*erase semaphore and barroer*/
	sem_destroy(&(pNNetwork->MasterThreadParams.SyncStuff.WorkerGo_sem));
	pthread_barrier_destroy(&(pNNetwork->MasterThreadParams.SyncStuff.AllInSync_barrier));	

	//ticket_barrier_destroy(&MyTicketBarrier);
}

/*worker thread*/
void* WorkerThread(void * params)
{
	int i=0;
	int j=0;
	float InputSum=0;
	
	/*make local copy to speedup a bit and simplify the code*/
	struct SWorkerThreadInitParams* ThreadParams = (struct SWorkerThreadInitParams*)params;
	struct SNNetworkProp OwnProp = ThreadParams->OwnProp;
	
	float* AllInputActivity = ThreadParams->AllInputActivity;
	float* AllHiddenActivity = ThreadParams->AllHiddenActivity;
	float* AllOutputActivity = ThreadParams->AllOutputActivity;
	
	float* ToComputeHiddenActivity = ThreadParams->ToComputeHiddenActivity;
	float* ToComputeOutputActivity = ThreadParams->ToComputeOutputActivity;
	
	float* HiddenWeights = ThreadParams->HiddenWeights;
	float* OutputWeights = ThreadParams->OutputWeights;
	
	int WeightPointer = 0;
	int NeuronPointer = 0;
	
		
	/*While alive*/
	while(ThreadParams->IsAlive)
	{
    	/*Wait for semaphore*/
    	sem_wait(&(ThreadParams->pSyncStuff->WorkerGo_sem)); 
    	
    	/*Check for job to do*/
		switch(ThreadParams->TypeOfJobToDo)
		{
			/*The first type of job is the forward pass*/
			case ForwardProp:
			
				/*Compute hidden layer*/
				WeightPointer = 0;
				NeuronPointer = 0;

				/*For all hidden unit allocated to this thread*/
				for(i=0;i<ThreadParams->NbNeuronsToCompute[Hidden];i++)
				{
					/*sum inputs from input layer*/
					InputSum = 0;
					for(j=0;j<OwnProp.NbOfNeurons[Input];j++)
					{
						InputSum += HiddenWeights[WeightPointer++]*AllInputActivity[j];
					}
					/*plus the connection to the bias*/
					InputSum += HiddenWeights[WeightPointer++]*OwnProp.HiddenLayerBias;
		
					/*sigmoid non-nonlinearity*/
					ToComputeHiddenActivity[NeuronPointer++] = SigmoidFunction(InputSum);
					
				}
	
				/*Barrier*/
				pthread_barrier_wait(&(ThreadParams->pSyncStuff->AllInSync_barrier));

				//ticket_barrier_wait(&MyTicketBarrier);

				/*reset offset counter*/
				WeightPointer = 0;
				NeuronPointer = 0;
			
				/*Compute output layer*/
				/*For all output layer*/
				for(i=0;i<ThreadParams->NbNeuronsToCompute[Output];i++)
				{
					/*sum all inputs*/
					InputSum = 0;
					for(j=0;j<OwnProp.NbOfNeurons[Hidden];j++)
					{
						InputSum += OutputWeights[WeightPointer++]*AllHiddenActivity[j];
					}
					InputSum += OutputWeights[WeightPointer++]*OwnProp.OutputLayerBias;

					/*sigmoid non-nonlinearity*/
					ToComputeOutputActivity[NeuronPointer++] = SigmoidFunction(InputSum);
				}

				/*Barrier*/
				pthread_barrier_wait(&(ThreadParams->pSyncStuff->AllInSync_barrier));

				//ticket_barrier_wait(&MyTicketBarrier);

			break;
			
#if 0
			/*The second type of job is the backpropagation of error*/
			case BackProp:
				/*Compute deltas output layer*/
				/*Barrier*/
				pthread_barrier_wait(&(ThreadParams->pSyncStuff->AllInSync_barrier));
				
				/*Compute deltas hidden layer*/
				/*Barrier*/
				pthread_barrier_wait(&(ThreadParams->pSyncStuff->AllInSync_barrier));
				
				/*adjust weights output Layer*/
				/*Barrier*/
				pthread_barrier_wait(&(ThreadParams->pSyncStuff->AllInSync_barrier));
			break;
#endif
			case Idle:
			break;
			
		}
	}
	
}



/*Feedforward propogation of activation. The activity of the input layer*/
/*should already have been set using SetInput()*/
/*This function will compute the activity of all cells down*/
/*output layer*/
void ComputeNetwork(struct SNNetwork* pNNetwork)
{

	int i,j=0;
	int WeightPointer=0;
	int NeuronPointer=0;
	int HiddenLayerOffset=0;
	int OutputLayerOffset=0;
	float InputSum;
	
	/*Compute activation for hidden layer*/
	/*Offset over the Input Layer*/
	NeuronPointer = pNNetwork->OwnProp.NbOfNeurons[Input];

	float* NeuronsActivity = pNNetwork->NeuronsActivity;
	float* Weights = pNNetwork->Weights;
	struct SNNetworkProp OwnProp = pNNetwork->OwnProp;
	
	
	
#if 1
/*That's the parallel processing solution*/


	int ii=0;
	
	
	/*Post a forward pass job*/
	for(ii = 0; ii < NB_WORK_THREADS; ii++)
	{
		pNNetwork->MasterThreadParams.WorkerThreadParams[ii].TypeOfJobToDo = ForwardProp;
		sem_post(&(pNNetwork->MasterThreadParams.SyncStuff.WorkerGo_sem));
	}
	
	/*wait for hidden layer*/
	pthread_barrier_wait(&(pNNetwork->MasterThreadParams.SyncStuff.AllInSync_barrier));
	//ticket_barrier_wait(&MyTicketBarrier);
	
	/*wait for output layer*/
	pthread_barrier_wait(&(pNNetwork->MasterThreadParams.SyncStuff.AllInSync_barrier));
	//ticket_barrier_wait(&MyTicketBarrier);


#else
/*That's the sequential solution*/
	/*For all hidden units*/
	for(i=0;i<OwnProp.NbOfNeurons[Hidden];i++)
	{
		/*sum inputs from input layer*/
		InputSum = 0;
		for(j=0;j<OwnProp.NbOfNeurons[Input];j++)
		{
			InputSum += Weights[WeightPointer++]*NeuronsActivity[j];
		}
		/*plus the connection to the bias*/
		InputSum += Weights[WeightPointer++]*OwnProp.HiddenLayerBias;
		
		/*sigmoid non-nonlinearity*/
		NeuronsActivity[NeuronPointer++] = SigmoidFunction(InputSum);
	}

	/*Compute Activation for output layer*/
	/*precompute the limits of sources (in hidden layer)*/
	HiddenLayerOffset = OwnProp.NbOfNeurons[Input];
	OutputLayerOffset = OwnProp.NbOfNeurons[Input]+OwnProp.NbOfNeurons[Hidden];
	
	/*readjust the pointers*/
	NeuronPointer = OutputLayerOffset;
	WeightPointer = (OwnProp.NbOfNeurons[Input]+1)*OwnProp.NbOfNeurons[Hidden];
	
	/*For all output layer*/
	for(i=0;i<OwnProp.NbOfNeurons[Output];i++)
	{
		/*sum all inputs*/
		InputSum = 0;
		for(j=HiddenLayerOffset;j<OutputLayerOffset;j++)
		{
			InputSum += Weights[WeightPointer++]*NeuronsActivity[j];
		}
		InputSum += Weights[WeightPointer++]*OwnProp.OutputLayerBias;

		/*sigmoid non-nonlinearity*/
		NeuronsActivity[NeuronPointer++] = SigmoidFunction(InputSum);
	}
#endif

}


/*Back propagation of error algorithm*/
/*We start by computing the deltas of the output and hidden layers*/
/*And then we compute the new weights*/
void BackPropError(struct SNNetwork* pNNetwork, float* DesiredOutput)
{
	int i,j=0;
	float SumOfWeightedOutputs = 0.0;
	
	/*Computes the offsets*/
	int HiddenActOffset = pNNetwork->OwnProp.NbOfNeurons[0];
	int OutputLayerOffset = pNNetwork->OwnProp.NbOfNeurons[Input]+pNNetwork->OwnProp.NbOfNeurons[Hidden];
	
	/*Get Local pointers to reduce indirect memory accesses*/
	float* InputLayer = &(pNNetwork->NeuronsActivity[0]);
	float* HiddenLayer = &(pNNetwork->NeuronsActivity[HiddenActOffset]);
	float* OutputLayer = &(pNNetwork->NeuronsActivity[OutputLayerOffset]);
	float* Weights = &(pNNetwork->Weights[0]);
	float* OutputDeltas = &(pNNetwork->OutputDeltas[0]);
	float* HiddenDeltas = &(pNNetwork->HiddenDeltas[0]);
	struct SNNetworkProp OwnProp = pNNetwork->OwnProp;

	/*Compute the deltas*/
	/*Compute the deltas for the output layer*/
	/*Delta for output = F'(Yo)*(To-Yo)*/
	/*Do = Gain*(To-Yo)*/
	/*To = Target(or desired) output*/
	/*Yo = actual output*/
	for(i=0;i<OwnProp.NbOfNeurons[Output];i++)
	{
		OutputDeltas[i] = (DesiredOutput[i]-OutputLayer[i]);
	}

	/*Compute the deltas for the hidden layer*/
	/*precompute the offsets*/
	int HidToOutWeightOffset = OwnProp.NbOfNeurons[Hidden]*OwnProp.NbOfNeurons[Input]+OwnProp.NbOfNeurons[Hidden];
	float* HidToOutLayerWeights = &(Weights[HidToOutWeightOffset]);
	int JumpOffset = OwnProp.NbOfNeurons[Hidden]+1;
	
	/*Delta = F'(Yh)*SumForAllOutput(Do*Who)*/
	/*Delta = F'(Yh)*E(Do*Who)*/
	/*Delta = Yh*(1-Yh)*E(Do*Who)*/
	for(i=0;i<OwnProp.NbOfNeurons[Hidden];i++)
	{
		SumOfWeightedOutputs = 0.0;
		for(j=0;j<OwnProp.NbOfNeurons[Output];j++)
		{
			SumOfWeightedOutputs+=OutputDeltas[j]*HidToOutLayerWeights[i+j*JumpOffset];
		}
		
		HiddenDeltas[i] = HiddenLayer[i]*(1-HiddenLayer[i])*SumOfWeightedOutputs;
	}

	/*Compute the new weights*/
	/*We proceed backward, (usually it goes from output to hidden)*/
	/*but we precomputed the deltas, so we can go from input to output*/
	/*this is slightly more consistent with the cache memory and easier on computation*/
	int WeightOffset = 0;

	/*weights between the input and hidden layer*/
	/*Wih(t+1) = Wih(t)+N*Dh*Yi*/
	for(i=0;i<OwnProp.NbOfNeurons[Hidden];i++)
	{
		for(j=0;j<OwnProp.NbOfNeurons[Input];j++)
		{
			Weights[WeightOffset++] += OwnProp.LearningRate*HiddenDeltas[i]*InputLayer[j];
		}
		Weights[WeightOffset++] += OwnProp.LearningRate*HiddenDeltas[i]*OwnProp.HiddenLayerBias;
	}
	
	/*weights between the hidden and output layer*/
	/*Who(t+1) = Who(t)+N*Do*Yh*/
	for(i=0;i<OwnProp.NbOfNeurons[Output];i++)
	{
		for(j=0;j<OwnProp.NbOfNeurons[Hidden];j++)
		{
			Weights[WeightOffset++] += OwnProp.LearningRate*OutputDeltas[i]*HiddenLayer[j];
		}
		Weights[WeightOffset++] += OwnProp.LearningRate*OutputDeltas[i]*OwnProp.OutputLayerBias;
	}
}


/*Get output*/
/*Copy the output layer into the memory area provided by the user*/
void GetOutput(struct SNNetwork* pNNetwork, float* OutputActivity)
{
	int i=0;
	int OutputOffset = pNNetwork->OwnProp.TotNumberOfNeurons-pNNetwork->OwnProp.NbOfNeurons[Output];
	float* OutputLayer = &(pNNetwork->NeuronsActivity[OutputOffset]);
	
	for(i=0;i<pNNetwork->OwnProp.NbOfNeurons[Output];i++)
	{
		OutputActivity[i] = OutputLayer[i];
	}

}

/*the non-linear function*/
inline float SigmoidFunction(float x)
{
	return 1/(1+exp(-x));
}

#if 0
static int arrived = 0;
static int N = 5;

void barrier()
{
	pthread_mutex_lock(&mutex_arr);
	arrived++;
	if (arrived<N) {
		pthread_cond_wait(&cond, &mutex_arr);
	}
	else {
		pthread_cond_broadcast(&cond);
		arrived=0; /* be prepared for next barrier */
	}
	pthread_mutex_unlock(&mutex_arr);
}
#endif

