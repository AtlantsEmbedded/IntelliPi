classdef t_openbci_interface < handle
%
% This object implements an openBCI interface to read data from the serial port.
%
% It is meant to be used with OpenBCI 32bit board "chipkit". With this hardware,
% the dongle gets the data from the board and broadcast it on the serial 
% port: COM8, using the FTDI driver. 
%
% This interface handles the basic communications required to extract data
% from the device:
%
% - t_openbci_interface, constructor
% - open_port, open the com port
% - reset_hardware, reset the hardware and check the validity of the status message
% - start_streaming, sets the device in streaming mode
% - stop_streaming, sets the device in not streaming mode
% - get_bytes_available, returns the number of bytes in the buffer
% - get_eeg_packets, reads available packets and return the eeg samples of all channels
% - flush_buffer, flushes the serial buffer entirely
%
% Reference: http://docs.openbci.com/software/02-OpenBCI_Streaming_Data_Format
%
% Frederic Simard, Atom Embedded, 2015
%
    
    properties(Access=public)
        
        %open bci serial port
        com_port;
        
    end
    
    methods(Access=public)
        
        % Initialise the open bci com port
        function openbci_interface = t_openbci_interface()
            openbci_constants;

            % close any openbci serial port previously opened
            close_existing_openbci();
            
            % configure the port
            openbci_interface.com_port = serial(OPENBCI_PORT,'BaudRate',OPENBCI_BAUDRATE);
        end
        
        % Open the port
        function open_port(openbci_interface)
            fopen(openbci_interface.com_port);
        end
        
        % stop streaming and close the port
        function close_port(openbci_interface)
            % stop streaming
            fwrite(openbci_interface.com_port,STOP_STREAMING_MSG,'uchar');

            % close
            fclose(openbci_interface.com_port);
        end
        
        % Reset the hardware and validate the status message
        function [result] = reset_hardware(openbci_interface)
            openbci_constants;
            
            result = 0;
            
            % write a reset
            fwrite(openbci_interface.com_port,RESET_MSG,'uchar');
            
            % read the status data
            status_chunk = fread(openbci_interface.com_port,STATUS_PACKET_LENGTH);
            
            % check device status
            if ~isempty(status_chunk)
                if ~strcmp(char(status_chunk(end-2:end))','$$$');
                    status_chunk
                    fprintf('OpenBCI: Reset didn''t recovered or msg not framed correctly\n');
                    return;
                end
            else
                fprintf('OpenBCI: Reset didn''t recovered, check Hardware\n');
                return;
            end
            
            % indicate the user that the port opened correctly
            fprintf('OpenBCI: port opening and reset proceeded correctly\n');
            
            % return OK, if everything worked fine
            result = 1;
        end
        
        % sends a "start streaming" command
        function start_streaming(openbci_interface)
            openbci_constants;
            
            % go in streaming mode
            fwrite(openbci_interface.com_port,START_STREAMING_MSG,'uchar');
        end
        
        % sends a "stop streaming" command and flush the buffer
        function stop_streaming(openbci_interface)
            openbci_constants;
            
            % stop streaming
            fwrite(openbci_interface.com_port,STOP_STREAMING_MSG,'uchar');
            
            % wait for the command to be effective
            pause(1);

            % flush the buffer
            flushinput(openbci_interface.com_port);
        end
        
        % Check how many packets are in the buffer
        function [nb_packets] = get_nb_packets_available(openbci_interface)
            openbci_constants;
            nb_packets = floor(openbci_interface.com_port.BytesAvailable/DATA_PACKET_LENGTH);
        end
        
        % Get the packets from the buffer, extract eeg samples and returns them
        function [eeg_data, nb_packets] = get_eeg_packets(openbci_interface)
            openbci_constants;
            nb_packets = floor(openbci_interface.com_port.BytesAvailable/DATA_PACKET_LENGTH);
            if nb_packets>0
                [ eeg_data ] = unpack_openbci_eeg(fread(openbci_interface.com_port,DATA_PACKET_LENGTH*nb_packets),nb_packets);
            end
        end
        
        % flush the buffer and start anew
        function flush_buffer(openbci_interface)
            flushinput(openbci_interface.com_port);
        end
    end
    
end

