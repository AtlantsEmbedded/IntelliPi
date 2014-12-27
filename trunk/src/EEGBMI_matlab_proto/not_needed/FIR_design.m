
fs = 220; 	% sampling freq
T = 5;     	% time in seconds
N = fs*T;   % nb of samples
tt = 1/fs:1/fs:T; % time vector
tt = tt-T/fs; % make it start from 0

latent_signal = wgn(1,N,6);

% Ex1: Very high order filter
b = fir1(2000,[2*8/(fs) 2*12/(fs)]);
filtered_signal = filter(b,1,latent_signal);
subplot(211)
freqz(b);
subplot(212)
spectrogram(filtered_signal,32,30,128,fs,'yaxis');
pause;

% Ex2: high order filter
b = fir1(500,[2*8/(fs) 2*12/(fs)]);
filtered_signal = filter(b,1,latent_signal);
subplot(211)
freqz(b);
subplot(212)
spectrogram(filtered_signal,32,30,128,fs,'yaxis');
pause;

% Ex3: low order filter
b = fir1(20,[2*8/(fs) 2*12/(fs)]);
filtered_signal = filter(b,1,latent_signal);
subplot(211)
freqz(b);
subplot(212)
spectrogram(filtered_signal,32,30,128,fs,'yaxis');
pause;

% Ex4: Good filter
b = fir1(100,[2*8/(fs) 2*12/(fs)]);
filtered_signal = filter(b,1,latent_signal);
subplot(211)
freqz(b);
subplot(212)
spectrogram(filtered_signal,32,30,128,fs,'yaxis');
pause;