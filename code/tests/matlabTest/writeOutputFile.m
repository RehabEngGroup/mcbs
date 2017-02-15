function writeOutputFile(filename, dataMatrix, muscleNames)
nMuscles=length(muscleNames);
if size(dataMatrix, 2)~=nMuscles
   error('outputWriter:inconsistentData', 'Number of columns in data matrix is different from number of muscle names') 
end
%header
fid=fopen(filename, 'w');
if fid==-1
    error('outputWriter:couldNotOpenFile', ['File' filename 'could not be opened']) 
end
for i=1:length(muscleNames)
    fprintf(fid,[muscleNames{i} '\t\t']);
end
fprintf(fid, '\n');
for i=1:length(muscleNames)
   % fprintf(fid,'read\teval\t');
   fprintf(fid,'eval\t');
end
fprintf(fid, '\n');
fclose(fid);
%Data
dlmwrite(filename, dataMatrix, '-append', 'delimiter', '\t', 'precision', 8);