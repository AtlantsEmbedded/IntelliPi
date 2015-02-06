

%
% This file load and analyze EEG-BMI data obtained from the MUSE device
% The classification approach employed is to run a fisher discriminant analysis
% to reduce the dimensionality of the data and then to train a naive Bayes classifier
%

clear


%
% Load dataset, preprocess(fft) data and form feature vectors
%
[ trials_data, baseline_data, record_trials_info_original ] = load_dataset( 2 );
[ feature_vectors_original ] = fft_data( trials_data, baseline_data );

% define the number of data that will be used for training
nb_train = 130;
nb_test = size(trials_data,1)-nb_train;

% success rate array (bootstrapping 100 times)
bootstrap_iter = 10;
success_rate = zeros(bootstrap_iter,1);
nl_success_rate = zeros(bootstrap_iter,1);

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

    predicts = classify(test_vectors,train_vectors,train_labels);
    
    success_rate(ii) = sum(predicts==test_labels)/numel(test_labels);
    
%     
%     
%     
% %     % run fda on training data
%     options = [];
%     options.Fisherface = 1;
%     [ldadata, eigvector, eigvalue] = fda(train_labels, options, train_vectors);
% %     
% %     transform test data
%      traindata = ldadata;
%      testdata = test_vectors*eigvector;
% 
%     % train bayesian decoder
%     O1 = NaiveBayes.fit(traindata,train_labels);
%     % predict the test labels
%     C1 = O1.predict(testdata);
%     % produce confusion matrix
%     cMat1 = confusionmat(test_labels,C1);
% 
%     % compute success rate for this iter
%     success_rate(ii) = (cMat1(1,1)+cMat1(2,2))/numel(test_labels);
%     
%     
%     %cl = svmtrain(train_vectors,train_labels,'kernel_function','rbf','BoxConstraint',Inf);
%     cl = svmtrain(traindata,train_labels);
%     svm_predicts = svmclassify(cl,testdata);
%     
%     nl_success_rate(ii) = sum(svm_predicts==test_labels)/numel(test_labels);
    
end

% show average success rate
mean(success_rate)

mean(nl_success_rate)

