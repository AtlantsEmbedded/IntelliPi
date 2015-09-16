openbci_constants;

newobjs = instrfind;
if ~isempty(newobjs)
    fclose(newobjs);
end

% init the port
s_bci = serial(OPENBCI_PORT,'BaudRate',OPENBCI_BAUDRATE);
fopen(s_bci);


%%

% write a reset
fwrite(s_bci,RESET_MSG,'uchar');

% read the status data
status_chunk = fread(s_bci,STATUS_PACKET_LENGTH);

if ~isempty(status_chunk)
    status_chunk(end-2:end)
    
    if ~strcmp(char(status_chunk(end-2:end)'),'$$$')
        error('Reset packet invalid');
    end
end

%%

% go in streaming mode
fwrite(s_bci,START_STREAMING_MSG,'uchar');

% nb of samples desired
nb_samples_desired = 100;
% running count of number of samples acquired
nb_samples_obtained = 0;

% nb of packets to read at once
nb_packets_to_read = 0;

% list of packet numbers (used to make sure we don't drop any)
packet_nb = zeros(nb_samples_desired,1);
% eeg data
eeg_data = zeros(nb_samples_desired,8);

% loop until we got all our samples
while nb_samples_obtained < nb_samples_desired

    % check how many samples are in the buffer
    nb_packets_to_read = floor(s_bci.BytesAvailable/DATA_PACKET_LENGTH);
    
    % adjust so we only get the number of samples we asked for
    if nb_samples_obtained+nb_packets_to_read > nb_samples_desired
        nb_packets_to_read = nb_samples_desired-nb_samples_obtained;
    end
    
    % if we have packets to read
    if nb_packets_to_read>0
        
        % read them from the buffer
        packets_array = fread(s_bci,DATA_PACKET_LENGTH*nb_packets_to_read);
        
        % show infos (for validation purpose only)
        fprintf('nb_packets: %i\npacket_array size: %i\n',nb_packets_to_read,numel(packets_array));
        
        % extract the eeg samples
        [eeg_data_array, packet_nb_array] = unpack_openbci_eeg(packets_array,nb_packets_to_read);
        
        % copy them in the eeg_data array
        for ii=1:nb_packets_to_read
            eeg_data(nb_samples_obtained+ii,:) = eeg_data_array(ii,:);
            packet_nb(nb_samples_obtained+ii) = packet_nb_array(ii);
        end
        
        % update the samples count
        nb_samples_obtained = nb_samples_obtained+nb_packets_to_read;
    end
end

% stop streaming
fwrite(s_bci,STOP_STREAMING_MSG,'uchar');

% wait for the command to be effective
pause(1);

% flush the buffer
flushinput(s_bci)

% test that the buffer is empty
if s_bci.BytesAvailable == 0
    clc;
    fprintf('OpenBCI Test Success!!!\n The hardware is working properly!');
end

fclose(s_bci);
