
% COM port configuration
OPENBCI_PORT = 'COM8';
OPENBCI_BAUDRATE = 115200;
OPENBCI_PORT_STATUS_OPEN = 'open';

% COM Messages for openbci
START_STREAMING_MSG = char('b');
STOP_STREAMING_MSG = char('s');
RESET_MSG = char('v');

% packets length
%STATUS_PACKET_LENGTH = 118;
STATUS_PACKET_LENGTH = 84;
DATA_PACKET_LENGTH = 33;

NB_CHANNELS = 8;
PACKET_FIRST_WORD = 160;

% sampling rate of the openbci 250Hz
SAMPLING_RATE = 250;
SAMPLING_PERIOD = 1/SAMPLING_RATE;

% timeline is allocated in blocks of 120 seconds
TIMELINE_TIME_BLOCK = 120;
TIMELINE_SAMPLES_BLOCK = TIMELINE_TIME_BLOCK/SAMPLING_PERIOD;

% display history of last 1 seconds
TIMELINE_LENGTH_TIME = 5;
% display history of last X samples
TIMELINE_LENGTH_SAMPLES = TIMELINE_LENGTH_TIME/SAMPLING_PERIOD;

% traces color
COLOR = 'rgbkrgbk';

% curves static placement on the graph
CURVE_OFFSETS = [0 1 2 3 4 5 6 7];

DISPLAY_PERIOD_SAMPLES = 50;

DISPLAY_Y_MAX = 8;

LOG_BUFFER_SIZE = 100;

% logger, folders and filename
LOG_FOLDER = 'log/';
LOG_FILENAME_BASE = 'trial_';
LOG_EXT = '.bci_dat';



%Scale Factor (Volts/count) = 4.5 Volts / gain / (2^23 - 1);
VOLT_SCALE = 4.5;
ADC_GAIN = 24;
NUMBER_SCALING = (2^23 - 1);
