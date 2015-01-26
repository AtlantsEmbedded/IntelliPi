classdef t_muse_object < handle
    
    properties
        fake_muse = 0;
        
    end
    
    methods
        
        function t_muse_object = t_muse_object(fake_muse)
            if nargin == 0
                t_muse_object.fake_muse = 0;
            end
        end
        
        
        
        function start_muse_io(t_muse_object)
            %!muse-io --osc osc.udp://localhost:5001
        end
        
        function start_recording(t_muse_object,trial_id)
            
            if ~t_muse_object.fake_muse
                % quickly write a batch file that runs the muse-player as needed
                plaunch_file = fopen('launch.bat','w');
                fprintf(plaunch_file,['muse-player -l udp:5001 -M trial_' num2str(trial_id) '.mat\n']);
                fclose(plaunch_file);

                % run the batch file
                !vbscript.vbs
                
                % give it the time to start
                %pause(1.5)
                %system('exit')
            end
            
        end
        
        function stop_recording(t_muse_object)
            
            if ~t_muse_object.fake_muse
                perl('quitproc.pl','muse-player.exe');
            end
        end
		
		function get_muse_pids(t_muse_object)
			[status,cmdout] = system('tasklist');
			k = strfind(cmdout, 'muse-player.exe');
			pid_1 = textscan(cmdout(k(1):end),'muse-player.exe               %f');
			pid_1 = textscan(cmdout(k(2):end),'muse-player.exe               %f');
		end
        
    end
    
end

