%
% This script tests a Matlab openBCI interface for trial based experiments
%
% It is meant to be used with OpenBCI 32bit board "chipkit". With this hardware,
% the dongle gets the data from the board and broadcast it on the serial 
% port: COM8, using the FTDI driver. 
%
% The test instantiate the openbci object and record 5 trials
%
% Frederic Simard, Atom Embedded, 2015
%

%
% Instantiate the openBCI interface
% this:
%    - open the port
%    - resets the device
%    - check the status packet
%
openbci = t_openbci;

%%
%
% Run a small experiment mockup
%
% Each trial: begin, record, repeat
%
for ii=1:5
    % gives instructions
    clc;
    fprintf('ready for next trial?\n')
    % wait for ok!
    pause;
    
    % start the trial and record
    clc;
    fprintf('Trial on-going\n')
    openbci.record_trial(ii, 5);

end
