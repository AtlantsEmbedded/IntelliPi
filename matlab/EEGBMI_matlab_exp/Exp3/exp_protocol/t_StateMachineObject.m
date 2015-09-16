classdef t_StateMachineObject < handle
    
    properties (Constant)
       
        % list of states
        IDLE = 0;
        SETUP_EXPERIMENT = 1;
        INTER_TRIAL = 2;
        SETUP_TRIAL = 3;
        
        % baseline is deactivated
        BASE_INSTRUCTION = 4;
        BASE_PREPARE = 5;
        BASE_RECORD = 6;
        BASE_BUFFER = 11;
        
        TASK_INSTRUCTION = 7;
        TASK_PREPARE = 8;
        TASK_RECORD = 9;
        TASK_BUFFER = 10;
        
        END_TRIAL = 12;
        SHUTDOWN_SYSTEM = 13;
        
        BASELINE = 3;
        TASK_1 = 1;
        TASK_2 = 2;
        
        BASELINE_msg = 'Task: Think of nothing';
        TASK_1_msg = 'Task: Eye open';
        TASK_2_msg = 'Task: Eye closed + relax';
        
        MODES = {'Record','Test'};

        % beep definition
        PREPARE_SOUND_CUE = [0.025*sin((1/8192:1/8192:0.25)*2*pi*200)];
        % beep-beep definition
        STOP_SOUND_CUE = [0.025*sin([1/8192:1/8192:0.25]*2*pi*200) zeros(1,2048) 0.025*sin([1/8192:1/8192:0.25]*2*pi*200)];
        
    end
        
    properties (Access = public)
        
		
        % fellow objects
        ScreenO;
        LogO;
        TaskMachineO;
        MuseO;
        
        log_struct;
        
		screen_options;
		task_machine_options;
        
        trial_id = 1;
        cur_task = 0;
        
        % state system running variables
        run_mode = 0;
        current_state = 0;
        next_state = 0;
        rand_fix_time = 0;
        
        % timer length variables (all in ms)
        timer_inter_trial = 1500;
        timer_instruction = 3500;
        timer_prepare = 3000;
        timer_record = 5000;
        timer_buffer = 1500;
		
        % timer time tracking variables
        waittill_instruction = 0;
        waittill_prepare = 0;
        waittill_record = 0;
        waittill_buffer = 0;
        waittill_inter_trial = 0;
        
        % The amount of time for which the experiment has been running
        OnTime = 0;
         
        % Number of trials to be executed
        nb_of_record_trials = 150;
        nb_of_test_trials = 0;
        
        pause_taken = 0;

        CurMode = '';
        
        % 1, when the system needs to be shut down
        ShutDownExperiment = 0;
		
        
    end
    
    methods (Access = public)
        
        function StateMachineO = t_StateMachineObject()
			
            % options for the screen object
			StateMachineO.screen_options.nb_tasks = 2;
		
            % options for the task machine object
			StateMachineO.task_machine_options.nb_tasks = 2;
            
            % init logging structure
            StateMachineO.log_struct.trial_id = 0;
            StateMachineO.log_struct.task = 0;
            
            % start task state machine
			StateMachineO.TaskMachineO = t_TaskMachineObject(StateMachineO.task_machine_options); 
            
            % start logger and screen
            StateMachineO.LogO = t_LogObject();
            StateMachineO.ScreenO = t_ScreenObject(0,StateMachineO.LogO);
            
            % initialise and start muse-io
            StateMachineO.MuseO = t_muse_object();
            StateMachineO.MuseO.start_muse_io();
        end
        
        function StartSystem(StateMachineO)
            StateMachineO.run_mode = 1;
        end
        
        
        function StopSystem(StateMachineO)
            StateMachineO.run_mode = 0;
        end
        
        function [Continue] = RunStateSystem(StateMachineO)
        
            % check if there has been a transition
            if StateMachineO.current_state ~= StateMachineO.next_state
                
                % yes, execute the action
                StateMachineO.current_state = StateMachineO.next_state;
				
                switch(StateMachineO.current_state)

                    case StateMachineO.IDLE
                        StateMachineO.Idle_ACT();
                        
                    case StateMachineO.SETUP_EXPERIMENT
                        StateMachineO.SetupExperiment_ACT();
                        
                    case StateMachineO.SHUTDOWN_SYSTEM
                        StateMachineO.ShutDownSystem_ACT();
                        
                    case StateMachineO.INTER_TRIAL
                        StateMachineO.InterTrial_ACT();
                        
                    case StateMachineO.SETUP_TRIAL
                        StateMachineO.SetupTrial_ACT();
                        
                    case StateMachineO.BASE_INSTRUCTION
                        StateMachineO.base_Instruction_ACT();
                        
                    case StateMachineO.BASE_PREPARE
                        StateMachineO.base_Prepare_ACT();
                        
                    case StateMachineO.BASE_BUFFER
                        StateMachineO.base_Buffer_ACT();
                        
                    case StateMachineO.TASK_INSTRUCTION
                        StateMachineO.task_Instruction_ACT();
                        
                    case StateMachineO.TASK_PREPARE
                        StateMachineO.task_Prepare_ACT();
                        
                    case StateMachineO.TASK_BUFFER
                        StateMachineO.task_Buffer_ACT();
                        
                    case StateMachineO.END_TRIAL
                        StateMachineO.EndTrial_ACT();
                        
                    otherwise    
                        error('Invalid state')
                end
                
            else
                
                % no, execute the transition
                switch(StateMachineO.current_state)

                    case StateMachineO.IDLE
                        StateMachineO.Idle_TRN();
                         
                    case StateMachineO.SETUP_EXPERIMENT
                        StateMachineO.SetupExperiment_TRN();
                        
                    case StateMachineO.SHUTDOWN_SYSTEM
                        StateMachineO.ShutDownSystem_TRN();
                        
                    case StateMachineO.INTER_TRIAL
                        StateMachineO.InterTrial_TRN();
                        
                    case StateMachineO.SETUP_TRIAL
                        StateMachineO.SetupTrial_TRN();
                        
                    case StateMachineO.BASE_INSTRUCTION
                        StateMachineO.base_Instruction_TRN();
                        
                    case StateMachineO.BASE_PREPARE
                        StateMachineO.base_Prepare_TRN();
                        
                    case StateMachineO.BASE_BUFFER
                        StateMachineO.base_Buffer_TRN();
                        
                    case StateMachineO.TASK_INSTRUCTION
                        StateMachineO.task_Instruction_TRN();
                        
                    case StateMachineO.TASK_PREPARE
                        StateMachineO.task_Prepare_TRN();
                        
                    case StateMachineO.TASK_BUFFER
                        StateMachineO.task_Buffer_TRN();
                        
                    case StateMachineO.END_TRIAL
                        StateMachineO.EndTrial_TRN();

                    otherwise    
                        error('Invalid transition')
                end
            end
            
            if StateMachineO.run_mode
                Continue = 1;
            else
                Continue = 0;
            end
        
        end
        
        
        function [ScreenO] = GetScreenO(StateMachineO)
            ScreenO = StateMachineO.ScreenO;
        end
        
    end
    
    
    methods (Access = private)
        
        % ---------------------------------------------%
        % ---------------------------------------------%
        function Idle_ACT(StateMachineO)
            fprintf('Idle State, waiting for the go');
        end
        
        function Idle_TRN(StateMachineO)
            
            if ~StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.IDLE;
                return;
            end
            
            if StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.SETUP_EXPERIMENT;
                return;
            end 
        end
        
        % ---------------------------------------------%
        % Setup Experiment
        % This states is used to initialize the various components
        % of the experiment such as the screen, the task machine and to
        % log the experiment information that will for the header of the
        % log file.
        % ---------------------------------------------%
        function SetupExperiment_ACT(StateMachineO)
            fprintf('Setup the Experiment');
            
            % Get time reference
            StateMachineO.OnTime = GetSecs;
            
            StateMachineO.CurMode = StateMachineO.MODES{1};
            
            % log initials
            clc;
            log_exp_struct.initials = input('Please enter your initials: ','s');
            log_exp_struct.nb_of_tasks = StateMachineO.screen_options.nb_tasks;
            log_exp_struct.nb_of_record_trials = StateMachineO.nb_of_record_trials;
            log_exp_struct.nb_of_test_trials = StateMachineO.nb_of_test_trials;

            StateMachineO.LogO.LogExpInfos(log_exp_struct);

            % Init the screen
            StateMachineO.ScreenO.FireUpTheScreen(StateMachineO.TaskMachineO);
            
        end
        
        function SetupExperiment_TRN(StateMachineO)
            
            if ~StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.IDLE;
                return;
            end
            
            if StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.INTER_TRIAL;
                return;
            end
        end
        
        % ---------------------------------------------%
        % Shut down
        % This states terminate the execution of the experiment
        % it makes sure that everything is terminated nicely.
        % ---------------------------------------------%
        function ShutDownSystem_ACT(StateMachineO)
            % inform user
            fprintf('ShutDown the system');
            
            % update state machine variables
            StateMachineO.ShutDownExperiment = 0;
            StateMachineO.run_mode = 0;
            
            % close the screen
            StateMachineO.ScreenO.ShutDownTheScreen();
            
            % close the logger
            StateMachineO.LogO.CloseFile();
            
        end
        
        function ShutDownSystem_TRN(StateMachineO)
            StateMachineO.next_state = StateMachineO.IDLE;
            return;
        end
        
        
        % ---------------------------------------------%
        % Inter Trial
        % The inter-trial delay is at the beginning of a trial and
        % gives the subject a short pause before the onset of the 
        % next trial.
        % ---------------------------------------------%
        function InterTrial_ACT(StateMachineO)
            StateMachineO.waittill_inter_trial = (StateMachineO.timer_inter_trial/1000)+GetSecs;
        end
        
        function InterTrial_TRN(StateMachineO)
            
            if ~StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.IDLE;
                return;
            end
            
            if  StateMachineO.run_mode && StateMachineO.waittill_inter_trial < GetSecs
                StateMachineO.next_state = StateMachineO.SETUP_TRIAL;
                return;
            end 
        end
        
        % ---------------------------------------------%
        % Setup Trial
        % This state setups the trial
        %
        % ---------------------------------------------%
        function SetupTrial_ACT(StateMachineO)
            
            % setup the trial to define what the task will be
			StateMachineO.TaskMachineO.SetupNewTrial();
            StateMachineO.cur_task = StateMachineO.TaskMachineO.GetTask;
            
        end
        
        function SetupTrial_TRN(StateMachineO)
            
            if ~StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.IDLE;
                return;
            end
            
            if StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.TASK_INSTRUCTION;
                return;
            end
        end
