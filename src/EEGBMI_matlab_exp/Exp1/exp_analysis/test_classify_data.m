function results = test_classify_data()

[ trials_data, record_trials_info ] = load_dataset( 1 );
[ feature_vectors ] = preprocess_data( trials_data );
record_trials_info(record_trials_info(:,2)==2,2)=-1;

results = zeros(10,1);

for ii=1:10
    perm_idx = randperm(70);

    feature_vectors = feature_vectors(perm_idx,:);
    record_trials_info = record_trials_info(perm_idx,:);

    train_vectors = feature_vectors(1:56,:);
    test_vectors = feature_vectors(57:end,:);

    train_labels = record_trials_info(1:56,2);
    test_labels = record_trials_info(57:end,2);

    fprintf('ratio train set: %.2f\n',sum(train_labels==1)/numel(train_labels))
    fprintf('ratio test set: %.2f\n',sum(test_labels==1)/numel(test_labels))
    
    SVMStruct = svmtrain(train_vectors,train_labels);
    predicted_labels = svmclassify(SVMStruct,test_vectors);

    results(ii) = sum(test_labels==predicted_labels)/numel(predicted_labels);
    fprintf('Result: %.2f\n',results(ii))
end

mean(results)

end

