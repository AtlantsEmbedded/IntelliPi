function [ trials_data, record_trials_info ] = load_dataset( dataset_id )


h_infofile = fopen(['Dataset_' num2str(dataset_id) '/infos']);

nb_record_trials = fscanf(h_infofile,'Subject Initials: FS_1\nNb of tasks: 2\nNb of record trials: %f\n');
nb_test_trials = fscanf(h_infofile,'Nb of test trials: %f\n');
record_trials_info = fscanf(h_infofile,'%f %f',[2,nb_record_trials]);
record_trials_info = record_trials_info';
test_trials_info = fscanf(h_infofile,'%f %f',[2,nb_test_trials]);
test_trials_info = test_trials_info';

fclose(h_infofile);

trials_data = cell(nb_record_trials,1);

for ii=1:nb_record_trials
    load(['Dataset_' num2str(dataset_id) '/trial_' num2str(ii)]);
    temp_data = IXDATA.raw.eeg.data((end-1699):end,:);
    trials_data{ii} = temp_data;
end


end

