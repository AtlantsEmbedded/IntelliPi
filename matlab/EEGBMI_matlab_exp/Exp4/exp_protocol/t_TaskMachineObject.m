classdef t_TaskMachineObject < handle
    
    
    properties (Constant)
       
        MODES = {'Record','Test'};
        
    end
    
    
    properties (Access=public)
        
        cur_mode = '';
		
        task_counters;
        cur_task;
		nb_tasks;
        
    end
    
    methods (Access=public)

        % this is the constructor of the object
        % initialise the class members to their default values
        function [TaskMachineO] = t_TaskMachineObject(opt)
        
            % default mode
            TaskMachineO.cur_mode = TaskMachineO.MODES{1};
			
            % copy options
			TaskMachineO.nb_tasks = opt.nb_tasks;
            
            % initialize the tasks counters
            TaskMachineO.task_counters = zeros(opt.nb_tasks,1);
            
        end
        
        % method to reset the mode to the default state
        function ResetMode(TaskMachineO)
            TaskMachineO.cur_mode = TaskMachineO.MODES{1};
        end
        
        % method to switch the mode to the test state (not used)
        function SwitchToTestMode(TaskMachineO)
            TaskMachineO.cur_mode = 'Test';
        end
        
        % method to get the current task id
        function [cur_task] = GetTask(TaskMachineO)
            cur_task = TaskMachineO.cur_task;
		end
		
        % method to setup a new trial, it defines what the current task will be
        function SetupNewTrial(TaskMachineO)
        
            if strcmp(TaskMachineO.cur_mode,'Record')
                
                % check how many of each task has been done
                max_task_count = max(TaskMachineO.task_counters);
                [min_task_count, low_task_id]= min(TaskMachineO.task_counters);
                
                TaskMachineO.task_counters
                
                % if the disparity is above 3, 
                if (max_task_count-min_task_count)>=3
                    % yes, force the next trial to be of lowest count task
                    TaskMachineO.cur_task = low_task_id;
                else
                    % otherwise leave it random
                    TaskMachineO.cur_task = randi(TaskMachineO.nb_tasks);
                end
                
                % update task counter
                TaskMachineO.task_counters(TaskMachineO.cur_task) = TaskMachineO.task_counters(TaskMachineO.cur_task)+1;
                
            elseif strcmp(TaskMachineO.cur_mode,'Test')
                
                % check how many of each task has been done
                max_task_count = max(TaskMachineO.task_counters);
                [min_task_count, low_task_id]= min(TaskMachineO.task_counters);
                
                % if the disparity is above 3, 
                if (max_task_count-min_task_count)>3
                    % yes, force the next trial to be of lowest count task
                    TaskMachineO.cur_task = low_task_id;
                else
                    % otherwise leave it random
                    TaskMachineO.cur_task = randi(2);
                end
                
                % update task counter
                TaskMachineO.task_counters(TaskMachineO.cur_task) = TaskMachineO.task_counters(TaskMachineO.cur_task)+1;
            end
		
        end
        
    end
	
    
    methods (Access=private)
        
        
    end
    
    
end

