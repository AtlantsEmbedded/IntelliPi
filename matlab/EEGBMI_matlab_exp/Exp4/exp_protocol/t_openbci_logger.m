classdef t_openbci_logger < handle
%
% This class implements a logger for the openbci library. It logs
% the eeg data samples and dumps them to a file, using an ASCII format
% periodically.
%
% methods available:
%  - t_openbci_logger, constructor, filename required
%  - add_data, add a sample to the logger
%  - close_logger, dumps the last data and close the file
%
% Frederic Simard, Atom Embedded, 2015
%
    properties(Access=private)
        
        fid; % file id
        
        timestamps_buffer = []; % keeps the time stamps
        data_buffer = []; % keeps the data
        
        dump_to_file_counter = 0; % buffer index and count before dump to file
        
    end
    
    methods(Access=public)
        
        % constructor, opens a file and get ready to log the eeg samples of a trial
        function openbci_logger = t_openbci_logger(trial_id)
            openbci_constants;
            
            openbci_logger.dump_to_file_counter = 0;
            
            openbci_logger.timestamps_buffer = zeros(LOG_BUFFER_SIZE,1);
            openbci_logger.data_buffer = zeros(LOG_BUFFER_SIZE,NB_CHANNELS);
            
            openbci_logger.fid = fopen([LOG_FOLDER LOG_FILENAME_BASE num2str(trial_id) LOG_EXT],'w');
            
            if openbci_logger.fid == -1
               error('logger couldn''t open the file'); 
            end
        end
        
        % add a set of samples to the buffer and dump them to a file if required
        % timestamps are also inserted at the beginning of the line
        % but the tiem stamp is repeated for all samples in the set
        function add_data(openbci_logger, time_stamp, eeg_data, nb_packets)
            openbci_constants;
            
            % add sample to buffer
            for ii=1:nb_packets
                openbci_logger.dump_to_file_counter = openbci_logger.dump_to_file_counter+1;
                openbci_logger.data_buffer(openbci_logger.dump_to_file_counter,:) = eeg_data(ii,:);
                openbci_logger.timestamps_buffer(openbci_logger.dump_to_file_counter) = time_stamp;
            end
            
            % if buffer is full, dump to file
            if openbci_logger.dump_to_file_counter == LOG_BUFFER_SIZE;
                openbci_logger.dump_to_file();
                openbci_logger.dump_to_file_counter = 0;
            end
            
        end
        
        % dump what's in the buffer and closethe file
        function close_logger(openbci_logger)
            openbci_logger.dump_to_file();
            fclose(openbci_logger.fid);
        end
            
    end
    
    methods(Access=private)
        
        % dump the buffer to the ASCII file
        % each line:
        % TIMESTAMP EEG1 EEG2 ... EEG8
        function dump_to_file(openbci_logger)
            openbci_constants;
            
            for ii=1:openbci_logger.dump_to_file_counter
                fprintf(openbci_logger.fid,'%f ',openbci_logger.timestamps_buffer(ii)); 
            for jj=1:NB_CHANNELS
                fprintf(openbci_logger.fid,'%f ',openbci_logger.data_buffer(ii,jj)); 
            end
                fprintf(openbci_logger.fid,'\n');
            end
        end
        
    end
end

