import os
fileNameSourceFile = open("pre-processing/pathFilev2","r")
total_size = 0
count = 0
for fileName in fileNameSourceFile:
    fileName = fileName.replace("-out\n","")
    size = os.path.getsize(fileName)    
    total_size = size + total_size
    count += 1
    if (total_size >16000000): 
		print count
		print total_size 
		break
print (total_size)
print (count)
