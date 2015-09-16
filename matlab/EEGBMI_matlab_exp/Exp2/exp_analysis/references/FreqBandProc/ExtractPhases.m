function [ PhaseOfSpike ] = ExtractPhases( SpikeTimes, SinPartOfSignal, CosPartOfSignal, SAMPLING_PERIOD )
%ExtractPhases Summary of this function goes here
%   Detailed explanation goes here

    PhaseOfSpike = zeros(numel(SpikeTimes), 1);
            
    %For each spikes
    for ss=1:numel(SpikeTimes)
    	%Extract the Phase
        LFPAddress = round(SpikeTimes(ss)/SAMPLING_PERIOD); 
        PhaseOfSpike(ss) = atan2(SinPartOfSignal(LFPAddress),CosPartOfSignal(LFPAddress));
    end
end

