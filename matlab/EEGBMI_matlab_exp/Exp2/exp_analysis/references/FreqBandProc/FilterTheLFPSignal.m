function [ SinFilteredLFP CosFilteredLFP ] = FilterTheLFPSignal( LFP, Freq, BandWidth )
%FILTERTHESIGNAL Filter the signal for the desired frequency band
%   Detailed explanation goes here

% Constants about the signal
SAMPLING_FREQ = 500; %(Hz)
SAMPLING_PERIOD = 1/SAMPLING_FREQ; %(s)

[GaborSinPart GaborCosPart] = CreateGaborKernel( Freq, BandWidth, SAMPLING_FREQ );
SinFilteredLFP = conv(LFP,GaborSinPart,'same');
CosFilteredLFP = conv(LFP,GaborCosPart,'same');

end

