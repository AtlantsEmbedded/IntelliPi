classdef t_openbci < handle
%
% This class implements the openbci object. It provides methods to record
% trials of specific duration to ASCII files.
%
% methods available:
%  - t_openbci, constructor, it initialize the interface
%  - add_data, add a sample to the logger
%  - close_logger, dumps the last data and close the file
%
% Frederic Simard, Atom Embedded, 2015
%
    
    properties(Access=public)
        
        interface = t_openbci_interface.empty();
        logger = t_openbci_logger.empty();
        
    end
    
    methods(Access=public)
        
        % Object constructor
        function openbci = t_openbci()
            
            close_existing_openbci();
            
            % initialize the components 
            openbci.interface = t_openbci_interface();
            openbci.logger = t_openbci_logger.empty();
            
            % reset and activate the hardware
            openbci.interface.open_port();
            openbci.interface.reset_hardware();
            
        end
        
        % blocking call, will record all samples for the specified duration
        % samples are dumped into a file, named after the trial_id
        function record_trial(openbci, trial_id, duration)
            
            % load constants
            openbci_constants;
            
            %empty the buffer
            openbci.interface.flush_buffer();
            
            % open logger and file
            openbci.logger = t_openbci_logger(trial_id);
            
            %log all samples for duration
            tic;
            
            %start streaming
            openbci.interface.start_streaming();
            
            while(toc < duration)
                
                % get every samples and dump in file
                if openbci.interface.get_nb_packets_available>0
                    [eeg_data, nb_packets] = openbci.interface.get_eeg_packets();
                    openbci.logger.add_data(toc, eeg_data, nb_packets);
                end
            end
            
            %stop streaming
            openbci.interface.stop_streaming();
            
            %empty the buffer
            openbci.interface.flush_buffer();
            
            %close datafile
            openbci.logger.close_logger;
            
        end
        
    end
    
end
