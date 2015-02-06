function [ filtered_eeg_cplx ] = filter_eeg_signal( eeg_signal, freq, bandwidth )
%FILTERTHESIGNAL Filter the signal for the desired frequency band
%   Detailed explanation goes here

% Constants about the signal
SAMPLING_FREQ = 220; %(Hz)
SAMPLING_PERIOD = 1/SAMPLING_FREQ; %(s)

[gabor_sin_part, gabor_cos_part] = create_gabor_filter( freq, bandwidth, SAMPLING_FREQ );
sin_filtered_eeg = conv(eeg_signal,gabor_sin_part,'same');
cos_filtered_eeg = conv(eeg_signal,gabor_cos_part,'same');

filtered_eeg_cplx = cos_filtered_eeg + i*sin_filtered_eeg;

end

