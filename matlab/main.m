%% 自平衡自行车
function varargout = main(varargin)
%   作者：林聪聪
%   功能：自平衡自行车
%   QQ  ：691147548
%   版本：20180101 V1.0
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

%% 界面打开
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
%初始化
clc;
%clear;
%warning('off');
%更改图标
javaFrame = get(hObject, 'JavaFrame');
javaFrame.setFigureIcon(javax.swing.ImageIcon('picture\Icon.jpg'));

%显示工具栏
set(hObject,'Toolbar','figure');
%检查配置文件是否存在
xmlFileName = 'main.config';
xml_exist = exist(xmlFileName,'file');
if(xml_exist == 0)
    uiwait(msgbox('配置文件不存在','','modal'));
    close(gcf); %关闭当前界面
end

%解决发生错误时，串口打开失败问题
delete(instrfindall);

InitializationGlobal(handles);  %初始化系统配置
SetRecord(handles);             %从配置文件中读取相应的下拉框选择记录并设置到界面上去

%% 初始化系统配置
function InitializationGlobal(handles)
clear global;
global usData;

%参数初始化
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
%画坐标
usData.dcm_obj = datacursormode(gcf);
set(gcf,'CloseRequestFcn','closereq')
set(usData.dcm_obj, 'UpdateFcn', @LineButtonCallback);

%初始化一个定时器
objTaskTimer = timer('TimerFcn',{@TimerCallback, handles}, 'Period', 0.005, 'ExecutionMode', 'fixedSpacing', 'Name', 'TaskTimer'); 
usData.objTaskTimer = objTaskTimer;
%显示坐标信息
function output_txt = LineButtonCallback(obj,event_obj)
global usData;
CursorInfoAll = getCursorInfo(usData.dcm_obj);
CursorInfo = CursorInfoAll(1);
output_txt = {['X: ',num2str(CursorInfo.Position(1))],...
              ['Y: ',num2str(CursorInfo.Position(2))]
              };
   
 %% -- 从配置文件中读取相应的下拉框选择记录并设置到界面上去
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
        case 'Port' %端口
            set(handles.popupmenuPort,'value', str2double(value));
    end
end

%% -- 保存几个下拉框选择的值到配置文件中去
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

% 保存前先把空行删除，不然每一次保存 config中的每一个节点都会多一个空行
RemoveAllEmptyNode(configuration);
% 保存配置到xml文件中
xmlwrite('main.config',docNode);


%% -- 删除 nodeList 下 所有节点中的空节点
function RemoveAllEmptyNode(nodeList)
if nodeList.hasChildNodes
    RemoveEmptyNode(nodeList);
    for i = 0 : nodeList.getLength - 1
        RemoveAllEmptyNode(nodeList.item(i));
    end
end

%% -- 删除 nodeList 下 子节点中的空节点
function RemoveEmptyNode(nodeList)
for i = nodeList.getLength - 1 : -1 : 0
    if strcmp(char(nodeList.item(i).getNodeName),'#text')
        isBlankNode = 1;
        str = char(nodeList.item(i).getNodeValue);
        for k = 1 : length(str)
            if str(k) ~= 10 && str(k) ~= 32  % 这里的 10 是 LF   32是 space
                isBlankNode = 0;
                break;
            end
        end
        if isBlankNode == 1
        nodeList.removeChild(nodeList.item(i));
        end
    end
end

%%  --- Display Err message 显示错误信息
function displayExternMsg(type, handles)
switch(type)
    case 1
        strErr = sprintf('串口打开成功');
    case 2
        strErr = sprintf('串口打开失败');
    case 3
        strErr = sprintf('串口关闭');
    case 4
        strErr = sprintf('设备连接成功');
    case 5
        strErr = sprintf('设备连接失败');
    case 6
        strErr = sprintf('准备就绪！！');
    case 7
        strErr = sprintf('开始采集数据！！');
    case 8
        strErr = sprintf('结束采集数据！！');
    case 99      
        strErr = sprintf('');
    otherwise
        strErr = sprintf('未知异常');
end
set(handles.textPrompt, 'String', strErr);


