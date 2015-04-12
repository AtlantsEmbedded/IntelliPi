

clear classes
clear



StateMachine = t_StateMachineObject();
%ScreenO = StateMachine.GetScreenO();

StateMachine.StartSystem;

while StateMachine.RunStateSystem
    %ScreenO.DrawScreen;
end

