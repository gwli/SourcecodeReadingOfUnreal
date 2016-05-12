//Multi thread Test, finding prime number
namespace VictoryMultiThreadTest
{
	//Multi-threaded link to UObjects, do not create,modify,destroy UObjects / AActors via this link!
	AVictoryGamePlayerController* ThePC;
 
	//~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~
	//   OUTPUT RESULTS OF TASK THREADS
	TArray<uint32> PrimeNumbers;
 
	// This is the array of thread completions to know if all threads are done yet
	FGraphEventArray		VictoryMultithreadTest_CompletionEvents;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~
 
	//~~~~~~~~~~~~~~~
	//Are All Tasks Complete?
	//~~~~~~~~~~~~~~~
	bool TasksAreComplete()
	{
		//Check all thread completion events
		for (int32 Index = 0; Index < VictoryMultithreadTest_CompletionEvents.Num(); Index++)
		{
			//If  ! IsComplete()
			if (!VictoryMultithreadTest_CompletionEvents[Index]->IsComplete())
			{
				return false;
			}
		}
		return true;
	}
	//~~~~~~~~~~~
	//Actual Task Code
	//~~~~~~~~~~~
	int32 FindNextPrimeNumber()
	{
		//Last known prime number  + 1
		int32 TestPrime = PrimeNumbers.Last();
 
		bool NumIsPrime = false;
		while( ! NumIsPrime)
		{
			NumIsPrime = true;
 
			//Try Next Number
			TestPrime++;
 
			//Modulus from 2 to current number - 1 
			for(int32 b = 2; b < TestPrime; b++)
			{
				if(TestPrime % b == 0) 
				{
					NumIsPrime = false;
					break;
					//~~~
				}
			}
		}
 
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//Did another thread find this number already?
		if(PrimeNumbers.Contains(TestPrime))
		{
			return FindNextPrimeNumber(); //recursion
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
		//Success!
		return TestPrime;
	}
 
 
	//~~~~~~~~~~~
	//Each Task Thread
	//~~~~~~~~~~~
	class FVictoryTestTask
	{
 
	  public:
		FVictoryTestTask() //send in property defaults here
		{
			//can add properties here
		}
 
		/** return the name of the task **/
		static const TCHAR* GetTaskName()
		{
			return TEXT("FVictoryTestTask");
		}
		FORCEINLINE static TStatId GetStatId()
		{
			RETURN_QUICK_DECLARE_CYCLE_STAT(FVictoryTestTask, STATGROUP_TaskGraphTasks);
		}
		/** return the thread for this task **/
		static ENamedThreads::Type GetDesiredThread()
		{
			return ENamedThreads::AnyThread;
		}
 
 
		/*
		namespace ESubsequentsMode
		{
			enum Type
			{
				//Necessary when another task will depend on this task. 
				TrackSubsequents,
				//Can be used to save task graph overhead when firing off a task that will not be a dependency of other tasks. 
				FireAndForget
			};
		}
		*/
		static ESubsequentsMode::Type GetSubsequentsMode() 
		{ 
			return ESubsequentsMode::TrackSubsequents; 
		}
 
                //~~~~~~~~~~~~~~~~~~~~~~~~
                //Main Function: Do Task!!
                //~~~~~~~~~~~~~~~~~~~~~~~~
		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
		{
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
			PrimeNumbers.Add(FindNextPrimeNumber());
 
			//***************************************
			//Show Incremental Results in Main Game Thread!
 
			//	Please note you should not create, destroy, or modify UObjects here.
			//	  Do those sort of things after all thread are completed.
 
			//	  All calcs for making stuff can be done in the threads
			//	     But the actual making/modifying of the UObjects should be done in main game thread,
			//		 which is AFTER all tasks have completed :)
 
			ThePC->ClientMessage(FString("A thread completed! ~ ") + FString::FromInt(PrimeNumbers.Last()));
			//***************************************
		}
	};
 
	//~~~~~~~~~~~~~~~~~~~
	//  Multi-Task Initiation Point 
	//~~~~~~~~~~~~~~~~~~~
	void FindPrimes(const uint32 TotalToFind)
	{
		PrimeNumbers.Empty();
		PrimeNumbers.Add(2);
		PrimeNumbers.Add(3);
 
		//~~~~~~~~~~~~~~~~~~~~
		//Add thread / task for each of total prime numbers to find
		//~~~~~~~~~~~~~~~~~~~~
 
		for(uint32 b = 0; b < TotalToFind; b++)
		{
			VictoryMultithreadTest_CompletionEvents.Add(TGraphTask<FVictoryTestTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady()); //add properties inside ConstructAndDispatchWhenReady()
		}
	}
 
}

//~~~ In the Game Thread ~~~
 
//timer to check when threads are done
//Please note timers must be in the game thread / main / normal thread
void AVictoryGamePlayerController::VictoryCheckAllThreadsDone()
{
 
	if(VictoryMultiThreadTest::TasksAreComplete() )
	{
		//Clear Timer
		GetWorldTimerManager().ClearTimer(this, 
			&AVictoryGamePlayerController::VictoryCheckAllThreadsDone);
 
		ClientMessage("Multi Thread Test Done!");
 
		VShow("Prime Numbers Found:");
		for(int32 v = 0; v < VictoryMultiThreadTest::PrimeNumbers.Num(); v++)
		{
			VShow(FString::FromInt(v) + FString("~ "), VictoryMultiThreadTest::PrimeNumbers[v]);
		}
	}
}

//Starting the Tasks / Threads
void AVictoryGamePlayerController::StartThreadTest()
{
 
	VictoryMultiThreadTest::ThePC = this;
	VictoryMultiThreadTest::FindPrimes(50000); //first 50,000 prime numbers
 
	//Start a timer to check when all the threads are done!
	GetWorldTimerManager().SetTimer(this, 
		&AVictoryGamePlayerController::VictoryCheckAllThreadsDone, 1, true);	
}

