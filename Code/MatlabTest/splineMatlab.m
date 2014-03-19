%% Before evaluating this script, do:
% mex evalSpline.cpp splineBasisFunction.cpp
% mex createSpline.cpp splineBasisFunction.cpp


%% Read input data
dataDir='../../Data/4DofHrHaHfKf/Reduced';
inputLmtFile=fullfile(dataDir, 'InputData', 'lmt.in');
trainingData = importdata(inputLmtFile, '\t');
%
nOfDof=size(trainingData.textdata, 1)-1;

[varNames remain]=strtok(trainingData.textdata(1:nOfDof));
[A remain]=strtok(remain);
[B remain]=strtok(remain);
[N remain]=strtok(remain);

a=zeros(1, nOfDof);
b=zeros(1, nOfDof);
n=zeros(1, nOfDof, 'int32');

for k=1:nOfDof
  a(k)=str2num(A{k});
  b(k)=str2num(B{k});
  n(k)=int32(str2num(N{k}));
end

[muscNames remain]=strtok(trainingData.textdata(end)); %last header line contains the names of the muscles
while ~isempty(remain{1})
   [muscNames(end+1)  remain]=strtok(remain);
end

%% Select muscles of interest
selectedMuscles=1:length(muscNames);

%% Create spline on input data
C=createSpline(trainingData.data(:, selectedMuscles), a*pi/180, b*pi/180, n);
%% Read new evaluation points - nodes points
inputAnglesFile=fullfile(dataDir, 'NodesData', 'angles.in');
evalData=dlmread(inputAnglesFile, '\t', 1, 0);

%% Evaluate spline on new points - nodes points
tic
[lmt ma]=evalSpline(C, evalData*pi/180);
% Write to file LMT
outputLmtFile=fullfile(dataDir, 'NodesData', 'lmtMatlab.out');
writeOutputFile(outputLmtFile, lmt, muscNames(selectedMuscles));

% Write to file MA
for dof=1:nOfDof
    outputMAFile=fullfile(dataDir, 'NodesData',['ma', varNames{dof},  'Matlab.out']);
    writeOutputFile(outputMAFile, -ma(:,:,dof), muscNames(selectedMuscles));
end
toc
%% Directly in matlab:
%tic
%[lmt2 ma2]=evalSplineMatlab(C, evalData*pi/180);
%toc

%% Read new evaluation points - between nodes points
inputAnglesFile=fullfile(dataDir, 'BetweenNodesData', 'angles.in');
evalData=dlmread(inputAnglesFile, '\t', 1, 0);

%% Evaluate spline on new points - between nodes points
tic
[lmt ma]=evalSpline(C, evalData*pi/180);
% Write to file LMT
outputLmtFile=fullfile(dataDir, 'BetweenNodesData', 'lmtMatlab.out');
writeOutputFile(outputLmtFile, lmt, muscNames(selectedMuscles));

% Write to file MA
for dof=1:nOfDof
    outputMAFile=fullfile(dataDir, 'BetweenNodesData', ['ma', varNames{dof},  'Matlab.out']);
    writeOutputFile(outputMAFile, -ma(:,:,dof), muscNames(selectedMuscles));
end
toc
% Directly in matlab:
%tic
%[lmt2 ma2]=evalSplineMatlab(C, evalData*pi/180);
%toc