%         
%         % ---------------------------------------------%
%         % ---------------------------------------------%
%         function base_Instruction_ACT(StateMachineO)
%             
%             StateMachineO.waittill_instruction = (StateMachineO.timer_instruction/1000)+GetSecs;
%             
%             StateMachineO.ScreenO.ShowInstruction(StateMachineO.BASELINE_msg);
%             fprintf(StateMachineO.BASELINE_msg);
%             
%         end
%         
%         function base_Instruction_TRN(StateMachineO)
%             
%             if ~StateMachineO.run_mode
%                 StateMachineO.next_state = StateMachineO.IDLE;
%                 return;
%             end
%             
%             if StateMachineO.waittill_instruction < GetSecs
%                 StateMachineO.next_state = StateMachineO.BASE_PREPARE;
%                 return;
%             end
%             
%         end
%         
%         
%         % ---------------------------------------------%
%         % Prepare
%         % This state allow for the subject to prepare himself
%         % to produce the mental state required.
%         %
%         % A beep sound is played at the beginning
%         % ---------------------------------------------%
%         function base_Prepare_ACT(StateMachineO)
%             % beep
%             sound(StateMachineO.PREPARE_SOUND_CUE);
%             
%             StateMachineO.ScreenO.HideInstruction;
%             
%             % start muse-io recording
%             StateMachineO.MuseO.start_recording(StateMachineO.trial_id, StateMachineO.BASELINE);
%             
%             
%         end
%         
%         function base_Prepare_TRN(StateMachineO)
%             
%             if ~StateMachineO.run_mode
%                 StateMachineO.next_state = StateMachineO.IDLE;
%                 return;
%             else
%                 StateMachineO.next_state = StateMachineO.BASE_BUFFER;
%                 return;
%             end
%             
%         end
%         
%         
%         % ---------------------------------------------%
%         % ---------------------------------------------%
%         function base_Buffer_ACT(StateMachineO)
%             
%             % beep-beep
%             sound(StateMachineO.STOP_SOUND_CUE);
%             
%             
%         end
%         
%         function base_Buffer_TRN(StateMachineO)
%             
%             if ~StateMachineO.run_mode
%                 StateMachineO.next_state = StateMachineO.IDLE;
%                 return;
%             else
%                 StateMachineO.next_state = StateMachineO.TASK_INSTRUCTION;
%                 return;
%             end
%             
%         end
%         
        
        % ---------------------------------------------%
        % ---------------------------------------------%
        function task_Instruction_ACT(StateMachineO)
            
            StateMachineO.waittill_instruction = (StateMachineO.timer_instruction/1000)+GetSecs;
            
            % show the instruction on screen, the instruction will depend on the 
            % task that has been chosen by the task machine
            switch(StateMachineO.cur_task)
               
                case StateMachineO.TASK_1
                    StateMachineO.ScreenO.ShowInstruction(StateMachineO.TASK_1_msg);
                    fprintf(StateMachineO.TASK_1_msg);
                    
                case StateMachineO.TASK_2
                    StateMachineO.ScreenO.ShowInstruction(StateMachineO.TASK_2_msg);
                    fprintf(StateMachineO.TASK_2_msg);
                    
                otherwise
                    
            end
            
        end
        
        function task_Instruction_TRN(StateMachineO)
            
            if ~StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.IDLE;
                return;
            end
            
            if StateMachineO.waittill_instruction < GetSecs
                StateMachineO.next_state = StateMachineO.TASK_PREPARE;
                return;
            end
            
        end
        
        
        % ---------------------------------------------%
        % Prepare
        % This state allow for the subject to prepare himself
        % to produce the mental state required.
        %
        % A beep sound is played at the beginning
        % ---------------------------------------------%
        function task_Prepare_ACT(StateMachineO)
            % beep
            sound(StateMachineO.PREPARE_SOUND_CUE);
            
            StateMachineO.ScreenO.HideInstruction;
            
            % start muse-io recording
            StateMachineO.MuseO.start_recording(StateMachineO.trial_id, StateMachineO.TASK_1);
            
        end
        
        function task_Prepare_TRN(StateMachineO)
            
            if ~StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.IDLE;
                return;
            else
                StateMachineO.next_state = StateMachineO.TASK_BUFFER;
                return;
            end
            
        end
        
        % ---------------------------------------------%
        % ---------------------------------------------%
        function task_Buffer_ACT(StateMachineO)
            
            % beep-beep
            sound(StateMachineO.STOP_SOUND_CUE);
            
            % log the inf about the current trial
            StateMachineO.log_struct.trial_id = StateMachineO.trial_id;
            StateMachineO.log_struct.task = StateMachineO.cur_task;
            StateMachineO.LogO.LogTrial(StateMachineO.log_struct);
            
            % update the trial count
            StateMachineO.trial_id = StateMachineO.trial_id+1;
            
            clc;
            
            fprintf('Trial ID: %f',StateMachineO.trial_id);
            
        end
        
        function task_Buffer_TRN(StateMachineO)
            
            if ~StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.IDLE;
                return;
            else
                StateMachineO.next_state = StateMachineO.END_TRIAL;
                return;
            end
            
        end
        
        
        % ---------------------------------------------%
        % ---------------------------------------------%
        function EndTrial_ACT(StateMachineO)
            
            % check if we are due for a break (halfway through)
            if StateMachineO.trial_id > StateMachineO.nb_of_record_trials/2 && ~StateMachineO.pause_taken
                StateMachineO.pause_taken = 1;
                StateMachineO.ScreenO.ShowInstruction('Pause, press key when ready');
                pause
            end
            
            if StateMachineO.trial_id > StateMachineO.nb_of_record_trials;
                StateMachineO.ShutDownExperiment = 1;
                return;
            end
        end
        
        function EndTrial_TRN(StateMachineO)
            
            if ~StateMachineO.run_mode
                StateMachineO.next_state = StateMachineO.IDLE;
                return;
            elseif StateMachineO.ShutDownExperiment
                StateMachineO.next_state = StateMachineO.SHUTDOWN_SYSTEM;
                return;
            else
                StateMachineO.next_state = StateMachineO.INTER_TRIAL;
                return;
            end
            
        end
        
    end
    
end
