classdef t_LogObject
    %T_LOGOBJECT Summary of this class goes here
    %   Detailed explanation goes here
    
    properties (Access = private)
        
        fid;
        
    end
    
    methods (Access = public)
        
        function LogO = t_LogObject
        
            t = clock;
            t = t([4 5]);
            LogO.fid = fopen(['log/' date '_' num2str(t(1)) 'h' num2str(t(2))],'w');
            
        end
        
        function LogExpInfos(LogO, LogExpStruct)
            
            fprintf(LogO.fid,'Subject Initials: %s\n', LogExpStruct.initials);
            fprintf(LogO.fid,'Nb of tasks: %i\n', LogExpStruct.nb_of_tasks);
            fprintf(LogO.fid,'Nb of record trials: %i\n', LogExpStruct.nb_of_record_trials);
            fprintf(LogO.fid,'Nb of test trials: %i\n', LogExpStruct.nb_of_test_trials);
            
        end
        
        
        
        function LogTrial(LogO, LogStruct)
            
            fprintf(LogO.fid,'%i %i\n',LogStruct.trial_id,LogStruct.task);
            
        end
        
        function CloseFile(LogO)
            fclose(LogO.fid);
        end
        
        
    end
    
end

