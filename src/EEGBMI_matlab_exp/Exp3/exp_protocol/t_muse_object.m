classdef t_muse_object < handle
%
% This class implements the interface to the Muse device of Interaxon
% It is meant to be used in a Matlab-based eeg-experiment.
% 
% It makes use of an external .vbs script to launch the muse-player.
% The trial record time length is set in that .vbs file. Matlab waits on 
% the script to complete before moving on.
%
% The name of the .mat file to which we want to samples to be written is set
% in this file. To do so a .bat file is written at each trial that contains 
% the muse-player command line with the -M option and filename.
%
% To use, launch the muse-io prior to attempting to record (function: start_muse_io)
%
% Otherwise, call start_recording, and the function will return once the recording
% is done and samples save in the file.
%
% Warning, allow for some time between trials, we noticed that some trial
% are not recorded properly (~5%). You should sort the files after the experiment.
%
% Developed as part of the EEG-BMI project by Frédéric Simard and Ron Brash
% Atom Embedded, 2015.
% You are free to share, modify, publish as long as you respect authorship.
%
    properties
        % set to 1 to bypass and mimic online Muse
        fake_muse = 0;
        
    end
    
    methods
        
        % constructor
        function t_muse_object = t_muse_object(fake_muse)
            
            % by default, fake_muse is off
            if nargin == 0
                t_muse_object.fake_muse = 0;
            end
        end

        % call to start the muse, through a system call to the .bat file
        function start_muse_io(t_muse_object)
            
            if ~t_muse_object.fake_muse && ~t_muse_object.ismuse_io_active
                !start_museio.bat &
                pause(2);
            end
        end
        
        % call to record a trial. This function returns once the .vbs script
        % is done running.
        function start_recording(t_muse_object,trial_id, trial_code)
            
            if ~t_muse_object.fake_muse
                
                % check the type of trial
                switch(trial_code)
                    
                    % task trials
                    case {1,2}
                        % quickly write a batch file that runs the muse-player as needed
                        plaunch_file = fopen('launch_muse_player.bat','w');
                        fprintf(plaunch_file,['muse-player -l udp:5001 -M trial_' num2str(trial_id) '.mat\n']);
                        fclose(plaunch_file);

                        % run the vbs file
                        !record_trial.vbs
                        
                    % baseline trials
                    case{3}
                        
                        % quickly write a batch file that runs the muse-player as needed
                        plaunch_file = fopen('launch_muse_player.bat','w');
                        fprintf(plaunch_file,['muse-player -l udp:5001 -M trial_' num2str(trial_id) '_base.mat\n']);
                        fclose(plaunch_file);

                        % run the vbs file
                        !record_trial.vbs
                        
                    otherwise
                        error('Muse Object: Invalid type of task')
                end
            else
                pause(6);
            end
            
        end
        
        function stop_recording(t_muse_object)
            
            if ~t_muse_object.fake_muse
                perl('quitproc.pl','muse-player.exe');
            end
        end
		
		function [active] = ismuse_io_active(t_muse_object)
			[status,cmdout] = system('tasklist');
			k = strfind(cmdout, 'muse-io.exe');
            active = ~isempty(k);
        end
        
        
		function [] = get_muse_pids(t_muse_object)
			[status,cmdout] = system('tasklist');
			k = strfind(cmdout, 'muse-player.exe');
            if ~isempty(k)
                pid_1 = textscan(cmdout(k(1):end),'muse-player.exe               %f');
                pid_1 = textscan(cmdout(k(2):end),'muse-player.exe               %f');
            end
		end
        
    end
    
end