%% --- 串口打开 Executes on button press in pushbuttonStartSerial.
% --- Executes on button press in pushbuttonStartSerial.
function pushbuttonStartSerial_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonStartSerial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global usData;
set(hObject, 'Enable', 'off');
strPorts = get(handles.popupmenuPort, 'String');%获取串口号 波特率
displayExternMsg(99, handles); %清空提示栏信息
if(usData.runStatus == 0)
    nSelectPort = get(handles.popupmenuPort, 'Value');
    objSerial = serial(strPorts(nSelectPort), 'BaudRate',115200, 'Name', 'mcommSerial', 'InputBufferSize', 102400);
     try
        fopen(objSerial);%打开串口
     catch
        set(hObject, 'Enable', 'on');
        displayExternMsg(2,handles);    %串口打开失败
        return
     end
    
     displayExternMsg(1,handles);    %串口打开成功
     pause(1);
     
     for k = 1 : 10;    
         strCmd = sprintf('LCC+CZSB=STM32');  %查找设备
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
            displayExternMsg(4,handles);    %设备打开成功
            break;
        else
            displayExternMsg(5,handles);    %设备打开失败
        end
     end
     
     pause(1);
     %displayExternMsg(6,handles);    %准备就绪
     usData.taskId =  1;
     
     usData.butStartObj = hObject;
     usData.objSerial = objSerial;
     set(hObject, 'Enable', 'on');
     set(hObject, 'String', '停止');
     set(usData.butStartObj, 'BackgroundColor', [1 0 0]);
     usData.runStatus = 1;
else
    usData.runStatus = 0;
    set(hObject, 'Enable', 'on');
    set(hObject, 'String', '开始');
    set(hObject, 'BackgroundColor', [0 1 0]);
    fclose(usData.objSerial);%关闭串口
    delete(usData.objSerial);%删除串口
    clear usData.objSerial;%清空串口
    displayExternMsg(3,handles);    %串口关闭
    usData.taskId =  0;
    
end

%% 串口接收定时器
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



%% --数据处理
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
                legend(gData.hPlot1,'角度','Location','NorthWest');
          else
                if(usData.count<500)
                    set(usData.hPlot1, 'xData',1:usData.count,'yData',usData.f32YAngle(1:usData.count)); 
                else
                     set(usData.hPlot1, 'xData',usData.count-500:usData.count,'yData',usData.f32YAngle(usData.count-500:usData.count)); 
                end
          end

          if isempty(usData.hPlot2)
                usData.hPlot2 = plot(usData.axes2, 1:usData.count, usData.i16XGyro(1:usData.count), 'b', 1:usData.count, usData.i16YGyro(1:usData.count), 'r', 1:usData.count, usData.i16ZGyro(1:usData.count), 'black'); 
                legend(usData.hPlot2,'角速度X','角速度Y','角速度Z','Location','NorthWest');
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
                legend(usData.hPlot3,'电机转速','Location','NorthWest');
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

%% ---开始采集数据 Executes on button press in pushbuttonstart.
function pushbuttonstart_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonstart (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global usData;
if ~isempty(usData.objSerial)
    strCmd = sprintf('LCC+SJCJ=OPEN');  %开始采集数据
    fwrite( usData.objSerial , strCmd, 'uint8');
    start(usData.objTaskTimer);
    displayExternMsg(7,handles);
end


%% ---结束采集数据 Executes on button press in pushbuttonstop.
function pushbuttonstop_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonstop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global usData;
if ~isempty( usData.objSerial )
    strCmd = sprintf('LCC+SJCJ=CLOSE');  %结束采集数据
    fwrite( usData.objSerial , strCmd, 'uint8');
    stop(usData.objTaskTimer);
    displayExternMsg(8,handles);
end

% --- 清空数据Executes on button press in pushbuttonclear.
function pushbuttonclear_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonclear (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global usData;
usData.count=0;   

% ---控制电机转速 Executes on slider movement.
function sliderspeed_Callback(hObject, eventdata, handles)
% hObject    handle to sliderspeed (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global usData;

if ~isempty( usData.objSerial )
    usData.speed=int16(get(handles.sliderspeed, 'Value'));
    strCmd = sprintf('LCC+MOTOR=1,%d',usData.speed);  %结束采集数据
    fwrite( usData.objSerial , strCmd, 'uint8');
    strErr=sprintf('speed:%d',usData.speed);
    set(handles.textPrompt, 'String', strErr);
end


%% 系统产生函数
function varargout = main_OutputFcn(hObject, eventdata, handles) 
varargout{1} = handles.output;

function figure1_DeleteFcn(hObject, eventdata, handles)
SaveRecord(handles);%保存配置
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
