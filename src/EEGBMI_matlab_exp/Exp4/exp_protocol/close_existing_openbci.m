function close_existing_openbci()
            
    % load constants
    openbci_constants;

    % load opened ports
    newobjs = instrfind;

    for ii=1:length(newobjs)
        if strcmp(newobjs(ii).Port,OPENBCI_PORT) && strcmp(newobjs(ii).Status,OPENBCI_PORT_STATUS_OPEN);
            fclose(newobjs(ii));
        end
    end
end

