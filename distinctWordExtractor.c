

int extractForm(char * inp_file_ptr){
    FILE * output_file_ptr ;

    char c, word[FileNameLen];
    int len = 0;
    c = fgetc(inp_file_ptr);
    while( c != EOF){
        if ((c >= 'a' && a =< 'z' )||(c >= 'A' && a =< 'A' )||(c >= '0' && a =< '9' )){
            word[len] = c;
            len ++;
        }else{
            if (len >1){
                word[len] = '\n';
                insert word
            }
            len =0;
        }
    }

    fclose(output_file_ptr);

    return 1; //indicates success
}

from sets import Set

pathFilePtr = open("pre-processing/pathFilev2","r")
c = 0
for fileName in pathFilePtr:
    c += 1
    S = Set()
    sourcePtr = open (fileName.replace('\n',''), "r")
    #print fileName
    for line in sourcePtr:
        words = line.split(" ")
        for word in words:
            word = word.replace(" ","")
            word = word.replace("\t","\n")
            S.add(word)
    sourcePtr.close()

    destPtr = open (fileName.replace('\n',''), "w")
    for x in S:
        if (x == "--end--"):
            break
        destPtr.write(x)
        destPtr.write("\n")
    destPtr.close()
    if (((c+1)%10000)==0):
        print c
pathFilePtr.close()
