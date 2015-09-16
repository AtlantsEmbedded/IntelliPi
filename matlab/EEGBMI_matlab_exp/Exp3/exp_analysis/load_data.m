%****************************************************************************************
% AUTHOR        : Frédéric Simard                                                       *
% DATE          : 04/15                                                                 *
% FILE          : load_data.m                                                       *
% DESCRIPTION   : Perform the fast fourier transform on a data set of format            *
%                 nb_trials x nb_channels x time_series. The results is stored as a     *
%                 vector of format nb_trials x nb_features, in which the features are   *
%                 the fft samples of each channels concatenated one next to the other.  *
%                                                                                       *
%                 Features are then normalize according to the selected option.         *
%                 See norm_features.m for details                                       *
%                                                                                       *
% INPUTS :                                                                              *
%   - dataset_id: integer identifying the dataset to be processed                       *
%   - window_length:  length of the timeseries to be considered, measure from the end   *
%                     trials that don't have enough samples are dropped                 *
%                                                                                       *
% OUTPUTS :                                                                             *
%   - trials_data: nb_trials x nb_features array                                        *
%   - trials_info: nb_trials x 2, (:,1) is the trial_id and (:,2) is the label          *
%                                                                                       *
%****************************************************************************************
function [ trials_data, trials_info ] = load_data( dataset_id, window_length )

% read the information file, that contains info such as:
h_infofile = fopen(['dataset_' num2str(dataset_id) '/infos']);

% subject initials (not used)
subject_initial = fscanf(h_infofile,'Subject Initials: %s\n',1);
% nb of trials
nb_record_trials = fscanf(h_infofile,'Nb of tasks: 2\nNb of record trials: %f\n',1);
% nb of test trials (not used)
nb_test_trials = fscanf(h_infofile,'Nb of test trials: %f\n',1);

% get the trial ids and labels
trials_info = fscanf(h_infofile,'%f %f\n',[2,nb_record_trials]);
trials_info = trials_info';

% get the test trial ids and labels (not used)
test_trials_info = fscanf(h_infofile,'%f %f\n',[2,nb_test_trials]);
test_trials_info = test_trials_info';

% information file, fully parsed
fclose(h_infofile);

% prepare to parse trial files
trials_data = cell(nb_record_trials,1); % a cell array is used, because not all trials have the same number of samples
dropped_trials = [];

% read all trial files (.mat files)
for ii=1:nb_record_trials
    
    % load it
    load(['dataset_' num2str(dataset_id) '/trial_' num2str(ii)]);
    
    % make sure it has enough samples
    if size(IXDATA.raw.eeg.data,1)<window_length
        % no, drop the trial
        dropped_trials = [dropped_trials ii];
    else
        % yes, copy the number of samples corresponding to the width of the window
        temp_data = IXDATA.raw.eeg.data((end-(window_length-1)):end,:);
        trials_data{ii} = temp_data;
    end
    
end


fprintf('Nb of dropped trials: %i\n',numel(dropped_trials));


trials_info(dropped_trials,:) = [];
trials_data(dropped_trials) = [];


end

