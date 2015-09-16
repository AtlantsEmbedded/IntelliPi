function generate_dataset()
%
% EEG-BMI data generator
% 
% This file implements a basic EEG-BMI data generator for the purpose of prototyping
% machine learning algorithms.
%
% For this basic dataset, the solution to the labelling problem resides in comparing the
% difference of power in the alpha frequencies [7.5-12.5Hz] of 5 seconds timeseries, sampled at 250Hz. 
% This dataset simulates this feature by generating a baseline made out of pink noise and then add a 
% signal to samples which are labeled "1". The signal has one average and three variability parameters, 
% 	1) baseline of alpha amplitude for trials labeled 1, amp_label1
% 	2) the across trial variance of amplitude, amp_noise_acrs = N(0,var_acrs)
% 	3) the within trial variance of amplitude, amp_noise_withn = N(0,var_withn)
% 	4) the variance in the phase which serves to spread the 10Hz carrier around in the frequency domain, phase_noise = N(0,var_freq)
%
% Two datasets are generated and save in files: training set (1500 samples) and test set (500 samples).
% the file format is: 
% label(1) data(1,1) data(1,2) ... data(1,N)
% label(2) data(2,1) data(2,2) ... data(2,N)
%
% call the function and look for these files: 'training_set.data', 'test_set.data'
%
write_to_file = 1;
number_of_samples = 400;

fs = 220; 	% sampling freq
T = 5;     	% time in seconds
N = fs*T;   % nb of samples
tt = 1/fs:1/fs:T; % time vector
tt = tt-T/fs; % make it start from 0

alpha_freq = 10; % Signal has a freq of 10Hz
amp_label1 = 1; % mean amplitude of 10 Hz signal
var_acrs = amp_label1*0.5; % variance across trials
var_withn = amp_label1*0.5; % variance within trials

% design a filter for latent signal
b = fir1(150,[2*8/(fs) 2*12/(fs)]);

if write_to_file
    train_file = fopen('training_set.data','w'); % open train file dataset
end

% generate the training set
for ii=1:number_of_samples

	label = randi(2)-1;		% this sample's label
    
	% generate the baseline
	this_sample = pinknoise(N);

	% if this sample is labeled 1
	if label==1
		% sample noise
		amp_noise_acrs = randn*var_acrs;
		amp_noise_withn = randn(1,N)*var_withn;
		
        % latent signal white noise, bandpass filtered so that latent signal freq is between [8 12]Hz
        % generate more samples, but trunk the beginnig, this is to account for the latency of the FIR filter
        latent_signal = wgn(1,N+500,6);
        latent_signal = filter(b,1,latent_signal);
        latent_signal(1:500) = [];
        
		% build sample
		this_sample = this_sample+(amp_label1+amp_noise_acrs+amp_noise_withn).*latent_signal;
        
    end
    
    if write_to_file
        % write sample to file
        fprintf(train_file,'%i ',label);
        for jj=1:numel(this_sample)
            fprintf(train_file,'%f ',this_sample(jj));
        end
        fprintf(train_file,'\n');
    end
end

if write_to_file
    fclose(train_file);
end
% 
% test_file = fopen('test_set.data','w'); % open test file dataset
% 
% % generate the test set
% for ii=1:500
% 
% 	label = randi(2)-1;		% this sample's label
% 
% 	% generate the baseline
% 	this_sample = pinknoise(N);
% 
% 	% if this sample is labeled 1
% 	if label==1
% 		% sample noise
% 		amp_noise_acrs = randn*var_acrs;
% 		amp_noise_withn = randn(1,N)*var_withn;
% 		
% 		% build sample
% 		this_sample = this_sample+(amp_label1+amp_noise_acrs+amp_noise_withn).*sin(tt.*alpha_freq.*2*pi+phase_noise*pi);
% 	end
% 
% 	% write sample to file
% 	fprintf(test_file,'%i ',label);
% 	for jj=1:numel(this_sample)
% 		fprintf(test_file,'%f ',this_sample(jj));
% 	end
% 	fprintf(test_file,'\n');
% end
% 
% fclose(test_file);

end

