%
% Function to be used with OpenBCI 32bit board "chipkit". With this hardware,
% the dongle gets the data from the board and broadcast it on the serial 
% port: COM8, using the FTDI driver. This function take it after the packet
% has been read from the serial port. It unpacks the eeg signals (8 channels)
% and return them in a row array.
%
% Input:
%   packet, a 33 bytes long column vector, containing the data
%
% Output:
%   eeg_data, row-vector with 8 unsigned integers.
%
% Packet overview:
%
% Header
% Byte 1: 0xA0
% Byte 2: Sample Number
% EEG Data
% Note: values are 24-bit signed, MSB first
% Bytes 3-5: Data value for EEG channel 1
% Bytes 6-8: Data value for EEG channel 2
% Bytes 9-11: Data value for EEG channel 3
% Bytes 12-14: Data value for EEG channel 4
% Bytes 15-17: Data value for EEG channel 5
% Bytes 18-20: Data value for EEG channel 6
% Bytes 21-23: Data value for EEG channel 6
% Bytes 24-26: Data value for EEG channel 8
% Accelerometer Data
% Note: values are 16-bit signed, MSB first
% Bytes 27-28: Data value for accelerometer channel X
% Bytes 29-30: Data value for accelerometer channel Y
% Bytes 31-32: Data value for accelerometer channel Z
% Footer
% Byte 33: 0xC0
%
% Reference: http://docs.openbci.com/software/02-OpenBCI_Streaming_Data_Format
%
% Frederic Simard, Atom Embedded, 2015
%

%
% Function that unpacks the eeg samples from the openbci packets
%
function [ eeg_data, packet_numbers] = unpack_openbci_eeg( packet, nb_packets )

openbci_constants;

% initialize output buffers
eeg_data = zeros(nb_packets,NB_CHANNELS);
packet_numbers = zeros(nb_packets,1);

for ii=1:nb_packets

    offset = (ii-1)*DATA_PACKET_LENGTH;

    % check if the first byte correspond to the
    % standard
    if packet(1+offset) ~= PACKET_FIRST_WORD

        % show the packet for debuggin purpose
        packet
        warning('Invalid packet format');
        return;
    end
    
    % save the packet number
    packet_numbers(ii) = packet(2+offset);
    
    % extract eeg samples (24 bits) and interpret them as signed integer 32
    eeg_data(ii,1) = int24_to_int32(packet((3:5)+offset));
    eeg_data(ii,2) = int24_to_int32(packet((6:8)+offset));
    eeg_data(ii,3) = int24_to_int32(packet((9:11)+offset));
    eeg_data(ii,4) = int24_to_int32(packet((12:14)+offset));
    eeg_data(ii,5) = int24_to_int32(packet((15:17)+offset));
    eeg_data(ii,6) = int24_to_int32(packet((18:20)+offset));
    eeg_data(ii,7) = int24_to_int32(packet((21:23)+offset));
    eeg_data(ii,8) = int24_to_int32(packet((24:26)+offset));
end

end

