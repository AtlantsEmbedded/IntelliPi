function [ Freq BandWidth ] = GetFreqBandInfo( FreqBand )
%GETFREQBANDINFO Summary of this function goes here
%   Detailed explanation goes here

switch(FreqBand)
    
    case 'Delta'
        Freq = 2.35;
        BandWidth = 1.65;
    case 'Theta'
        Freq = 6;
        BandWidth = 2;
    case 'Alpha'
        Freq = 10;
        BandWidth = 2;
    case 'Beta'
        Freq = 18.5;
        BandWidth = 6.5;
    case 'LowGamma'
        Freq = 42.5;
        BandWidth = 17.5;
    case 'HighGamma'
        Freq = 75;
        BandWidth = 15;
end



end

