
%
% To anybody reading this file... in this case, you... Seb ;)
% I used this script on actual data, but instead of sending you the data I used, I
% generated a bunch of "fake" data so that you can run the algo. The data
% are represented by a feature vector of 300 long. I hid a gaussian function in each
% class and the LDA will find it, run the code, you'll see!!!
%

% For the test, ignore all the code that isn't for the timeseries. I can show you how
% to format data coming from a spectrogram for this code to work.





clear;
Timeseries = 1;
test_on = 1;

% I ran the algorithm both on time series, in other words raw LFPs
% and on spectrogram, which is "vectorized" matrix of frequency x time

if ~test_on
    if Timeseries
        load(['data/combined/timeseries_fda_data_trail']);
    else
        load(['data/combined/spectrogram_fda_data_trail']);
    end
end

% Where I have my general purpose function
%addpath('GeneralStuff');

% Test data (I add information to the samples as to test the algorithm)
% 600 time series of 300 time samples, half of them have a Gaussian function added from 50 to 125
% the other half have a gaussian function added from 150 to 225. The categories can then be discriminated based
% on this extra information (again, test only!)
if test_on

    TrainLabels = [zeros(300,1)+1 ; zeros(300,1)+2];
    TrainSet = zeros(600,300)+randn(600,300);
    TrainSet(1:300,51:125) = 3+randn(300,75)/4;
    TrainSet(301:600,151:225) = 3+randn(300,75)/4;

    TestSet = zeros(100,300);
    TestSet(1:50,51:125) = 1+randn(50,75)/4;
    TestSet(51:100,151:225) = 1+randn(50,75)/4;
end

options = [];
options.Fisherface = 1;

% This is done to mix the trials, in the case of LDA, it is not required, but
% it is always better to do so, just to make sure that classes aren't presented one
% at a time. (in a neural network it would screw up the algo for sure)
permute = randperm(600);

if Timeseries
    % Permute the data and the labels accrodingly
    TrainSet_perm = TrainSet(permute,:);
    TrainLabels_perm = TrainLabels(permute);
    %labels, column vector containing the labels
    %data, each row is an observation
	
	% Call the LDA or Fisher Discriminant Analysis
    [ldadata, eigvector] = fda(TrainLabels_perm, options, TrainSet_perm);
else
	% Mor stands for Morlet transform, which is the transform I used to get the spectrogram
    MorTrainSet_perm = MorTrainSet(permute,:);
    TrainLabels_perm = TrainLabels(permute);
    [ldadata, eigvector] = fda(TrainLabels_perm, options, MorTrainSet_perm);
end

%%
figure('color','w')

%
% Top figure, show the one dimension on which the test data are represented.
%
subplot(2,1,1)
hold on;

if Timeseries
    scatter(TestSet(1:size(TestSet,1)/2,:)*eigvector,zeros(size(TestSet,1)/2,1)+1,'.r')
    scatter(TestSet(size(TestSet,1)/2+1:end,:)*eigvector,zeros(size(TestSet,1)/2,1)+2,'.g')
else
    scatter(MorTestSet(1:size(MorTestSet,1)/2,:)*eigvector,zeros(size(MorTestSet,1)/2,1)+1,'.r')
    scatter(MorTestSet(size(MorTestSet,1)/2+1:end,:)*eigvector,zeros(size(MorTestSet,1)/2,1)+2,'.g')
end

title('1 dimensional clusters visualization, on test data')

axis([-10 10 0 2])

%
% Bottom figure, show the features that were used to discriminate the sets.
%
subplot(2,1,2)

if Timeseries
    filter = gausswin(5);
    filter = filter/sum(filter);
    hold on;
    plot(conv(eigvector,filter,'same)'));
    line([0 301],[mean(conv(eigvector,filter,'same)')) mean(conv(eigvector,filter,'same)'))],'linestyle','--','color','k');
    axis([1 301 -0.2 0.2])
	
	title('Features weighting in the optimal transformation found by the FDA');
	
else
    % Reshaping of the vector into a matrix, this is the spectrogram data...
    map = abs(reshape(eigvector,40,numel(eigvector)/40));
    map = map-mean(map(:));
    colormap(margecolor);
    imagesc(MetaData.SampWindTime,MetaData.faxis,conv2(map,gausswin(10)*gausswin(10)','same'));
end