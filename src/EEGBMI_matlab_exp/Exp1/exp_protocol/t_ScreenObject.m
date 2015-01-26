classdef t_ScreenObject < handle
    %SCREENOBJECT Summary of ScreenO class goes here
    %   Detailed explanation goes here
    
    properties (Constant)
        
        IMAGES_SET_1 = {'Car1.bmp','Car2.bmp','Car3.bmp','Car4.bmp','Car5.bmp','Car6.bmp'};
        IMAGES_SET_2 = {'House1.bmp','House2.bmp','House3.bmp','House4.bmp','House5.bmp','House6.bmp'};
        
    end
    
    
    properties (Access = public)
        
        BackGroundColor = [127 127 127];
        ScreenNumber = 0;
        WinPtr = 0;
        WinRect = 0;
        
        ConfigName = 'Fred Laptop'
        PixelBits = 32;
        WidthPix = 1366;
        HeightPix = 768;
        FrameRateHz = 60;
        NbOfBuffer = 2;
        
        Width_cm = 48;
        Height_cm = 31;
        Dist_cm = 59;
        
        SyncTestsPref = 1;
        
        ScreenUpAndRunning = 0;
        
        CenterPix = [0 0];
		
		StimuliO;
		LogO;
        
		CentralFixationOn = 0;
		TargetOn = 0;
		CandidateOn = 0;
		
		FixPoint;
		Target1;
		Target2;
        
        FFTedStimuliSet;
        CandidateRect;
        ScreenRect;
        TargetFrameRect;
        %StimLoc_deg = [-10 20];
        StimLoc_deg = [0 0];
        TargetFrameDim = 35;
        CentralFixPoint = [0 0];
        LeftFixPoint = [-5 5];
        RightFixPoint = [5 -5];
        
        LogNextFrame = 0;
        SystemOnTime = 0;
		
		Coherence = 0.60;
        
        fliptime = 0;
        CurStimIm = [];
        StimOpt = 0;
        Mask;
        CurAlpha = 1;
        
        InstructionOn = 0;
        InstructionToDisplay = '';
        
    end
    
    
    methods (Access = public)
        
        function [ScreenO] = t_ScreenObject(ConfigPreset,LogO)
            
            ScreenO.LogO = LogO;
            rand('twister',GetSecs);
            
            if nargin 
                switch(ConfigPreset)
                    
                    % Fred computer preset (default values)
                    case 0
                        
                    % Psychophysic lab preset (overwrite default values)
                    case 1
                        ScreenO.ConfigName = 'Fred Computer';
                        ScreenO.WidthPix = 1366;
                        ScreenO.HeightPix = 768;
                        
                    otherwise
                        error('invalid preset configuration ID');
                        
                end
            end
            
            ScreenO.CenterPix = [ScreenO.WidthPix/2 ScreenO.HeightPix/2];
			ScreenO.InitCentralFixPoint;
            
            %Screen('Preference', 'SkipSyncTests',ScreenO.SyncTestsPref);
            fprintf('System configured as: %s\n',ScreenO.ConfigName);
            
        end
        
        function SetCoherence(ScreenO, NewCoherence)
           
            ScreenO.Coherence = NewCoherence; 
        end
        
        function FireUpTheScreen(ScreenO, StimuliO)
           
			ScreenO.StimuliO = StimuliO;
            [ScreenO.WinPtr, ScreenO.WinRect]=Screen('OpenWindow',ScreenO.ScreenNumber,ScreenO.BackGroundColor, ...
            [0 0 ScreenO.WidthPix ScreenO.HeightPix],[],ScreenO.NbOfBuffer);
            %HideCursor
            ScreenO.ScreenUpAndRunning = 1;
            
        end
        
        
        function ShutDownTheScreen(ScreenO)
            
            Screen('CloseAll');
            ScreenO.ScreenUpAndRunning = 0;
            ShowCursor
            
        end
        
        function WinPtr = GetWindowPtr(ScreenO)
        
            if ScreenO.ScreenUpAndRunning
                WinPtr = ScreenO.WinPtr;
            else
                error('Screen not initialized yet');
            end
        end
		
		% call this function to show a central fixation point
		function ShowCentralFix(ScreenO)
			ScreenO.CentralFixationOn = 1;
		end
		
		% call this function to hide the central fixation point
		function HideCentralFix(ScreenO)
			ScreenO.CentralFixationOn = 0;
        end
		
		% call this function to show an instruction on screen
		function ShowInstruction(ScreenO,InstructionToDisplay)
            ScreenO.InstructionOn = 1;
            ScreenO.InstructionToDisplay = InstructionToDisplay;
            ScreenO.DrawScreen;
        end
        
		% call this function to hide the instruction on screen
		function HideInstruction(ScreenO)
            ScreenO.InstructionOn = 0;
        end
		
		% call this function to draw the currently configure screen
		function DrawScreen(ScreenO)
		
            if ~ScreenO.ScreenUpAndRunning
                return;
            end
            win = ScreenO.WinPtr;
			
			% draw background
            Screen('FillRect',win,ScreenO.BackGroundColor);  
            Screen('BlendFunction',win,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
			
            if ScreenO.InstructionOn
                Screen('DrawText',win, ScreenO.InstructionToDisplay, ScreenO.FixPoint.x-6*length(ScreenO.InstructionToDisplay) , ScreenO.FixPoint.y+200);
            end
            
            [fliptime_tmp] = Screen('Flip',win);
            
            if ScreenO.LogNextFrame == 1;
                ScreenO.LogNextFrame = 0;
                ScreenO.fliptime = fliptime_tmp;
            end
        end
        
		% call to get the time at which the screen was flipped
        function [fliptime] = GetFrameFlipTime(ScreenO)
            fliptime = ScreenO.fliptime;
        end
        
		% Call to add a fixation point asynchronously
		function ShowFixPoint(ScreenO, WindowID)
            	
            if ~ScreenO.ScreenUpAndRunning
                return;
            end
            
            Screen('FillRect',ScreenO.WinPtr,ScreenO.BackGroundColor); 
            
            switch(WindowID)
                
                case 1
                    Screen('gluDisk', ScreenO.WinPtr, ScreenO.FixPoint.color, ScreenO.FixPoint.x , ScreenO.FixPoint.y ,ScreenO.FixPoint.sizePix );
                case 2
                    Screen('gluDisk', ScreenO.WinPtr, ScreenO.Target1.color, ScreenO.Target1.x , ScreenO.Target1.y , ScreenO.Target1.sizePix );
                case 3
					Screen('gluDisk', ScreenO.WinPtr, ScreenO.Target2.color, ScreenO.Target2.x , ScreenO.Target2.y , ScreenO.Target2.sizePix );
                otherwise
                    error('Invalid Window ID');
            end
               
            Screen('Flip',ScreenO.WinPtr);
            
        end
        
		% Call to remove the fixation point asynchronously
		function ResetFixPoint(ScreenO, WindowID)
            	
            if ~ScreenO.ScreenUpAndRunning
                return;
            end
            
            Screen('FillRect',ScreenO.WinPtr,ScreenO.BackGroundColor); 
            Screen('Flip',ScreenO.WinPtr);
            
        end
		
    end
    
    
    methods (Access = public)
        
		%
		% Utils, degree to pixel conversions and stuff like ScreenO
		%
		
        function [Pix] = deg2Pix(ScreenO, Deg)
            
            % Convert degrees to radian
            Rad = (2*pi/360)*Deg;
            Pix = ScreenO.Dist_cm*tan(Rad);
            %factor = [ScreenO.WidthPix/ScreenO.Width_cm -ScreenO.HeightPix/ScreenO.Height_cm];
            factor = ScreenO.WidthPix/ScreenO.Width_cm;
            Pix = round(Pix.*factor);
            
        end
        
        
        function [Deg] = pix2Deg(ScreenO, Pix)
            
            factor = ScreenO.Width_cm/ScreenO.WidthPix;
            Rad = Pix*factor;
            Rad = atan2(Rad,ScreenO.Dist_cm);
            Deg = Rad/(2*pi)*360;
            
        end
        
		
		%
		% Fixation points initialisation
		%
		function InitCentralFixPoint(ScreenO)
		
			ScreenO.FixPoint.color = [0 0 0];
			ScreenO.FixPoint.x = ScreenO.deg2Pix(ScreenO.CentralFixPoint(1))+ScreenO.CenterPix(1);
			ScreenO.FixPoint.y = ScreenO.deg2Pix(ScreenO.CentralFixPoint(2))+ScreenO.CenterPix(2);
			ScreenO.FixPoint.sizePix = ScreenO.deg2Pix(0.2);
			
        end
        
		
    end
    
end

