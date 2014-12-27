%
% the following function can be used to visualize a sample  
%
function [S,F,T,P] = visualize_sample(this_sample)

fs = 220; 	% sampling freq
T = 5;     	% time in seconds
N = fs*T;   % nb of samples
tt = 1/fs:1/fs:T; % time vector
tt = tt-T/fs; % make it start from 0

figure('color','w');
subplot(411)
plot(tt,this_sample);
axis([tt(1) tt(end) -1 1])

subplot(412)
% show the result
nfft = 2^nextpow2(numel(this_sample)); % Next power of 2 from length of y
this_sample_ffted = fft(this_sample,nfft)/N;
f_scale = fs/2*linspace(0,1,nfft/2+1);
plot(f_scale,2*abs(this_sample_ffted(1:nfft/2+1)))
axis([f_scale(1) f_scale(end) 0 max(2*abs(this_sample_ffted(1:nfft/2+1)))*1.2])

title('Single-Sided Amplitude Spectrum of EEG-BMI sample(t)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')

subplot(4,1,[3 4])
ham_window = 128;

% window, cuts in time
% overlap defines the amount of overlap between two adjacent windows
% nfft, cuts in freq. NFFT/2 gives you the resolution of the frequency scale
% P is the power spectral 
spectrogram(this_sample,ham_window,120,128,fs,'yaxis');
[S,F,T,P] = spectrogram(this_sample,ham_window,120,128,fs,'yaxis');

% S is the short fourier transform of each hamming window size = numel(F)*numel(T)
% F is the frequency scale size = numel(F)
% T is the time vector size = numel(T)
% T is the time vector
% 
% surf(T,F,10*log10(abs(P)));
% axis tight;
% view(0,90);

end

