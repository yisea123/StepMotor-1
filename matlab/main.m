%% ��ƽ�����г�
function varargout = main(varargin)
%   ���ߣ��ִϴ�
%   ���ܣ���ƽ�����г�
%   QQ  ��691147548
%   �汾��20180101 V1.0
% MAIN MATLAB code for main.fig
%      MAIN, by itself, creates a new MAIN or raises the existing
%      singleton*.
%
%      H = MAIN returns the handle to a new MAIN or the handle to
%      the existing singleton*.
%
%      MAIN('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MAIN.M with the given input arguments.
%
%      MAIN('Property','Value',...) creates a new MAIN or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before main_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to main_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help main

% Last Modified by GUIDE v2.5 02-Jun-2018 21:01:49

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @main_OpeningFcn, ...
                   'gui_OutputFcn',  @main_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

%% �����
% --- Executes just before main is made visible.
function main_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to main (see VARARGIN)

% Choose default command line output for main
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes main wait for user response (see UIRESUME)
% uiwait(handles.figure1);
%��ʼ��
clc;
%clear;
%warning('off');
%����ͼ��
javaFrame = get(hObject, 'JavaFrame');
javaFrame.setFigureIcon(javax.swing.ImageIcon('picture\Icon.jpg'));

%��ʾ������
set(hObject,'Toolbar','figure');
%��������ļ��Ƿ����
xmlFileName = 'main.config';
xml_exist = exist(xmlFileName,'file');
if(xml_exist == 0)
    uiwait(msgbox('�����ļ�������','','modal'));
    close(gcf); %�رյ�ǰ����
end

%�����������ʱ�����ڴ�ʧ������
delete(instrfindall);

InitializationGlobal(handles);  %��ʼ��ϵͳ����
SetRecord(handles);             %�������ļ��ж�ȡ��Ӧ��������ѡ���¼�����õ�������ȥ

%% ��ʼ��ϵͳ����
function InitializationGlobal(handles)
clear global;
global usData;

%������ʼ��
usData.xml = xmlread('main.config');
usData.handles=handles;
usData.taskId =  0;
usData.runStatus = 0;
usData.count =0;
usData.axes1 = handles.axes1;
usData.hPlot1 = [];
cla(usData.axes1);
usData.axes2 = handles.axes2;
usData.hPlot2 = [];
cla(usData.axes2);
usData.axes3 = handles.axes3;
usData.hPlot3 = [];
cla(usData.axes3);
%������
usData.dcm_obj = datacursormode(gcf);
set(gcf,'CloseRequestFcn','closereq')
set(usData.dcm_obj, 'UpdateFcn', @LineButtonCallback);

%��ʼ��һ����ʱ��
objTaskTimer = timer('TimerFcn',{@TimerCallback, handles}, 'Period', 0.005, 'ExecutionMode', 'fixedSpacing', 'Name', 'TaskTimer'); 
usData.objTaskTimer = objTaskTimer;
%��ʾ������Ϣ
function output_txt = LineButtonCallback(obj,event_obj)
global usData;
CursorInfoAll = getCursorInfo(usData.dcm_obj);
CursorInfo = CursorInfoAll(1);
output_txt = {['X: ',num2str(CursorInfo.Position(1))],...
              ['Y: ',num2str(CursorInfo.Position(2))]
              };
   
 %% -- �������ļ��ж�ȡ��Ӧ��������ѡ���¼�����õ�������ȥ
function SetRecord(handles)
global usData;
docNode = usData.xml;
configuration = docNode.getDocumentElement;
records = configuration.getElementsByTagName('record');
for count = 0 : records.getLength - 1
    record = records.item(count);
    attr = record.getAttributes;
    key = char(attr.getNamedItem('key').getNodeValue);
    value = char(attr.getNamedItem('value').getNodeValue);
    switch key
        case 'Port' %�˿�
            set(handles.popupmenuPort,'value', str2double(value));
    end
end

%% -- ���漸��������ѡ���ֵ�������ļ���ȥ
function SaveRecord(handles)
global usData;
docNode = usData.xml;
configuration = docNode.getDocumentElement;
records = configuration.getElementsByTagName('record');
for count = 0 : records.getLength - 1
    record = records.item(count);
    attr = record.getAttributes;
    key = char(attr.getNamedItem('key').getNodeValue);
    valueAttr = attr.getNamedItem('value');
    switch key
        case 'Port'
            value = get(handles.popupmenuPort,'value');
    end
    value = num2str(value);
    valueAttr.setNodeValue(value);
end

% ����ǰ�Ȱѿ���ɾ������Ȼÿһ�α��� config�е�ÿһ���ڵ㶼���һ������
RemoveAllEmptyNode(configuration);
% �������õ�xml�ļ���
xmlwrite('main.config',docNode);


%% -- ɾ�� nodeList �� ���нڵ��еĿսڵ�
function RemoveAllEmptyNode(nodeList)
if nodeList.hasChildNodes
    RemoveEmptyNode(nodeList);
    for i = 0 : nodeList.getLength - 1
        RemoveAllEmptyNode(nodeList.item(i));
    end
end

%% -- ɾ�� nodeList �� �ӽڵ��еĿսڵ�
function RemoveEmptyNode(nodeList)
for i = nodeList.getLength - 1 : -1 : 0
    if strcmp(char(nodeList.item(i).getNodeName),'#text')
        isBlankNode = 1;
        str = char(nodeList.item(i).getNodeValue);
        for k = 1 : length(str)
            if str(k) ~= 10 && str(k) ~= 32  % ����� 10 �� LF   32�� space
                isBlankNode = 0;
                break;
            end
        end
        if isBlankNode == 1
        nodeList.removeChild(nodeList.item(i));
        end
    end
end

%%  --- Display Err message ��ʾ������Ϣ
function displayExternMsg(type, handles)
switch(type)
    case 1
        strErr = sprintf('���ڴ򿪳ɹ�');
    case 2
        strErr = sprintf('���ڴ�ʧ��');
    case 3
        strErr = sprintf('���ڹر�');
    case 4
        strErr = sprintf('�豸���ӳɹ�');
    case 5
        strErr = sprintf('�豸����ʧ��');
    case 6
        strErr = sprintf('׼����������');
    case 7
        strErr = sprintf('��ʼ�ɼ����ݣ���');
    case 8
        strErr = sprintf('�����ɼ����ݣ���');
    case 99      
        strErr = sprintf('');
    otherwise
        strErr = sprintf('δ֪�쳣');
end
set(handles.textPrompt, 'String', strErr);


%% --- ���ڴ� Executes on button press in pushbuttonStartSerial.
% --- Executes on button press in pushbuttonStartSerial.
function pushbuttonStartSerial_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonStartSerial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global usData;
set(hObject, 'Enable', 'off');
strPorts = get(handles.popupmenuPort, 'String');%��ȡ���ں� ������
displayExternMsg(99, handles); %�����ʾ����Ϣ
if(usData.runStatus == 0)
    nSelectPort = get(handles.popupmenuPort, 'Value');
    objSerial = serial(strPorts(nSelectPort), 'BaudRate',115200, 'Name', 'mcommSerial', 'InputBufferSize', 102400);
     try
        fopen(objSerial);%�򿪴���
     catch
        set(hObject, 'Enable', 'on');
        displayExternMsg(2,handles);    %���ڴ�ʧ��
        return
     end
    
     displayExternMsg(1,handles);    %���ڴ򿪳ɹ�
     pause(1);
     
     for k = 1 : 10;    
         strCmd = sprintf('LCC+CZSB=STM32');  %�����豸
         fwrite(objSerial, strCmd, 'uint8');
         pause(1);
         nRcvLen = get(objSerial, 'BytesAvailable');
        if(nRcvLen > 0)
            rcvBuf = fread(objSerial, nRcvLen, 'uint8');
        else
            rcvBuf = sprintf('');
        end
        index = strfind(char(rcvBuf)','+CZSB=OK!');
        if ~isempty(index)
            displayExternMsg(4,handles);    %�豸�򿪳ɹ�
            break;
        else
            displayExternMsg(5,handles);    %�豸��ʧ��
        end
     end
     
     pause(1);
     %displayExternMsg(6,handles);    %׼������
     usData.taskId =  1;
     
     usData.butStartObj = hObject;
     usData.objSerial = objSerial;
     set(hObject, 'Enable', 'on');
     set(hObject, 'String', 'ֹͣ');
     set(usData.butStartObj, 'BackgroundColor', [1 0 0]);
     usData.runStatus = 1;
else
    usData.runStatus = 0;
    set(hObject, 'Enable', 'on');
    set(hObject, 'String', '��ʼ');
    set(hObject, 'BackgroundColor', [0 1 0]);
    fclose(usData.objSerial);%�رմ���
    delete(usData.objSerial);%ɾ������
    clear usData.objSerial;%��մ���
    displayExternMsg(3,handles);    %���ڹر�
    usData.taskId =  0;
    
end

%% ���ڽ��ն�ʱ��
function TimerCallback(hObject, eventdata,handles) 
%displayExternMsg(6,handles);
global usData;
if ~isempty(usData.objSerial) && usData.runStatus == 1
    nBufferlength = get(usData.objSerial, 'BytesAvailable');
    maxLen = 134;
    if nBufferlength >= maxLen
        serialBuff = fread(usData.objSerial, maxLen, 'uint8');
        %disp(serialBuff);
        index = strfind(char(serialBuff)', 'L');%LCC xxxx FZU
        if ~isempty(index)     
            if index(1) ~= 1
                serialBuff = serialBuff(index(1):end);
                tempBuff = fread(usData.objSerial, index(1)-1, 'uint8');
                serialBuff(maxLen-index(1)+2:maxLen) = tempBuff;   
            end
            %disp(serialBuff);
            ParseData(serialBuff);   
        end
    end
end



%% --���ݴ���
function ParseData(serialBuff)
global usData;
if serialBuff(1) == 'L' && serialBuff(2) == 'C' && serialBuff(3) == 'C' && serialBuff(132) == 'F' && serialBuff(133) == 'Z'  && serialBuff(134) == 'U'
     c = serialBuff(4:131);
     try
          usMearsureData = strread(char(c), '%4c');
          usMearsureData = hex2dec(usMearsureData);
          %usData.count =0;
          
          usData.count =usData.count +1;
          usData.f32XAngle( usData.count) = (usMearsureData(1)-30000)/100;
          usData.f32YAngle( usData.count) = (usMearsureData(2)-30000)/100;
          usData.f32ZAngle( usData.count) = (usMearsureData(3)-30000)/100;

          usData.i16XGyro( usData.count) = (usMearsureData(4)-30000);
          usData.i16YGyro( usData.count) = (usMearsureData(5)-30000);
          usData.i16ZGyro( usData.count) = (usMearsureData(6)-30000);

          usData.hRotSpeed( usData.count) = (usMearsureData(8)-30000);
          %disp(usData.f32YAngle);

          if isempty(usData.hPlot1)
                usData.hPlot1 = plot(usData.axes1, 1:usData.count, usData.f32YAngle(1:usData.count), 'b'); 
                legend(gData.hPlot1,'�Ƕ�','Location','NorthWest');
          else
                if(usData.count<500)
                    set(usData.hPlot1, 'xData',1:usData.count,'yData',usData.f32YAngle(1:usData.count)); 
                else
                     set(usData.hPlot1, 'xData',usData.count-500:usData.count,'yData',usData.f32YAngle(usData.count-500:usData.count)); 
                end
          end

          if isempty(usData.hPlot2)
                usData.hPlot2 = plot(usData.axes2, 1:usData.count, usData.i16XGyro(1:usData.count), 'b', 1:usData.count, usData.i16YGyro(1:usData.count), 'r', 1:usData.count, usData.i16ZGyro(1:usData.count), 'black'); 
                legend(usData.hPlot2,'���ٶ�X','���ٶ�Y','���ٶ�Z','Location','NorthWest');
          else
                if(usData.count<500)
                    set(usData.hPlot2(1), 'xData',1:usData.count,'yData',usData.i16XGyro(1:usData.count)); 
                     set(usData.hPlot2(2), 'xData',1:usData.count,'yData',usData.i16YGyro(1:usData.count));
                      set(usData.hPlot2(3), 'xData',1:usData.count,'yData',usData.i16ZGyro(1:usData.count));
                else
                     set(usData.hPlot2(1), 'xData',usData.count-500:usData.count,'yData',usData.i16XGyro(usData.count-500:usData.count)); 
                     set(usData.hPlot2(2), 'xData',usData.count-500:usData.count,'yData',usData.i16YGyro(usData.count-500:usData.count)); 
                     set(usData.hPlot2(3), 'xData',usData.count-500:usData.count,'yData',usData.i16ZGyro(usData.count-500:usData.count)); 
                end
          end

          if isempty(usData.hPlot3)
                usData.hPlot3 = plot(usData.axes3, 1:usData.count, usData.hRotSpeed(1:usData.count), 'b'); 
                legend(usData.hPlot3,'���ת��','Location','NorthWest');
          else
                if(usData.count<500)
                    set(usData.hPlot3, 'xData',1:usData.count,'yData',usData.hRotSpeed(1:usData.count)); 
                else
                     set(usData.hPlot3, 'xData',usData.count-500:usData.count,'yData',usData.hRotSpeed(usData.count-500:usData.count)); 
                end
          end
      catch ex
        disp(ex.message);
        return;
     end
      
end

%% ---��ʼ�ɼ����� Executes on button press in pushbuttonstart.
function pushbuttonstart_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonstart (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global usData;
if ~isempty(usData.objSerial)
    strCmd = sprintf('LCC+SJCJ=OPEN');  %��ʼ�ɼ�����
    fwrite( usData.objSerial , strCmd, 'uint8');
    start(usData.objTaskTimer);
    displayExternMsg(7,handles);
end


%% ---�����ɼ����� Executes on button press in pushbuttonstop.
function pushbuttonstop_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonstop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global usData;
if ~isempty( usData.objSerial )
    strCmd = sprintf('LCC+SJCJ=CLOSE');  %�����ɼ�����
    fwrite( usData.objSerial , strCmd, 'uint8');
    stop(usData.objTaskTimer);
    displayExternMsg(8,handles);
end

% --- �������Executes on button press in pushbuttonclear.
function pushbuttonclear_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonclear (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global usData;
usData.count=0;   

% ---���Ƶ��ת�� Executes on slider movement.
function sliderspeed_Callback(hObject, eventdata, handles)
% hObject    handle to sliderspeed (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global usData;

if ~isempty( usData.objSerial )
    usData.speed=int16(get(handles.sliderspeed, 'Value'));
    strCmd = sprintf('LCC+MOTOR=1,%d',usData.speed);  %�����ɼ�����
    fwrite( usData.objSerial , strCmd, 'uint8');
    strErr=sprintf('speed:%d',usData.speed);
    set(handles.textPrompt, 'String', strErr);
end


%% ϵͳ��������
function varargout = main_OutputFcn(hObject, eventdata, handles) 
varargout{1} = handles.output;

function figure1_DeleteFcn(hObject, eventdata, handles)
SaveRecord(handles);%��������
delete(hObject);

% --- Executes during object creation, after setting all properties.
function popupmenuPort_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function popupmenuPort_Callback(hObject, eventdata, handles)



% --- Executes during object creation, after setting all properties.
function sliderspeed_CreateFcn(hObject, eventdata, handles)
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end
