function [ trials_data, record_trials_info ] = load_dataset( dataset_id )

window_length = 600;

h_infofile = fopen(['dataset_' num2str(dataset_id) '/infos']);

nb_record_trials = fscanf(h_infofile,'Subject Initials: FS\nNb of tasks: 2\nNb of record trials: %f\n');
nb_test_trials = fscanf(h_infofile,'Nb of test trials: %f\n');
record_trials_info = fscanf(h_infofile,'%f %f',[2,nb_record_trials]);
record_trials_info = record_trials_info';
test_trials_info = fscanf(h_infofile,'%f %f',[2,nb_test_trials]);
test_trials_info = test_trials_info';

fclose(h_infofile);

trials_data = cell(nb_record_trials,1);

dropped_trials = [];

for ii=1:nb_record_trials
    load(['dataset_' num2str(dataset_id) '/trial_' num2str(ii)]);
    
    if size(IXDATA.raw.eeg.data,1)<window_length
        fprintf('trial: %i dropped\n',ii);
        dropped_trials = [dropped_trials ii];
    else
        temp_data = IXDATA.raw.eeg.data((end-(window_length-1)):end,:);
        trials_data{ii} = temp_data;
    end
    
end


% for missing trials, fills in by copying the next trial with the same task
for ii=dropped_trials
    
    if ii<10
        
        sum_of_closest = zeros(window_length,4);
        
        jj=2;
        kk=0;
        
        while kk < 2
            
            if ~isempty(trials_data{jj}) && record_trials_info(jj,2)==record_trials_info(ii,2)
                sum_of_closest = sum_of_closest+trials_data{jj};
                kk=kk+1;
            end
            
            jj=jj+1;
            
        end
        
        trials_data{ii} = sum_of_closest./2;
        
    elseif ii>numel(trials_data)-10
        
        sum_of_closest = zeros(window_length,4);
        
        jj=numel(trials_data)-1;
        kk=0;
        
        while kk < 2
            
            if ~isempty(trials_data{jj}) && record_trials_info(jj,2)==record_trials_info(ii,2)
                sum_of_closest = sum_of_closest+trials_data{jj};
                kk=kk+1;
            end
            
            jj=jj-1;
            
        end
        
        trials_data{ii} = sum_of_closest./2;
        
    else
        
        sum_of_closest = zeros(window_length,4);
        
        jj=ii-1;
        kk=0;
        
        while kk < 1
            
            if ~isempty(trials_data{jj}) && record_trials_info(jj,2)==record_trials_info(ii,2)
                sum_of_closest = sum_of_closest+trials_data{jj};
                kk=kk+1;
            end
            
            jj=jj-1;
            
        end
        
        jj = ii+1;
        
        while kk < 1
            
            if ~isempty(trials_data{jj}) && record_trials_info(jj,2)==record_trials_info(ii,2)
                sum_of_closest = sum_of_closest+trials_data{jj};
                kk=kk+1;
            end
            
            jj=jj+1;
            
        end
        
        trials_data{ii} = sum_of_closest./2;
    end
       
end



end

