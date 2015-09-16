%
% This script will execute every analysis methods on each dataset, in sequence.
% 

%%

fprintf('\n');
fprintf('******************\n');
fprintf('**  FFT-fda  *****\n');
fprintf('******************\n');
fprintf('\n');

% options for data analysis
normalize = 3; % normalization
window_size = 600; % sampling window
train_proportion = 0.85; % percentage of training samples

testbench_fft_fda_bayes_svm(3,normalize,window_size,train_proportion);
testbench_fft_fda_bayes_svm(4,normalize,window_size,train_proportion);
testbench_fft_fda_bayes_svm(5,normalize,window_size,train_proportion);
testbench_fft_fda_bayes_svm(6,normalize,window_size,train_proportion);
testbench_fft_fda_bayes_svm(7,normalize,window_size,train_proportion);

%%
fprintf('\n');
fprintf('******************\n');
fprintf('**  BP-fda  *****\n');
fprintf('******************\n');
fprintf('\n');

% options for data analysis
normalize = 3; % normalization
window_size = 600; % sampling window
train_proportion = 0.85; % percentage of training samples

testbench_band_fda_bayes_svm(3,normalize,window_size,train_proportion);
testbench_band_fda_bayes_svm(4,normalize,window_size,train_proportion);
testbench_band_fda_bayes_svm(5,normalize,window_size,train_proportion);
testbench_band_fda_bayes_svm(6,normalize,window_size,train_proportion);
testbench_band_fda_bayes_svm(7,normalize,window_size,train_proportion);

%%
fprintf('\n');
fprintf('******************\n');
fprintf('*****  BP  *******\n');
fprintf('******************\n');
fprintf('\n');

% options for data analysis
normalize = 3; % normalization
window_size = 600; % sampling window
train_proportion = 0.85; % percentage of training samples

testbench_band_bayes_svm(3,normalize,window_size,train_proportion);
testbench_band_bayes_svm(4,normalize,window_size,train_proportion);
testbench_band_bayes_svm(5,normalize,window_size,train_proportion);
testbench_band_bayes_svm(6,normalize,window_size,train_proportion);
testbench_band_bayes_svm(7,normalize,window_size,train_proportion);