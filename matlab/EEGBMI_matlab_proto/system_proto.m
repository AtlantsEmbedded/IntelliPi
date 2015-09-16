clear;

%
% This file implements the prototype of the processing steps required for the 
% EEG BMI system.
%
%
%
% The preprocessing prototype goes like this:
%
% 1) Define context(sampling rate, timeline, ...)
% 2) Read datafiles
% 3) Average samples over each of the two classes and show them (if verbosity>2)
%
% 4) filter and downsample signal to speedup spectrogram. (Show it if verbosity>2)
% 5) Build spectrogram of each trial and vectorize them, these are our features vectors
% 6) Train the SVM on the training set
%
% The run-time prototype goes like this:
%
% 7) Load the test set and test the SVM
% 8) report performance
%
%


verbosity = 3;

%
% 1) signal definition
%
fs = 220; 	% sampling freq
T = 5;     	% time in seconds
N = fs*T;   % nb of samples
tt = 1/fs:1/fs:T; % time vector
tt = tt-T/fs; % make it start from 0

%
% 2) open file and load a few samples in memory
%
nb_samples = 400;
nb_test_samples = 100;
ptraindata_file = fopen('training_set.data','r');

labels = zeros(nb_samples,1);
samples = zeros(nb_samples,N);

label0_PSD = zeros(65,122);
label1_PSD = zeros(65,122);

% read files
for ii=1:nb_samples
    labels(ii) = fscanf(ptraindata_file,'%f',1);
    samples(ii,:) = fscanf(ptraindata_file,'%f',N);
    
    % 
    % 3) get the spectrogram of each trial
    %
    [S,F,T,P] = spectrogram(samples(ii,:),128,120,128,fs,'yaxis');
    
    % compute the total PSD for each class
    if labels(ii)
        label1_PSD = label1_PSD+P;
    else
        label0_PSD = label0_PSD+P;
    end
    
end

test_samples = samples(1:nb_test_samples,:);
test_labels = labels(1:nb_test_samples,:);
samples(1:nb_test_samples,:) = [];
labels(1:nb_test_samples,:) = [];

nb_samples = 300;



% close file
fclose(ptraindata_file);

% compute the average PSD of each class
label1_PSD = label1_PSD/sum(labels);
label0_PSD = label0_PSD/sum(~labels);

%
% 3') if above verbosity threshold, show average PSD.
%
if verbosity>2
    
    %%
    % show PSDs of each class
    figure('color','w')
    subplot(211)
    imagesc(label0_PSD,[0 0.1])
    title('Average PSD: Label 0')
    subplot(212)
    imagesc(label1_PSD,[0 0.1])
    title('Average PSD: Label 1')

    %%
    % display the raw signal of a single sample labeled 1
    figure('color','w')
    label_1_list = find(labels==1);
    subplot(311)
    plot(tt,samples(label_1_list(1),:))
    
    title('Raw signal: EEG-BMI original signal(t)')
    
    axis([tt(1) tt(end) -2 2])
    subplot(312)
    spectrogram(samples(label_1_list(1),:),128,120,128,fs,'yaxis');
    
    title('Spectrogram')

    subplot(313)
    % display the power spectrum  of a single sample labeled 1
    nfft = 2^nextpow2(numel(samples(label_1_list(1),:))); % Next power of 2 from length of y
    this_sample_ffted = fft(samples(label_1_list(1),:),nfft)/N;
    f_scale = fs/2*linspace(0,1,nfft/2+1);
    plot(f_scale,2*abs(this_sample_ffted(1:nfft/2+1)))
    axis([f_scale(1) f_scale(end) 0 max(2*abs(this_sample_ffted(1:nfft/2+1)))*1.2])

    title('Single-Sided Amplitude Spectrum')
    xlabel('Frequency (Hz)')
    ylabel('|Y(f)|')

end


%%

%
% Downsampled signal definition
%
fs = 110; 	% sampling freq
T = 5;     	% time in seconds
N = fs*T;   % nb of samples
tt = 1/fs:1/fs:T; % time vector
tt = tt-T/fs; % make it start from 0

