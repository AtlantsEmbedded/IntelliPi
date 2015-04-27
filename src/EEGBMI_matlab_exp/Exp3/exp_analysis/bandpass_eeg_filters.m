%****************************************************************************************
% AUTHOR        : Frédéric Simard                                                       *
% DATE          : 04/15                                                                 *
% FILE          : bandpass_eeg_filters.m                                                *
% DESCRIPTION   : Generates a series of band-pass filters, one for each bands of the eeg*
%                 signal.                                                               *
%                 - Delta 0.5-3.5 Hz                                                    *
%                 - Theta 4-7.5 Hz                                                      *
%                 - Alpha 8-12 Hz                                                       *
%                 - Beta 13-35 Hz                                                       *
%                 - Gamma 35-50 Hz                                                      *
%                                                                                       * 
%                 Band-pass filters are of the butterworth type.                        *
%                                                                                       *
% PARAMÈTRES :                                                                          *
%   - order: order of the butterworth filter                                            *
%   - samp_freq: sampling frequency                                                     *
%                                                                                       *
% RETOURS :                                                                             *
%   - filter_array: array of filters                                                    *
%   - gs: scaling factors associated with the filters (not used)                        *
%                                                                                       *
%****************************************************************************************
function [ filter_array, gs ] = bandpass_eeg_filters( order, samp_freq )

% allocate memory for the filter array
filter_array = zeros(5,2,order,3);
gs = zeros(5,1);

% compute the nyquist frequency
nyquist_freq = 0.5*samp_freq;

% Delta 0.5-3.5 Hz
w1 = 0.5/nyquist_freq;
w2 = 3.5/nyquist_freq;
[b,a] = butter(order,[w1 w2],'bandpass');
[sos,gs(1)] = tf2sos(b,a); % transfer function to second-order section
Bs = sos(:,1:3); % Section numerator polynomials
As = sos(:,4:6); % Section denominator polynomials
filter_array(1,1,:,:) = Bs;
filter_array(1,2,:,:) = As;

% Theta 4-7.5 Hz
w1 = 4/nyquist_freq;
w2 = 7.5/nyquist_freq;
[b,a] = butter(order,[w1 w2],'bandpass');
[sos,gs(2)] = tf2sos(b,a);
Bs = sos(:,1:3); % Section numerator polynomials
As = sos(:,4:6); % Section denominator polynomials
filter_array(2,1,:,:) = Bs;
filter_array(2,2,:,:) = As;

% Alpha 8-12 Hz
w1 = 8/nyquist_freq;
w2 = 12/nyquist_freq;
[b,a] = butter(order,[w1 w2],'bandpass');
[sos,gs(3)] = tf2sos(b,a);
Bs = sos(:,1:3); % Section numerator polynomials
As = sos(:,4:6); % Section denominator polynomials
filter_array(3,1,:,:) = Bs;
filter_array(3,2,:,:) = As;

% Beta 13-35 Hz
w1 = 13/nyquist_freq;
w2 = 35/nyquist_freq;
[b,a] = butter(order,[w1 w2],'bandpass');
[sos,gs(4)] = tf2sos(b,a);
Bs = sos(:,1:3); % Section numerator polynomials
As = sos(:,4:6); % Section denominator polynomials
filter_array(4,1,:,:) = Bs;
filter_array(4,2,:,:) = As;

% Gamma 35-50 Hz
w1 = 35/nyquist_freq;
w2 = 50/nyquist_freq;
[b,a] = butter(order,[w1 w2],'bandpass');
[sos,gs(5)] = tf2sos(b,a);
Bs = sos(:,1:3); % Section numerator polynomials
As = sos(:,4:6); % Section denominator polynomials
filter_array(5,1,:,:) = Bs;
filter_array(5,2,:,:) = As;

end

