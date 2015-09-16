

clc;

hudpr = dsp.UDPReceiver;

hudpr = dsp.UDPReceiver('LocalIPPort',5001,'RemoteIPAddress','127.0.0.1','MessageDataType','int32')

msg_count = 0;

while msg_count<20
    dataReceived = step(hudpr);
    if numel(dataReceived)>1
        msg_count = msg_count+1;
        numel(dataReceived)
        dataReceived
    end
end


release(hudpr);

