#This program compiles and runs the dsse proram
import os

os.system("make compile")

total_no_of_words_in_W = 500000
max_no_of_files_supported = 1000000
max_no_of_files_2b_uploaded = 50000
NumSet = (6122)
argmnts1 =  str(total_no_of_words_in_W)+ " " + str(max_no_of_files_supported)+ " " + str(max_no_of_files_2b_uploaded)
for j in range (1):
    for i in range (10):
        print ("Ongoing "+str(j)+" th round")
        no_of_files_for_build = 6122
        no_of_add_queries = 100
        no_of_search_queries = 100
        no_of_del_queries = 100
        argmnts2= str(no_of_files_for_build)+ " " + str(no_of_add_queries)+ " " + str(no_of_search_queries)+ " " + str(no_of_del_queries)
        argmnt = "sudo nice -n -20 ./dsse.o " + argmnts1+ " "+ argmnts2
        print (argmnt)
        os.system(argmnt)
print("Results Successfully Found for Test Cases")
