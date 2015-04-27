%****************************************************************************************
% AUTHOR        : Frédéric Simard                                                       *
% DATE          : 04/15                                                                 *
% FILE          : norm_features.m                                                       *
% DESCRIPTION   : This file load and analyze EEG-BMI data obtained from the MUSE device *
%                 The analysis method applied in this file is:                          *
%                                                                                       *
%                 Pre-processing:                                                       *
%                 Apply a series of band pass filters to the signal. The features are   *
%                 the RMS of the filtered signals.                                      *
%                                                                                       *
%                 Classification (independently):                                       *
%                 Bayesian network                                                      *
%                 Radial-basis support-vector machine                                   *
%                                                                                       *
%                 A report is writing in the command window for each classifier         *
%                                                                                       *
% PARAMETERS :                                                                          *
%   - dataset: dataset identifier {3,4,5,6,7}                                           *
%   - normalize: option to select the normalization to be applied                       *
%   - window_length: length of the sampling window, measured from the end of the sampled*
%                    trial.                                                             *
%   - trainset_proportion: proportion of training samples out of the dataset.           *
%                                                                                       *
% RETOURS :                                                                             *
%   - aucun                                                                             *
%                                                                                       *
%****************************************************************************************
function testbench_band_bayes_svm(dataset, normalize, window_length, trainset_proportion)

% complete the input arguments with default values (if required)
switch nargin 
    
    case 4
        
    case 3
        trainset_proportion = 0.85;
        
    case 2
        trainset_proportion = 0.85;
        window_length = 600;
        
    case 1
        trainset_proportion = 0.85;
        window_length = 600;
        normalize = 3;
        
    otherwise
        error('testbench_fft_fda_bayes_svm: Invalid number of inputs')
    
end


% Load dataset
[ trials_data, record_trials_info_original ] = load_data( dataset, window_length );

% fft the timeseries
[ feature_vectors_original ] = bandsplit_data( trials_data );

% normalize the feature vectors, according to the selected rule
feature_vectors_original = norm_features( feature_vectors_original, normalize );

% define the number of data that will be used for training
nb_train = floor(numel(trials_data)*trainset_proportion);
nb_test = size(trials_data,1)-nb_train;

% success rate array (bootstrapping 1000 times)
bootstrap_iter = 1000;
success_rate = zeros(1000,1);
nl_success_rate = zeros(1000,1);

% run for the number of bootstrap iter defined
for ii=1:bootstrap_iter

    % permute the samples
    perm_idx = randperm(size(trials_data,1));

    % make copy of permutated data
    feature_vectors = feature_vectors_original(perm_idx,:);
    
    labels = record_trials_info_original(perm_idx,2);

    % divide into training
    train_vectors = feature_vectors(1:nb_train,:);
    train_labels = labels(1:nb_train,:);
    
    %  and test sets
    test_vectors = feature_vectors((nb_train+1):end,:);
    test_labels = labels((nb_train+1):end,:);

    % train bayesian decoder
    O1 = NaiveBayes.fit(train_vectors,train_labels);
    % predict the test labels
    C1 = O1.predict(test_vectors);
    % produce confusion matrix
    cMat1 = confusionmat(test_labels,C1);

    % compute success rate for this iter
    success_rate(ii) = (cMat1(1,1)+cMat1(2,2))/nb_test;
    
    cl = svmtrain(train_vectors,train_labels,'kernel_function','rbf','BoxConstraint',Inf);
    svm_predicts = svmclassify(cl,test_vectors);
    
    nl_success_rate(ii) = sum(svm_predicts==test_labels)/numel(test_labels);
    
end


% show results success rate
fprintf('************************\n');
fprintf('******Perf Report*******\n');
fprintf('Dataset: %i\n',dataset);
fprintf('Normalize: %i\n',normalize);
fprintf('Train (%%): %0.2f\n',trainset_proportion);
fprintf('Window length: %i\n',window_length);
fprintf('Naive Bayes: %0.2f±%0.2f\n',mean(success_rate)*100,std(success_rate)*100);
fprintf('svm:: %0.2f±%0.2f\n',mean(nl_success_rate)*100,std(nl_success_rate)*100);
fprintf('************************\n');


end



