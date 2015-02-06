


% defines the sounds that are used to cue the participant
PREPARE_SOUND_CUE = [0.025*sin([1/8192:1/8192:0.25]*2*pi*200)];
GO_SOUND_CUE = [0.025*sin([1/8192:1/8192:0.25]*2*pi*200)];
STOP_SOUND_CUE = [0.025*sin([1/8192:1/8192:0.25]*2*pi*200) zeros(1,2048) 0.025*sin([1/8192:1/8192:0.25]*2*pi*200)];



% display instruction
pause(5);

% prepare
sound(PREPARE_SOUND_CUE)
pause(3);

% execute
sound(GO_SOUND_CUE)
pause(7);

% stop
sound(STOP_SOUND_CUE)
