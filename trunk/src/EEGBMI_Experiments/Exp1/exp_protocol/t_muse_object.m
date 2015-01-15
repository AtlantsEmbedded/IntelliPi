classdef t_muse_object < handle
    
    properties
        
        
    end
    
    methods
        
        function start_recording(t_muse_object,trial_id)
            
            % quickly write a batch file that runs the muse-player as needed
            plaunch_file = fopen('launch.bat','w');
            fprintf(plaunch_file,['muse-player -l udp:5001 -M trial_' num2str(trial_id) '.m\n']);
            fprintf(plaunch_file,'exit');
            fclose(plaunch_file);
            
            % run the batch file
            !launch.bat &
            
            % give it the time to start
            pause(1.5)
            
        end
        
        function stop_recording(t_muse_object)
            
            perl('quitproc.pl','muse-player.exe');
            
        end
        
    end
    
end

