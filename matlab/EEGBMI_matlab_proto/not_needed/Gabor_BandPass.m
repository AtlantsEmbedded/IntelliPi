
ShowStuff = 0;

%%
% Generate the signal that will be filtered
% 

SamplingFreq = 100; % Sampling frequency (Hz)
SamplingPeriod = 1/SamplingFreq; % Sampling period (sec)
SamplingLength = 10; % Sampling length (sec)
NbOfSample = SamplingFreq*SamplingLength; % Tot number of samples

% Time reference (10 secs)
t = (0:(NbOfSample-1))*SamplingPeriod;

% Frequency Components of the "sampled" signal
f1 = 1;
f2 = 10;
f3 = 30;
f4 = 60;

% Generation of the sampled signal
SampledSignal = sin(f1*2*pi*t)+sin(f2*2*pi*t)+sin(f3*2*pi*t)+sin(f4*2*pi*t);
SampledSignal_cpy = SampledSignal;
% FFT of the sampled signal
NFFT = 2^nextpow2(numel(t));
fft_out = fft(SampledSignal,NFFT)/numel(t);
freq_axis = SamplingFreq/2*linspace(0,1,NFFT/2+1); % +1 is for the DC component at 0 Hz

% Show the FFT output
if(1)
    figure(1);
    plot(freq_axis, 2*abs(fft_out(1:NFFT/2+1)));
end

%%
% Generates the two orthogonal components of the gabor filter

SamplingFreq = 1000; % Sampling frequency (Hz)
SamplingPeriod = 1/SamplingFreq; % Sampling period (sec)

CenterFreq = 30*(2*pi);
BandWidth = 0.01*2*pi;
t = -2*pi:SamplingPeriod:2*pi;

SinPartGF = exp(-t.^2/(2*BandWidth^2)).*sin(t*CenterFreq);
CosPartGF = exp(-t.^2/(2*BandWidth^2)).*cos(t*CenterFreq);

filteredSignal = [];
filteredSignal_phase = [];
filteredSignal_sin = [];
filteredSignal_cos = [];

filteredSignal_sin = conv(SampledSignal,SinPartGF);
filteredSignal_cos = conv(SampledSignal,CosPartGF);

filteredSignal = zeros(1,length(filteredSignal_sin));
filteredSignal_phase = zeros(1,length(filteredSignal_sin));

for i=1:length(filteredSignal_sin)
    filteredSignal(i) = sqrt(filteredSignal_sin(i)^2+filteredSignal_cos(i)^2);
    filteredSignal_phase(i) = atan2(filteredSignal_sin(i),filteredSignal_cos(i)); 
end

% FFT of the filtered signal
NFFT = 2^nextpow2(numel(filteredSignal_sin));
fft_out = fft(filteredSignal_sin,NFFT)/numel(filteredSignal_sin);
freq_axis = SamplingFreq/2*linspace(0,1,NFFT/2+1); % +1 is for the DC component at 0 Hz
    
    

if(0)
    figure(1);hold on;
    plot(t,SinPartGF, 'color', 'r');
    plot(t,CosPartGF, 'color', 'b');
end

if(1)
    figure(2);
    subplot(2,2,1);
    plot(filteredSignal);
    subplot(2,2,2);
    plot(5000:6000,filteredSignal_phase(5000:6000));
    subplot(2,2,[3 4]);
    plot(freq_axis, 2*abs(fft_out(1:NFFT/2+1)));
end





