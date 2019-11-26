import random
import sys

#print(sys.argv[1:])
with open('corpus.txt') as f, open(sys.argv[2], "w") as keyfile, open(sys.argv[3], "w") as all_present, open(sys.argv[4],"w") as half_present, open (sys.argv[5],"w")as none_present:
    lines = random.sample(f.readlines(),int (sys.argv[1]))
    lines= [l.strip() for l in lines]

    #for item in lines:
    #    keyfile.write("%s" % lines[item])
    

    #Generate keyfile
    for i in range(0,len(lines)-1000):
        keyfile.write("%s\n" % lines[i])
    
    #Generate none key present sample
    lines1=lines[-1000:]
    #print(lines1)
    for item in lines1:
        none_present.write("%s\n" % item)


    #generate all key present sample
    lines2= random.sample(lines[:len(lines)-1000],1000)
    
    for item in lines2:
        all_present.write("%s\n" % item)



    lines3= random.sample(lines[:len(lines)-1000],500)
    lines3.extend(random.sample(lines1,500))

    #random.shuffle(lines3)

    for item in lines3:
        half_present.write("%s\n" % item)

    
    

    
    
    

    #print(lines)

