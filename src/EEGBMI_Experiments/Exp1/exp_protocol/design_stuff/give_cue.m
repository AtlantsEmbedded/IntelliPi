function give_cue( cue )

constants

switch(cue)
    
    case PREPARE
        % play the sound associated with this cue (one beep)
        sound(PREPARE_SOUND_CUE)
        
    case GO
        
        % play the sound associated with this cue (one beep)
        sound(GO_SOUND_CUE)
        
    case STOP
       
        % play the sound associated with this cue (two beep)
        sound(STOP_SOUND_CUE)
        
    otherwise
        error('Unknown cue code\n');
end


end