%
% FFT settings
%
window_length = 128;
window_overlap = 0;
nfft = 64; % perform a n-point DFT

filtered_samples = zeros(size(samples));
downsampled_samples = zeros(size(samples,1),size(samples,2)/2);

power_spectrum_samples = zeros(size(samples,1),nfft/2+1,fix((size(samples,2)/2-window_overlap)/(window_length-window_overlap)));

for ii=1:nb_samples
    %
    % 4) filter and downsample signal to speedup spectrogram
    %
    
    filtered_samples = biquad_lowpass_filter(samples(ii,:));
    downsampled_samples(ii,:) = downsample(filtered_samples,2);    

    %
    % 5) Build spectrogram of each trial
    %
    [S,F,T,P] = spectrogram(downsampled_samples(ii,:),window_length,window_overlap,nfft,fs,'yaxis');
    power_spectrum_samples(ii,:,:) = P;

end

%5') Vectorize them and add them to dataset. These are our feature vectors
train_vectors = reshape(power_spectrum_samples,size(power_spectrum_samples,1),size(power_spectrum_samples,2)*size(power_spectrum_samples,3));

  
%%
%
% Downsampled signal definition
%
fs = 110; 	% sampling freq
T = 5;     	% time in seconds
N = fs*T;   % nb of samples
tt = 1/fs:1/fs:T; % time vector
tt = tt-T/fs; % make it start from 0

%
% And show it, if verbosity>2
%
if verbosity>2
    
    % display the raw signal of a single sample labeled 1
    figure('color','w')
    label_1_list = find(labels==1);
    subplot(311)
    plot(tt,downsampled_samples(label_1_list(1),:))
    
    title('Raw signal: EEG-BMI downsampled signal(t)')
    
    axis([tt(1) tt(end) -2 2])
    subplot(312)
    spectrogram(downsampled_samples(label_1_list(1),:),128,120,128,fs,'yaxis');
    
    title('Spectrogram')

    subplot(313)
    % display the power spectrum  of a single sample labeled 1
    nfft = 2^nextpow2(numel(downsampled_samples(label_1_list(1),:))); % Next power of 2 from length of y
    this_sample_ffted = fft(downsampled_samples(label_1_list(1),:),nfft)/N;
    f_scale = fs/2*linspace(0,1,nfft/2+1);
    plot(f_scale,2*abs(this_sample_ffted(1:nfft/2+1)))
    axis([f_scale(1) f_scale(end) 0 max(2*abs(this_sample_ffted(1:nfft/2+1)))*1.2])

    title('Single-Sided Amplitude Spectrum')
    xlabel('Frequency (Hz)')
    ylabel('|Y(f)|')
end

%%
%
% 6) Train the SVM on the training set
%
SVMStruct = svmtrain(train_vectors,labels);

%
% 7) Preprocess (filter-downsample) the test set and run the SVM classify
% 
%
% FFT settings
%
window_length = 128;
window_overlap = 0;
nfft = 64; % perform a n-point DFT
power_spectrum_test_samples = zeros(size(test_samples,1),nfft/2+1, fix((size(test_samples,2)/2-window_overlap)/(window_length-window_overlap)));

for ii=1:nb_test_samples

    filtered_test_samples = biquad_lowpass_filter(test_samples(ii,:));
    downsampled_test_samples(ii,:) = downsample(filtered_test_samples,2);    
    [S,F,T,P] = spectrogram(downsampled_test_samples(ii,:),window_length,window_overlap,nfft,fs,'yaxis');
    power_spectrum_test_samples(ii,:,:) = P;

end

test_vectors = reshape(power_spectrum_test_samples,size(power_spectrum_test_samples,1),size(power_spectrum_test_samples,2)*size(power_spectrum_test_samples,3));

svm_classified = svmclassify(SVMStruct,test_vectors);

%
% 8) Report Performance
%
success_rate = sum(test_labels == svm_classified)/numel(svm_classified);

fprintf('\nSVM classifier Report\n');
fprintf('Feature vector dim: %i\n',size(train_vectors,2));
fprintf('Success Rate: %f\n',success_rate);
